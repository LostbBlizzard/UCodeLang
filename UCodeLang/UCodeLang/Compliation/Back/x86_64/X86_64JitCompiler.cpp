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


ImportUseing86x64Gen

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

constexpr GReg FirstIntLikeParam = GReg::RBX;
constexpr GReg SencdIntLikeParam = GReg::RBX;
constexpr GReg ThreeIntLikeParam = GReg::RBX;

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
			_Gen.push64(GReg::RBP);//push rbp
		}
		{
			_Gen.mov64(GReg::RBP, GReg::RSP);//mov rbp,rsp
		}

		
		//RDI is were the Input arugument is move on stack
		_Gen.push64(GReg::RCX);

		CallOffset = GetIndex();
		_Gen.call(Near32(0));
		CallInsSize = GetIndex() - CallOffset;
		
		
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
				const size_t StatckSize =32;

				_Gen.pop64(GReg::RCX);//Pass Input
				
				//_Gen.sub32(GReg::RSP, Ret_Type.Size);//stack alloc

				_Gen.push64(GReg::RAX);//move ret on stack

				_Gen.mov64(GReg::RSP, GReg::RDX);//pass pointer
				
				_Gen.sub32(GReg::RSP, StatckSize);//so CPPinterface_Set_ReturnPtr does not break the ret value


				//Input.Set_Return(&FuncRet,sizeof(int));



				_Gen.mov(GReg::r8, 4);//pass sizeof(int)
				
				_Gen.mov(GReg::RAX,*(X86Gen::Value64*)&InterpreterCPPinterface_Set_ReturnPtr);
				_Gen.call(GReg::RAX);

				_Gen.add32(GReg::RSP, StatckSize);//move stack back
				_Gen.pop64(GReg::RAX);//remove ret on stack
				
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
			_Gen.pop64(GReg::RBP);
		}
		_Gen.ret();
	}

	{
		size_t Offset = GetIndex() - CallOffset - CallInsSize;
		_Gen.r_call(_Gen.GetData(CallOffset),Near32(Offset));

		Out_NativeCallOffset = _Gen.GetIndex();

		{
			_Gen.push64(GReg::RBP);//push rbp
		}
		{
			_Gen.mov64(GReg::RBP, GReg::RSP);//mov rbp,rsp
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
				UAddress Ins = Item.Value0.AsAddress + 1;
				auto Func = State->GetMethod(Ins);
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

					BuildFallBack = false;

					NullJitCalls Call;
					Call.CPPoffset = GetIndex();
					Call.UCodeAddress = Ins;
					NullCalls.push_back(Call);
					
					FuncToLink Link;
					Link.CPPOffset = GetIndex();
					Link.OnUAddress = Call.UCodeAddress;
					LinkingData.push_back(Link);
					
					_Gen.call(Near32(9999));


					
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
						case 1:mov(GReg::RAX, V->AsInt8); break;
						case 2:mov(GReg::RAX, V->AsInt16); break;
						case 4:mov(GReg::RAX,V->AsInt32); break;
						case 8:mov(GReg::RAX, V->AsInt64); break;
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
					_Gen.pop64(GReg::RBP);//pop rbp
				}
				_Gen.ret();
				break;
			}
		}
	}


	{
		for (auto& Item : NullCalls)
		{

			size_t PlaceHolderoffset = 0;
			if (!FuncsPlaceHolder.HasValue(Item.UCodeAddress))
			{


				UnLoadedFuncPlaceHolder tep;
				tep.Offset = GetIndex();


				{//PlaceHolder Func
				
					//save values on stack
					{

					}
					
					_Gen.mov(GReg::RCX, X86_64Gen::Value64(Item.UCodeAddress));//pass the UCodeAddress

					_Gen.mov(GReg::RAX,X86_64Gen::Value64(BuildAddressPtr));
					_Gen.call(GReg::RAX);

					{//get function argument off the stack

					}
					
					
					FuncToLink Link;
					Link.CPPOffset = GetIndex();
					Link.OnUAddress = Item.UCodeAddress;
					LinkingData.push_back(Link);

					_Gen.call(Near32(0));//will be relinked when BuildAddressPtr builds the funcion we are trying to call 


					

					_Gen.ret();
				}
				PlaceHolderoffset = tep.Offset;
				FuncsPlaceHolder.AddValue(Item.UCodeAddress,std::move(tep));
			}
			else
			{
				auto& V = FuncsPlaceHolder.at(Item.UCodeAddress);
				PlaceHolderoffset = V.Offset;
			}
			auto Ptr = _Gen.GetData(Item.CPPoffset);
			UInt32 displace = (UInt32)(PlaceHolderoffset-Item.CPPoffset) -5;
			_Gen.r_call(Ptr,Near32(displace));
		}
	}

	BufferOffset += _Gen._Base._Output.ByteOutput.size();

	NullCalls.clear();
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
void X86_64JitCompiler::SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, void* X64Output)
{
	Byte* bytes = &((Byte*)X64Output)[CPPOffset];
	UInt64 ValueFunc = (UInt64)Value;
	UInt64 CallPos = (UInt64)bytes;

	
	Int32 Offset = ValueFunc > CallPos ? 
		ValueFunc - CallPos : CallPos - ValueFunc;

	_Gen.r_call(bytes,Near32(Offset));
}

void X86_64JitCompiler::mov(GReg R, X86_64Gen::Value8 Value)
{
	_Gen.mov(R, Value);
}
void X86_64JitCompiler::mov(GReg R, X86_64Gen::Value16 Value)
{
	_Gen.mov(R, Value);
}
void X86_64JitCompiler::mov(GReg R, X86_64Gen::Value32 Value)
{
	/*
	if (Value == 0)
	{

	}
	else 
	{
	*/
		_Gen.mov(R, Value);
	//}
}
void X86_64JitCompiler::mov(GReg R, X86_64Gen::Value64 Value)
{
	_Gen.mov(R, Value);
}

UCodeLangEnd

