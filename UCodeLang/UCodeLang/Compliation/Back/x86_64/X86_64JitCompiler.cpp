#include "X86_64JitCompiler.hpp"

#include <sstream>
#include <iostream>
UCodeLangStart


x86_64::GeneralRegisters GetGeneralRegister(RegisterID Value)
{
	switch (Value)
	{
	case RegisterID::A:return x86_64::GeneralRegisters::D;
	case RegisterID::B:return x86_64::GeneralRegisters::C;
	case RegisterID::C:return x86_64::GeneralRegisters::r8;
	case RegisterID::D:return x86_64::GeneralRegisters::r9;
	case RegisterID::E:return x86_64::GeneralRegisters::A;
	case RegisterID::F:return x86_64::GeneralRegisters::B;

	default:return x86_64::GeneralRegisters::Null;
	}
}


X86_64JitCompiler::X86_64JitCompiler()
{
}

X86_64JitCompiler::~X86_64JitCompiler()
{
}

void X86_64JitCompiler::Reset()
{
}

const char HEX_MAP[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char replace(unsigned char c)
{
	return HEX_MAP[c & 0x0f];
}

std::string char_to_hex(unsigned char c)
{
	std::string hex;

	// First four bytes
	char left = (c >> 4);
	// Second four bytes
	char right = (c & 0x0f);

	hex += replace(left);
	hex += replace(right);

	return hex;
}

using GReg = X86_64Gen::GReg;
bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output)
{
	//should be set by the UCode.
	bool PointerSizeIs32Bit = sizeof(void*) == 4;

	Output = &X64Output;
	_Ins = &Ins;
	
	size_t CallOffset = _Gen.GetIndex();
	
	size_t Ret_Size = 0;
	bool IsStruct = false;

	{//CPPCall-body
		
	
		_Gen.Push_Ins_CallNear(0);
		
		
		{
			if (Func->RetType._Type == ReflectionTypes::Void)
			{
				Ret_Size = 0;
				IsStruct = false;
			}
			else if (Func->RetType._Type == ReflectionTypes::uIntPtr || Func->RetType._Type == ReflectionTypes::uIntPtr
				|| Func->RetType.IsAddress()
				|| Func->RetType.IsAddressArray())
			{
				if (PointerSizeIs32Bit)
				{
					goto Is32BitInt;
				}
				else
				{
					goto Is64BitInt;
				}
				Ret_Size = 2;
			}
			else
			if (Func->RetType._Type == ReflectionTypes::uInt32 || Func->RetType._Type == ReflectionTypes::sInt32)
			{
				Is32BitInt:
				Ret_Size = 4;
				//Input.Set_Return(&FuncRet,sizeof(int));
				//_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt64 || Func->RetType._Type == ReflectionTypes::sInt64)
			{
			Is64BitInt:
				Ret_Size = 8;
				//Input.Set_Return(&FuncRet,sizeof(int));
				//_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt16 || Func->RetType._Type == ReflectionTypes::sInt16)
			{
				Ret_Size = 2;
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt8 || Func->RetType._Type == ReflectionTypes::sInt8
				|| Func->RetType._Type == ReflectionTypes::Char
				|| Func->RetType._Type == ReflectionTypes::Bool)
			{
				Ret_Size = 1;
			}
			else
			{
				return false;
			}
		}
		_Gen.Push_Ins_ret();
	}

	{
		size_t Offset = _Gen.GetIndex() - CallOffset - 6;
		_Gen.Sub_Ins_CallNear(_Gen.GetData(CallOffset), Offset);

		Out_NativeCallOffset = _Gen.GetIndex();
	}
	{//c-code body
		const bool RetTypeIsVoid = Ret_Size == 0;
		
		for (size_t i = funcAddress; i < Ins.size(); i++)
		{
			auto& Item = Ins[i];

			switch (Item.OpCode)
			{
			case InstructionSet::Store8:
			{
				auto& Reg = GetRegData(Item.Value0.AsRegister);
				auto Value = Item.Value1.AsInt8;
				Reg.Contains = AnyInt64(Value);
			}
			break;
			case InstructionSet::Store16:
			{
				auto& Reg = GetRegData(Item.Value0.AsRegister);
				auto Value = Item.Value1.AsInt16;
				Reg.Contains = AnyInt64(Value);
			}
			break;
			case InstructionSet::Store32:
			{
				auto& Reg = GetRegData(Item.Value0.AsRegister);
				auto Value = Item.Value1.AsInt32;
				Reg.Contains = AnyInt64(Value);
			}
			break;
			case InstructionSet::Store64:
			{
				auto& Reg = GetRegData(Item.Value0.AsRegister);
				auto Value = Item.Value1.AsInt32;
				Reg.Contains = AnyInt64(Value);
			}
			break;

			case InstructionSet::StoreRegToReg8:
			case InstructionSet::StoreRegToReg16:
			case InstructionSet::StoreRegToReg32:
			case InstructionSet::StoreRegToReg64:
			{
				auto& InReg = GetRegData(Item.Value0.AsRegister);
				auto& OutReg = GetRegData(Item.Value1.AsRegister);
				
				OutReg.Contains = InReg.Contains;
			}
			break;
			case InstructionSet::Return:
			{ 
				if (!RetTypeIsVoid)
				{
					auto& OutReg = GetRegData(RegisterID::OuPutRegister);
					if (auto V = OutReg.Contains.Get_If<AnyInt64>())
					{
						switch (Ret_Size)
						{
						case 1:_Gen.Push_Ins_MovImm8(GReg::A, V->AsInt8); break;
						case 2:_Gen.Push_Ins_MovImm16(GReg::A, V->AsInt16); break;
						case 4:_Gen.Push_Ins_MovImm32(GReg::A,V->AsInt32); break;
						case 8:_Gen.Push_Ins_MovImm64(GReg::A, V->AsInt64); break;
						default:return false;
						}
					}
						
				}
				_Gen.Push_Ins_ret();
			}
			break;
			default:
				return false;
				break;
			}

			if (Item.OpCode == InstructionSet::Return)
			{
				break;
			}
		}
	}
	_Ins = nullptr;
	Output = nullptr;

	X64Output = std::move(_Gen._Base._Output.ByteOutput);
	



	
	return true;
}





void BuildSysCallIns(InstructionSysCall Ins, RegisterID Reg)
{
	switch (Ins)
	{

	case UCodeLang::InstructionSysCall::Cout_CString:
		break;
	case UCodeLang::InstructionSysCall::Cout_Char:
		break;
	case UCodeLang::InstructionSysCall::Cout_Buffer:
		break;
	case UCodeLang::InstructionSysCall::Cout_ReadChar:
		
		break;
	case UCodeLang::InstructionSysCall::File_Open:
		break;
	case UCodeLang::InstructionSysCall::File_Close:
		break;
	case UCodeLang::InstructionSysCall::File_Read:
		break;
	default:
		break;
	}
}
void X86_64JitCompiler::SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, Vector<UInt8>& X64Output)
{
	_Gen.Sub_Ins_Callptr(&X64Output[CPPOffset], *(uint64_t*)&Value);
}

UCodeLangEnd

