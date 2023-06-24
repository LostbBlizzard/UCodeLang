#include "X86_64JitCompiler.hpp"

#include <sstream>
#include <iostream>
UCodeLangStart




X86_64JitCompiler::X86_64JitCompiler()
{
}

X86_64JitCompiler::~X86_64JitCompiler()
{
}

void X86_64JitCompiler::Reset()
{
}


using GReg = X86_64Gen::GReg;
using ModRM = X86_64Gen::ModRM;
using Rm = X86_64Gen::Rm;
struct JitType
{
	size_t Size = 0;
	bool IsStruct = false;

	bool IsVoid()
	{
		return Size == 0;
	}
	bool IsBadType()
	{
		return  Size == SIZE_MAX;
	}
	void SetAsBadType()
	{
		Size = SIZE_MAX;
	}
};
JitType AsJitType(const ReflectionTypeInfo& V, const ClassAssembly& assembly,bool PointerSizeIs32Bit)
{
	JitType R;

	if (V._Type == ReflectionTypes::Void)
	{
		R.Size = 0;
		R.IsStruct = false;
	}
	else if (V._Type == ReflectionTypes::uIntPtr
		|| V._Type == ReflectionTypes::sIntPtr
		|| V.IsAddress()
		|| V.IsAddressArray())
	{
		if (PointerSizeIs32Bit)
		{
			goto Is32BitInt;
		}
		else
		{
			goto Is64BitInt;
		}
		R.Size = 2;
	}
	else
		if (V._Type == ReflectionTypes::uInt32 || V._Type == ReflectionTypes::sInt32)
		{
		Is32BitInt:
			R.Size = 4;
			//Input.Set_Return(&FuncRet,sizeof(int));
			//_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
		}
		else if (V._Type == ReflectionTypes::uInt64 || V._Type == ReflectionTypes::sInt64)
		{
		Is64BitInt:
			R.Size = 8;
			//Input.Set_Return(&FuncRet,sizeof(int));
			//_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
		}
		else if (V._Type == ReflectionTypes::uInt16 || V._Type == ReflectionTypes::sInt16)
		{
			R.Size = 2;
		}
		else if (V._Type == ReflectionTypes::uInt8 || V._Type == ReflectionTypes::sInt8
			|| V._Type == ReflectionTypes::Char
			|| V._Type == ReflectionTypes::Bool)
		{
			R.Size = 1;
		}
		else
		{
			R.SetAsBadType();
		}

	return R;
}

bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output)
{
	//should be set by the UCode.
	bool PointerSizeIs32Bit = sizeof(void*) == 4;

	Output = &X64Output;
	_Ins = &Ins;
	
	size_t CallOffset = 0;
	size_t CallInsSize = 0;
	

	JitType Ret_Type;
	{//CPPCall-body
		{
			_Gen.PushByte(0x55);//push rbp
		}
		{
			_Gen.PushByte(0x48);//mov rbp,rsp
			_Gen.PushByte(0x89);
			_Gen.PushByte(0xe5);
		}

		
		//RDI is were the Input arugument is move on stack
		_Gen.Push_Ins_Push64(GReg::RCX);

		CallOffset = _Gen.GetIndex();
		_Gen.Push_Ins_CallNear(0);
		CallInsSize = _Gen.GetIndex() - CallOffset;
		
		{
			if (Func->RetType._Type == ReflectionTypes::Void)
			{
				Ret_Type.Size = 0;
				Ret_Type.IsStruct = false;
			}
			else if (Func->RetType._Type == ReflectionTypes::uIntPtr 
				|| Func->RetType._Type == ReflectionTypes::sIntPtr
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
				Ret_Type.Size = 2;
			}
			else
			if (Func->RetType._Type == ReflectionTypes::uInt32 || Func->RetType._Type == ReflectionTypes::sInt32)
			{
				Is32BitInt:
				Ret_Type.Size = 4;

				_Gen.Push_Ins_Pop64(GReg::RCX);//Pass Input

				_Gen.Push_Ins_RegToReg64(GReg::RSP, GReg::RDX);
				_Gen.Push_Ins_Push64(GReg::RAX);//pass &FuncRet
				_Gen.Push_Ins_Push64(GReg::RAX);//pass &FuncRet
				_Gen.Push_Ins_Push64(GReg::RAX);//pass &FuncRet
				

				//Input.Set_Return(&FuncRet,sizeof(int));



				_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
				
				_Gen.Push_Ins_MovImm64(GReg::RAX,*(X86Gen::Value64*)&InterpreterCPPinterface_Set_ReturnPtr);
				_Gen.Push_Ins_CallFuncPtr(GReg::RAX);

				_Gen.Push_Ins_Pop64(GReg::RAX);
				_Gen.Push_Ins_Pop64(GReg::RAX);
				_Gen.Push_Ins_Pop64(GReg::RAX);
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt64 || Func->RetType._Type == ReflectionTypes::sInt64)
			{
			Is64BitInt:
				Ret_Type.Size = 8;
				//Input.Set_Return(&FuncRet,sizeof(int));
				//_Gen.Push_Ins_MovImm32(GReg::r8, 4);//pass sizeof(int)
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt16 || Func->RetType._Type == ReflectionTypes::sInt16)
			{
				Ret_Type.Size = 2;
			}
			else if (Func->RetType._Type == ReflectionTypes::uInt8 || Func->RetType._Type == ReflectionTypes::sInt8
				|| Func->RetType._Type == ReflectionTypes::Char
				|| Func->RetType._Type == ReflectionTypes::Bool)
			{
				Ret_Type.Size = 1;
			}
			else
			{
				return false;
			}
		}

		{
			_Gen.PushByte(0x5d);//pop rbp
		}
		_Gen.Push_Ins_ret();
	}

	{
		size_t Offset = _Gen.GetIndex() - CallOffset - CallInsSize;
		_Gen.Sub_Ins_CallNear(_Gen.GetData(CallOffset), Offset);

		Out_NativeCallOffset = _Gen.GetIndex();

		{
			_Gen.PushByte(0x55);//push rbp
		}
		{
			_Gen.PushByte(0x48);//mov rbp,rsp
			_Gen.PushByte(0x89);
			_Gen.PushByte(0xe5);
		}

	}
	{//c-code body
		const bool RetTypeIsVoid = Ret_Type.IsVoid();
		
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
				auto Value = Item.Value1.AsInt64;
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

			
			case InstructionSet::Call:
			{
				bool BuildFallBack = true;
				auto Func = State->GetMethod(Item.Value0.AsAddress);
				if (Func)
				{
					JitType Call_Ret = AsJitType(Func->RetType,State->Get_Assembly(),PointerSizeIs32Bit);
					if (Call_Ret.IsBadType())
					{
						break;
					}
					Vector<JitType> Pars; Pars.resize(Func->ParsType.size());
					for (size_t i = 0; i < Pars.size(); i++)
					{
						Pars[i] = AsJitType(Func->ParsType[i], State->Get_Assembly(), PointerSizeIs32Bit);
						if (Pars[i].IsBadType())
						{
							break;
						}
					}

					{//push pars

					}

					_Gen.Push_Ins_CallNear(0);
				}

				

				if (BuildFallBack)
				{
					//build code to move back to the interpreter and call the func
					return false;
				}
			}
			break;
			case InstructionSet::CPPCall:
			{
				return false;
			}
			break;
			case InstructionSet::CppCallNamed:
			{
				auto Ptr = (UIntNative)State->Get_StaticMemPtr();
				auto NewPtr = Ptr + Item.Value0.AsUIntNative;

				String Str((const char*)NewPtr);
				auto Cpp = State->FindCPPCall(Str);
				if (Cpp.has_value())
				{
					auto& CppV = Cpp.value();
					if (CppV->NativeCall)
					{
						//use the native call
						return false;
					}
					else
					{
						//build code to move back to the interpreter and call the func
						return false;
					}

				}
				else
				{
					String CantFindMsg = "Cant find cpp Call named " + Str;
					State->Log(CantFindMsg.c_str());

					throw std::exception("bad cpp call.");//Cant find CPPCall.
				}
			}
			break;
			case InstructionSet::Return:
			{ 
				if (!RetTypeIsVoid)
				{
					auto& OutReg = GetRegData(RegisterID::OuPutRegister);
					if (auto V = OutReg.Contains.Get_If<AnyInt64>())
					{
						switch (Ret_Type.Size)
						{
						case 1:Push_Ins_MovImm8(GReg::RAX, V->AsInt8); break;
						case 2:Push_Ins_MovImm16(GReg::RAX, V->AsInt16); break;
						case 4:Push_Ins_MovImm32(GReg::RAX,V->AsInt32); break;
						case 8:Push_Ins_MovImm64(GReg::RAX, V->AsInt64); break;
						default:return false;
						}
					}
						
				}
				
			}
			break;
			#pragma region MathOp
			case InstructionSet::Add8:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt8 + Val2->AsInt8);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Add16:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt16 + Val2->AsInt16);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Add32:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt32 + Val2->AsInt32);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Add64:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt64 + Val2->AsInt64);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Sub8:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt8 - Val2->AsInt8);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Sub16:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt16 - Val2->AsInt16);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Sub32:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt32 - Val2->AsInt32);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			case InstructionSet::Sub64:
			{
				auto& In1Reg = GetRegData(Item.Value0.AsRegister);
				auto& In2Reg = GetRegData(Item.Value1.AsRegister);

				bool BuiltCode = false;
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::OuPutRegister).Contains = AnyInt64(Val1->AsInt64 - Val2->AsInt64);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					return false;
				}
			}
			break;
			
			#pragma endregion
			default:
				return false;
				break;
			}

			if (Item.OpCode == InstructionSet::Return)
			{
				{
					_Gen.PushByte(0x5d);//pop rbp
				}
				_Gen.Push_Ins_ret();
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

void X86_64JitCompiler::Push_Ins_MovImm8(X86_64Gen::GReg R, X86_64Gen::Value8 Value)
{
	_Gen.Push_Ins_MovImm8(R, Value);
}
void X86_64JitCompiler::Push_Ins_MovImm16(X86_64Gen::GReg R, X86_64Gen::Value16 Value)
{
	_Gen.Push_Ins_MovImm16(R, Value);
}
void X86_64JitCompiler::Push_Ins_MovImm32(X86_64Gen::GReg R, X86_64Gen::Value32 Value)
{
	/*
	if (Value == 0)
	{

	}
	else 
	{
	*/
		_Gen.Push_Ins_MovImm32(R, Value);
	//}
}
void X86_64JitCompiler::Push_Ins_MovImm64(X86_64Gen::GReg R, X86_64Gen::Value64 Value)
{
	_Gen.Push_Ins_MovImm64(R, Value);
}

UCodeLangEnd

