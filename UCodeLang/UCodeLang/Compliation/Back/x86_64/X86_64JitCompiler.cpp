#include "X86_64JitCompiler.hpp"

#include <sstream>
#include <iostream>
UCodeLangStart




X86_64JitCompiler::X86_64JitCompiler()
{
	auto CallConvention = X86_64IR::CallConvention();
#if UCodeLang_Platform_Windows
	CallConvention.SetWindows();
#else
	UCodeLangThrowException("Cant find Call Convetion");
#endif
	_CallConvention = _IR.AddCallConvention(std::move(CallConvention));

	_IntrCallConvention = _IR.AddCallConvention(X86_64IR::CallConvention::Get_CallInternal());
}

X86_64JitCompiler::~X86_64JitCompiler()
{
}

void X86_64JitCompiler::Reset()
{
	this->~X86_64JitCompiler();
	new (this)  X86_64JitCompiler;
}

X86_64Gen::GReg X86_64JitCompiler::To(RegisterID id)
{
	const auto& Convention = _IR.CallingConventions.at(_IntrCallConvention);
	switch (id)
	{
	case UCodeLang::RegisterID::A:return Convention.CallClobberedGRegisters[0];
	case UCodeLang::RegisterID::B:return Convention.CallClobberedGRegisters[1];
	case UCodeLang::RegisterID::C:return Convention.CallClobberedGRegisters[2];
	case UCodeLang::RegisterID::ThisRegister:return Convention.IntegerFuncionArguments[0];
	case UCodeLang::RegisterID::InPutRegister:return Convention.IntegerFuncionArguments[1];
	case UCodeLang::RegisterID::OutPutRegister:return Convention.IntegerReturnValue;
	default:break;
	}
	UCodeLangUnreachable();
}

bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<Byte>& X64Output)
{
	//should be set by the UCode.
	bool PointerSizeIs32Bit = sizeof(void*) == 4;

	Output = &X64Output;
	_Ins = &Ins;
	
	size_t CallOffset = 0;
	size_t CallInsSize = 0;
	
	
	JitFuncData ThisFuncData;
	
	X86_64IR::FuncID CppFuncID = _IR.Get_RelocationID();//it just needs to be unique
	X86_64IR::FuncID NativeFuncID = _IR.Get_RelocationID();

	X86_64IR::Relocation32 NativeFuncNearCallRelocation = X86_64IR::Relocation32(_IR.Get_RelocationID());
	{//CPPCall-body
		
		//use InterpreterCPPinterface_GetParm to get pars
		auto& newfunc = _IR.AddNewFunc(NativeFuncID);
		newfunc.CallConvention = _CallConvention;

		newfunc.Add_Ins(X86_64IR::Ins::Call(
			X86_64IR::NearRelocation32(
				NativeFuncNearCallRelocation
			)));

		newfunc.Add_Ins(X86_64IR::Ins::Ret());
	}

	{
		//UCodeIns To Call
		auto& newfunc = _IR.AddNewFunc(CppFuncID);
		newfunc.CallConvention = _IntrCallConvention;

		for (size_t i = funcAddress; i < Ins.size(); i++)
		{
			auto& UIns = Ins[i];
			switch (UIns.OpCode)
			{
			case InstructionSet::Debug_FuncStart:break;
			case InstructionSet::Debug_FuncEnd:break;
			case InstructionSet::Debug_LineEnter:break;
			case InstructionSet::Storef32v1:
			{
				auto next1 = Ins[i + 1];

				Int32 Value;
				(&Value)[0] = UIns.Op_RegUInt16.B;
				(&Value)[1] = next1.Op_RegUInt16.B;

				const auto& OutReg = UIns.Op_RegUInt16.A;

				newfunc.Add_Ins(
					X86_64IR::Ins::Move(
						X86_64IR::Ins::Move::ConstToReg(Value, To(OutReg))
					)
				);

				i++;
			}
			break;
			case InstructionSet::StoreRegToReg32:
			{
				const auto& InReg = UIns.Op_TwoReg.A;
				const auto& OutReg = UIns.Op_TwoReg.B;

				newfunc.Add_Ins(
					X86_64IR::Ins::Move(
						X86_64IR::Ins::Move::RegToReg(To(InReg), To(OutReg))
					)
				);
			}
			break;
			case InstructionSet::Return:
			{
				break;
			}
			break;
			default:return false;
			}
		}
		

		newfunc.Add_Ins(X86_64IR::Ins::Ret());
	}

	
	_IR.CleanUp(X86_64IR::CleanUpMode::RunTimeSpeed);
	auto BuildInfo = _IR.Build();
	Vector<Byte> Outputbytes;
	{
		//
		auto& CppFunc = BuildInfo.Funcs[0];
		auto& NativeFunc = BuildInfo.Funcs[1];

		Vector< X86_64IR::Relocation> Relocations;
		for (auto& Item : BuildInfo.Funcs)
		{
			Vector<Byte> Ins = std::move(Item.Bytes);
			//Do Relocs
			for (auto& Reloc : Item.Relocations)
			{
				Reloc.ByteToUpdateOffset += Outputbytes.size();
				Relocations.push_back(std::move(Reloc));
			}

			if (&NativeFunc == &Item)
			{
				Out_NativeCallOffset = Outputbytes.size();
			}
			else if (&CppFunc == &Item)
			{
				Out_CppCallOffset = Outputbytes.size();
			}

			for (auto& byte : Ins)
			{
				Outputbytes.push_back(byte);
			}
		}

		for (auto& Item : Relocations)
		{
			if (Item.RelocationId == NativeFuncNearCallRelocation.ID)
			{
				Byte* bitstoupdate = Outputbytes.data() + Item.ByteToUpdateOffset;
				Byte* callpos = Outputbytes.data() + Out_NativeCallOffset;
				int offset = (int)(callpos - bitstoupdate ) - 4;
				memcpy(bitstoupdate, &offset, sizeof(int));
			}
		}

	}

	BufferOffset += Outputbytes.size();

	NullCalls.clear();
	_Ins = nullptr;
	Output = nullptr;

	X64Output = std::move(Outputbytes);
	



	
	return true;
}

void X86_64JitCompiler::SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, void* X64Output)
{
}


UCodeLangEnd

