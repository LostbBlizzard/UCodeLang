#ifndef UCodeLangNoCompiler

#include "IRToX86_64IR.hpp"
UCodeLangStart
void IRToX86_64IR::Reset()
{
	this->~IRToX86_64IR();
	new (this) IRToX86_64IR();
}

void IRToX86_64IR::Build(const IRBuilder* builder, X86_64IR::CleanUpMode mode)
{
	_Input = builder;

	_MainCallConvention = _Out.AddCallConvention(X86_64IR::CallConvention::Get_UCodeLang());
	_InternalCallConvention = _Out.AddCallConvention(X86_64IR::CallConvention::Get_CallInternal());
	OtherCallConventions = {
		_Out.AddCallConvention(X86_64IR::CallConvention::Get_Windows_Default()),
		_Out.AddCallConvention(X86_64IR::CallConvention::Get_Windows_Cdecl()),

		_Out.AddCallConvention(X86_64IR::CallConvention::Get_Linux_Default()),
		_Out.AddCallConvention(X86_64IR::CallConvention::Get_Linux_Cdecl()),

		_Out.AddCallConvention(X86_64IR::CallConvention::Get_MacOSandIOS_Default()),
		_Out.AddCallConvention(X86_64IR::CallConvention::Get_MacOSandIOS_Cdecl()),
	};

	IRLocs.clear();
	TemVarRegs.clear();

	auto& MainCallConvention = _Out.CallingConventions.GetValue(_MainCallConvention);
	for (auto& Item : MainCallConvention.CallClobberedGRegisters)
	{
		TemVarRegs.AddValue(Item, RegInfo());
	}

	Build(&builder->_StaticInit);
	Build(&builder->_StaticdeInit);
	Build(&builder->_threadInit);
	Build(&builder->_threaddeInit);

	for (auto& Item : builder->Funcs)
	{
		Build(Item.get());
	}


	_Out.CleanUp(mode);
}
void IRToX86_64IR::Build(const IRFunc* func)
{
	OnFunc = func;
	OutFunc = &_Out.AddNewFunc(func->identifier);

	//add check here
	if (true)
	{
		OutFunc->CallConvention = _MainCallConvention;
	}
	else
	{
		OutFunc->CallConvention = _InternalCallConvention;
	}


	for (auto& Item : func->Blocks)
	{
		Build(Item.get());
	}
}
void IRToX86_64IR::Build(const IRBlock* func)
{
	for (auto& Item : func->Instructions)
	{
		switch (Item->Type)
		{
		case IRInstructionType::None:
		{

		}
		break;
		case IRInstructionType::LoadNone:
		{

		}
		break;
		case IRInstructionType::Load:
		{
			GReg r = Get_FreeReg().value();
			auto pos = LoadOp(Item.get(), Item->Target());
			SetInfo(Get_FreeReg().value(), pos);
			IRLocs.AddValue(Item.get(), pos);
		}
		break;
		case IRInstructionType::LoadReturn:
		{
			auto Returned = LoadOp(Item.get(), Item->Target());
			//OutFunc->Add_Ins()
			//MoveToReg(GetLoc(Item->Target()), );
			if (auto Val = Returned.LocTypes.Get_If<GReg>())
			{
				X86_64IR::Ins::Move::RegToFuncReturn v = X86_64IR::Ins::Move::RegToFuncReturn(ToInsRegSize(Returned.type), *Val);
				OutFunc->Add_Ins(X86_64IR::Ins::Move(v));
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
		break;
		case IRInstructionType::Return:
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	OutFunc->Add_Ins(X86_64IR::Ins::Ret());
}
IRToX86_64IR::IRLoc IRToX86_64IR::LoadOp(const IRInstruction* Value, const IROperator& Op)
{
	switch (Op.Type)
	{
	case IROperatorType::Value:
	{
		auto V = GetType(Value, Op);
		auto outreg = Get_FreeRegNoOp();
		switch (V._Type)
		{
		case IRTypes::i8:
		{
			X86_64IR::Ins::Move::ConstToReg v = X86_64IR::Ins::Move::ConstToReg(Op.Value, outreg);
			v.RegSize = X86_64IR::Ins::InsRegSize::bits8;
			OutFunc->Add_Ins(X86_64IR::Ins::Move(v));
		}
		break;
		case IRTypes::i16:
		{
			X86_64IR::Ins::Move::ConstToReg v = X86_64IR::Ins::Move::ConstToReg(Op.Value, outreg);
			v.RegSize = X86_64IR::Ins::InsRegSize::bits16;
			OutFunc->Add_Ins(X86_64IR::Ins::Move(v));
		}
		break;
		case IRTypes::i32:
		{
			X86_64IR::Ins::Move::ConstToReg v = X86_64IR::Ins::Move::ConstToReg(Op.Value, outreg);
			v.RegSize = X86_64IR::Ins::InsRegSize::bits32;
			OutFunc->Add_Ins(X86_64IR::Ins::Move(v));
		}
		break;
		case IRTypes::i64:
		{
			X86_64IR::Ins::Move::ConstToReg v = X86_64IR::Ins::Move::ConstToReg(Op.Value, outreg);
			v.RegSize = X86_64IR::Ins::InsRegSize::bits64;
			OutFunc->Add_Ins(X86_64IR::Ins::Move(v));
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
		IRLoc r;
		r.type = V;
		r.LocTypes = outreg;
		SetInfo(outreg, Op);
		return r;
	}
	break;
	case IROperatorType::IRInstruction:
	{
		auto ins = Op.Pointer;
		return GetLoc(ins);
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
UCodeLangEnd

#endif