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

GReg VarablesReg[] =
{
	GReg::RAX,
	GReg::RCX,
	GReg::RDX,
	GReg::r8,
	GReg::r9,
	GReg::r10,
	GReg::r11,
};
constexpr size_t VarablesRegSize = sizeof(VarablesReg) / sizeof(VarablesReg[0]);

#if UCodeLang_Platform_Windows
constexpr GReg IntLikeParam_1 = GReg::RCX;
constexpr GReg IntLikeParam_2 = GReg::RDX;
constexpr GReg IntLikeParam_3 = GReg::r8;
constexpr GReg IntLikeParam_4 = GReg::r9;
#endif
const size_t SaveStackSize = 32;//an offset to stop c++ touching the stack am not sure why it does that or were it specified in c++ 86_64 i you know tell me. or im must be doing sumthing worng
bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output)
{
	//should be set by the UCode.
	bool PointerSizeIs32Bit = sizeof(void*) == 4;

	Output = &X64Output;
	_Ins = &Ins;
	
	size_t CallOffset = 0;
	size_t CallInsSize = 0;
	
	
	JitFuncData ThisFuncData;
	{//CPPCall-body
		PushFuncStart();


		auto Tep = As(Func, PointerSizeIs32Bit);

		if (!Tep.has_value()) { return false; }
		ThisFuncData = Tep.value();

		size_t StackSize = SaveStackSize + ThisFuncData.Ret.GetSize();

		for (auto& Item : ThisFuncData.Pars)
		{
			StackSize += Item.GetSize();
		}

		_Gen.sub32(GReg::RSP, (Int32)StackSize);//so CPPinterface_Set_ReturnPtr does not break the FuncRet value

		_Gen.push64(IntLikeParam_1);//move Input arugument on stack

		
		{//Get Pars
			size_t IntparTypeCount = 0;
			size_t ParsSize = 0;
			for (auto& Item : ThisFuncData.Pars)
			{
				if (Item.Type == JitType_t::Int32)
				{
					IntparTypeCount++;

					//What it should do 
					//CPPinterface::Get_Paramter(Input,Pointer,sizeof(Item));


					_Gen.pop64(IntLikeParam_1);//pass Input
					_Gen.push64(IntLikeParam_1);//push copy.

					_Gen.mov(IntLikeParam_3, X86_64Gen::Value32(Item.GetSize()));//Pass sizeof(Item)


					{
						_Gen.sub32(GReg::RSP, 8);//move stack for pointer
						{
							_Gen.mov64(GReg::RSP, IntLikeParam_2);//pass pointer of stack

							_Gen.sub32(GReg::RSP, SaveStackSize);
							_Gen.mov(GReg::RAX, *(X86Gen::Value64*)&InterpreterCPPinterface_Get_Par);//CPPinterface::Get_Paramter
							_Gen.call(GReg::RAX);
							_Gen.add32(GReg::RSP, SaveStackSize);
						}
						_Gen.pop64(GReg::RAX);//Get Param
					}

					size_t StackPos = (StackSize - ParsSize - 8);
					_Gen.mov64(IndrReg(GReg::RSP), (Int32)StackPos, GReg::RAX);//move parm on stack
					ParsSize += Item.GetSize();
				}
				else
				{
					throw std::exception("not added");
				}
			}
			
		}
		
		
		{//move into native call
			size_t IntparTypeCount = 0;
			size_t ParsSize = 0;
			for (auto& Item : ThisFuncData.Pars)
			{
				if (Item.Type == JitType_t::Int32)
				{

					GReg PReg;
					switch (IntparTypeCount)
					{
					case 0:PReg = IntLikeParam_1; break;
						case 1:PReg = IntLikeParam_2; break;
						case 2:PReg = IntLikeParam_3; break;
						case 3:PReg = IntLikeParam_4; break;
					default:
						//push on stack
						throw std::exception("not added");
						break;
					}
					IntparTypeCount++;
					ParsSize += Item.GetSize();

					size_t StackPos = (StackSize - ParsSize - 8);
					_Gen.mov64(PReg,IndrReg(GReg::RSP), (Int32)StackPos);//move to Preg
				}
			}
		}

		CallOffset = GetIndex();
		Gen_InvaildNear32Call();
		CallInsSize = GetIndex() - CallOffset;

		{//pop pars
			size_t IntparTypeCount = 0;
			for (auto& Item : ThisFuncData.Pars)
			{
				if (Item.Type == JitType_t::Int32)
				{

					GReg PReg;
					switch (IntparTypeCount)
					{
					case 0:PReg = IntLikeParam_1; break;
					case 1:PReg = IntLikeParam_2; break;
					case 2:PReg = IntLikeParam_3; break;
					case 3:PReg = IntLikeParam_4; break;
					default:
						//push on stack
						throw std::exception("not added");
						break;
					}
					IntparTypeCount++;
					_Gen.pop64(GReg::RCX);//pop
				}
			}
		}
		
		{
			
			if (ThisFuncData.Ret.Type == JitType_t::Int32
				|| ThisFuncData.Ret.Type == JitType_t::Int16
				|| ThisFuncData.Ret.Type == JitType_t::Int8
				|| ThisFuncData.Ret.Type == JitType_t::Int64)
			{
				_Gen.pop64(IntLikeParam_1);//Pass Input

				_Gen.push64(GReg::RAX);//move FuncRet on stack

				_Gen.mov64(GReg::RSP, IntLikeParam_2);//pass pointer of FuncRet

				//CPPinterface::Set_Return(Input,&FuncRet,sizeof(int));

				_Gen.mov(IntLikeParam_3,X86_64Gen::Value32(ThisFuncData.Ret.GetSize()));//pass sizeof(ThisFuncData.Ret)
				

				_Gen.mov(GReg::RAX, *(X86Gen::Value64*)&InterpreterCPPinterface_Set_ReturnPtr);
				_Gen.call(GReg::RAX);

				
				_Gen.pop64(GReg::RAX);//remove FuncRet on stack
				
				
			}
			else if(ThisFuncData.Ret.Type == JitType_t::Void)
			{
				//Do Nothing.
			}
			else
			{
				throw std::exception("not added");
			}
		}
		
		
		_Gen.add32(GReg::RSP, (Int32)StackSize);//move stack back
		PushFuncEnd();
		_Gen.ret();
	}

	{
		size_t Offset = GetIndex() - CallOffset - CallInsSize;
		_Gen.r_call(_Gen.GetData(CallOffset),Near32((Int32)Offset));

		Out_NativeCallOffset = _Gen.GetIndex();
		
		PushFuncStart();
	}

	{//Set Pars
		size_t IntparTypeCount = 0;
		RegisterID UparReg = RegisterID::StartParameterRegister;
		for (auto& Item : ThisFuncData.Pars)
		{
			if (Item.Type == JitType_t::Int32)
			{

				GReg PReg;
				switch (IntparTypeCount)
				{
				case 0:PReg = IntLikeParam_1; break;
				case 1:PReg = IntLikeParam_2; break;
				case 2:PReg = IntLikeParam_3; break;
				case 3:PReg = IntLikeParam_4; break;
				default:
					//push on stack
					throw std::exception("not added");
					break;
				}
				IntparTypeCount++;
				GetRegData(UparReg).Contains = PReg;
				(*(RegisterID_t*)&UparReg)++;
			}
		}
	}
	{//c-code body
		const bool RetTypeIsVoid = ThisFuncData.Ret.IsVoid();
		
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
				
				//OutReg.Contains = InReg.Contains;
				throw std::exception("");
			}
			break;

			
			case InstructionSet::Call:
			{
				bool BuildFallBack = true;
				UAddress Ins = Item.Value0.AsAddress + 1;
				auto Func = State->GetMethod(Ins);
				if (Func)
				{
					auto JitFuncDat = As(Func, PointerSizeIs32Bit);
					if (!JitFuncDat.has_value())
					{
						return false;
					}
					auto& JitFuncDat_Value = JitFuncDat.value();

					PassNativePars(JitFuncDat_Value.Pars);

					BuildFallBack = false;

					NullJitCalls Call;
					Call.CPPoffset = GetIndex();
					Call.UCodeAddress = Ins;
					NullCalls.push_back(Call);
					
					FuncToLink Link;
					Link.CPPOffset = GetIndex();
					Link.OnUAddress = Call.UCodeAddress;
					LinkingData.push_back(Link);
					
					Gen_InvaildNear32Call();
					
					PopPassNativePars(JitFuncDat_Value.Pars);
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



						_Gen.sub32(GReg::RSP, SaveStackSize);//add stack frame because NativeCall kills the stack

						//pars have been passed becuase all Cpp Calls are in there own funcions
						//Rax is never used as a par

						_Gen.mov(GReg::RAX, X86_64Gen::Value64(CppV->NativeCall));
						_Gen.call(GReg::RAX);
						
						_Gen.add32(GReg::RSP, SaveStackSize);//move back stack
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
					MoveRegToNative(OutReg, ThisFuncData.Ret, GReg::RAX);
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
				RegisterID A = Item.Value0.AsRegister;
				RegisterID B = Item.Value1.AsRegister;
				auto& In1Reg = GetRegData(A);
				auto& In2Reg = GetRegData(B);

				bool BuiltCode = false;

				
				if (auto Val1 = In1Reg.Contains.Get_If<AnyInt64>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						GetRegData(RegisterID::MathOuPutRegister).Contains = AnyInt64(Val1->AsInt32 + Val2->AsInt32);
						BuiltCode = true;
					}
					else if (auto Val2 = In2Reg.Contains.Get_If<GReg>())
					{
						NewFunction2(Val2,Val1);
						BuiltCode = true;
					}
				}
				else if (auto Val1 = In1Reg.Contains.Get_If<GReg>())
				{
					if (auto Val2 = In2Reg.Contains.Get_If<AnyInt64>())
					{
						NewFunction2(Val1, Val2);
						BuiltCode = true;
					}
				}

				if (BuiltCode == false)
				{
					Optional<GReg> NewOutReg = GetRegFor(RegisterID::MathOuPutRegister);

					if (NewOutReg.has_value())
					{
						_Gen.mov32(*NewOutReg, GetAsNative(A,IntSizes::Int32));
						_Gen.add32(*NewOutReg, GetAsNative(B, IntSizes::Int32));
						GetRegData(RegisterID::MathOuPutRegister).Contains = *NewOutReg;
					}
					else
					{
						auto V = GetAsNative(A, IntSizes::Int32);
						_Gen.add32(V, GetAsNative(B, IntSizes::Int32));
						GetRegData(RegisterID::MathOuPutRegister).Contains = V;
					}
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
			case InstructionSet::IncrementStackPointer:
			{
				auto Value = Item.Value0.AsAddress;
				_Gen.sub64(GReg::RSP, Value);
			}
			break;
			case InstructionSet::DecrementStackPointer:
			{
				auto Value = Item.Value0.AsAddress;
				_Gen.add64(GReg::RSP, Value);
			}
			break;
			/*
			case InstructionSet::GetPointerOfStackSub:
			{
				auto Value = Item.Value1.Value;
				RegisterID OutReg= Item.Value0.AsRegister;
				_Gen.lea(GetRegFor(OutReg),IndrReg(GReg::RSP),0, GReg::RDI,0);
			}
			*/
			break;
			default:
				return false;
				break;
			}

			if (Item.OpCode == InstructionSet::Return)
			{
				{
					PushFuncEnd();
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
				
					PushFuncStart();

					auto Func = As(State->GetMethod(Item.UCodeAddress),PointerSizeIs32Bit).value();
					//save values on stack because who knows what BuildAddress does to the regs.
					
					
					

					PushAllParsOnStack(Func.Pars);
					
					
					_Gen.mov(IntLikeParam_1, X86_64Gen::Value64(Item.UCodeAddress));//pass the UCodeAddress

					_Gen.mov(GReg::RAX,X86_64Gen::Value64(BuildAddressPtr));
					_Gen.call(GReg::RAX);

					PopAllParsOnStack(Func.Pars);
					
					
					FuncToLink Link;
					Link.CPPOffset = GetIndex();
					Link.OnUAddress = Item.UCodeAddress;
					LinkingData.push_back(Link);

					Gen_InvaildNear32Call();//will be relinked when BuildAddressPtr builds the funcion we are trying to call 

					
					
					PushFuncEnd();
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

void X86_64JitCompiler::NewFunction2(UCodeLang::X86_64JitCompiler::GReg* Val1, UCodeLang::AnyInt64* Val2)
{
	Optional<GReg> NewOutReg = GetRegFor(RegisterID::MathOuPutRegister);

	if (NewOutReg.has_value())
	{
		_Gen.mov32(*NewOutReg, *Val1);
		_Gen.add32(*NewOutReg, Val2->AsInt32);
		GetRegData(RegisterID::MathOuPutRegister).Contains = *NewOutReg;
	}
	else
	{
		_Gen.add32(*Val1, Val2->AsInt32);
		GetRegData(RegisterID::MathOuPutRegister).Contains = *Val1;
	}
}

GReg X86_64JitCompiler::GetAsNative(RegisterID ID,IntSizes Size)
{
	auto V = GetRegData(ID);

	if (auto Val = V.Contains.Get_If<AnyInt64>())
	{
		GReg R = GetFreeRegOrMovToGetFree();

		switch (Size)
		{
		case UCodeLang::IntSizes::Int8:
			_Gen.mov(R, Val->AsInt8);
			break;
		case UCodeLang::IntSizes::Int16:
			_Gen.mov(R, Val->AsInt16);
			break;
		case UCodeLang::IntSizes::Int32:
			_Gen.mov(R, Val->AsInt32);
			break;
		case UCodeLang::IntSizes::Int64:
			_Gen.mov(R, Val->AsInt64);
			break;
		default:
			throw std::exception("bad");
			break;
		}
		return R;
	}
	else if (auto Val = V.Contains.Get_If<GReg>())
	{
		return *Val;
	}
	else
	{
		throw std::exception("bad");
	}
	
}

void X86_64JitCompiler::PassNativePars(const Vector<JitType>& Pars)
{
	{//push pars

		size_t IntparTypeCount = 0;
		RegisterID ParReg = RegisterID::StartParameterRegister;

		for (auto& Item : Pars)
		{
			if (Item.Type == JitType_t::Int32)
			{

				if ((RegisterID_t)ParReg == (RegisterID_t)RegisterID::EndParameterRegister + 1)
				{
					throw std::exception("not added");
					//move to stack
				}
				else
				{
					auto& ItemReg = GetRegData(ParReg);

					GReg PReg;
					switch (IntparTypeCount)
					{
					case 0:PReg = IntLikeParam_1; break;
					case 1:PReg = IntLikeParam_2; break;
					case 2:PReg = IntLikeParam_3; break;
					case 3:PReg = IntLikeParam_4; break;
					default:
						//push on stack
						throw std::exception("not added");
						break;
					}
					MoveRegToNative(ItemReg,Item, PReg);
					IntparTypeCount++;
				}
			}
			else
			{
				throw std::exception("not added");
			}
		}


	}
}

void X86_64JitCompiler::PopPassNativePars(const Vector<JitType>& Pars)
{

}
void X86_64JitCompiler::PushAllParsOnStack(const Vector<JitType>& Pars)
{
	size_t IntparTypeCount = 0;
	for (auto& Item : Pars)
	{
		if (Item.Type == JitType_t::Int32)
		{

			GReg PReg;
			switch (IntparTypeCount)
			{
			case 0:PReg = IntLikeParam_1; break;
			case 1:PReg = IntLikeParam_2; break;
			case 2:PReg = IntLikeParam_3; break;
			case 3:PReg = IntLikeParam_4; break;
			default:
				//push on stack
				throw std::exception("not added");
				break;
			}
			IntparTypeCount++;
			_Gen.push64(PReg);
		}
		else
		{
			throw std::exception("not added");
		}
	}
	_Gen.sub32(GReg::RSP, SaveStackSize);//so CPPinterface_Set_ReturnPtr does not break the FuncRet value

}

void X86_64JitCompiler::PopAllParsOnStack(const Vector<JitType>& Pars)
{
	_Gen.add32(GReg::RSP, SaveStackSize);
	size_t IntparTypeCount = 0;
	for (auto& Item : Pars)
	{
		if (Item.Type == JitType_t::Int32)
		{

			GReg PReg;
			switch (IntparTypeCount)
			{
			case 0:PReg = IntLikeParam_1; break;
			case 1:PReg = IntLikeParam_2; break;
			case 2:PReg = IntLikeParam_3; break;
			case 3:PReg = IntLikeParam_4; break;
			default:
				//push on stack
				throw std::exception("not added");
				break;
			}
			IntparTypeCount++;
			_Gen.pop64(PReg);
		}
		else
		{
			throw std::exception("not added");
		}
	}
}

void X86_64JitCompiler::Gen_InvaildNear32Call()
{
	size_t V = _Gen.GetIndex();
	_Gen.call(Near32(0));
	size_t CallInsSize = _Gen.GetIndex() - V;

	memset(_Gen.GetData(V), 0x33, CallInsSize);//debug break if not set
}

void X86_64JitCompiler::PushFuncEnd()
{

	{
		_Gen.pop64(GReg::RBP);
	}
}

void X86_64JitCompiler::PushFuncStart()
{
	{
		_Gen.push64(GReg::RBP);//push rbp
	}
	{
		_Gen.mov64(GReg::RBP, GReg::RSP);//mov rbp,rsp
	}

}



X86_64JitCompiler::JitType X86_64JitCompiler::AsJitType(const ReflectionTypeInfo& V, const ClassAssembly& assembly, bool PointerSizeIs32Bit)
{
	JitType R;

	if (V._Type == ReflectionTypes::Void)
	{
		R.Set(JitType_t::Void);
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
	}
	else
		if (V._Type == ReflectionTypes::uInt32 || V._Type == ReflectionTypes::sInt32)
		{
		Is32BitInt:
			R.Set(JitType_t::Int32);
		}
		else if (V._Type == ReflectionTypes::uInt64 || V._Type == ReflectionTypes::sInt64)
		{
		Is64BitInt:
			R.Set(JitType_t::Int64);
		}
		else if (V._Type == ReflectionTypes::uInt16 || V._Type == ReflectionTypes::sInt16)
		{
			R.Set(JitType_t::Int16);
		}
		else if (V._Type == ReflectionTypes::uInt8 || V._Type == ReflectionTypes::sInt8
			|| V._Type == ReflectionTypes::Char
			|| V._Type == ReflectionTypes::Bool)
		{
			R.Set(JitType_t::Int8);
		}
		else
		{
			R.SetAsBadType();
		}

	return R;
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

	
	Int32 Offset = (Int32)(ValueFunc > CallPos ?
		ValueFunc - CallPos : CallPos - ValueFunc);

	_Gen.r_call(bytes,Near32(Offset));
}
Optional<X86_64JitCompiler::JitFuncData> X86_64JitCompiler::As(const ClassMethod* Method, bool PointerSizeIs32Bit)
{
	JitType Call_Ret = AsJitType(Method->RetType, State->Get_Assembly(), PointerSizeIs32Bit);
	if (Call_Ret.IsBadType())
	{
		return {};
	}
	Vector<JitType> Pars; Pars.resize(Method->ParsType.size());
	for (size_t i = 0; i < Method->ParsType.size(); i++)
	{
		auto& Par = Method->ParsType[i];
		
		Pars[i] = Par.IsOutPar ? JitType(JitType_t::Pointer) : AsJitType(Par.Type, State->Get_Assembly(), PointerSizeIs32Bit);
		if (Pars[i].IsBadType())
		{
			return {};
		}
	}

	JitFuncData R;
	R.Ret = std::move(Call_Ret);
	R.Pars = std::move(Pars);

	return R;
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

void X86_64JitCompiler::MoveRegToNative(const RegData& Reg, const JitType& TypeInReg, X86_64Gen::GReg NativeReg)
{
	if (auto V = Reg.Contains.Get_If<AnyInt64>())
	{
		switch (TypeInReg.GetSize())
		{
		case 1:mov(NativeReg, V->AsInt8); break;
		case 2:mov(NativeReg, V->AsInt16); break;
		case 4:mov(NativeReg, V->AsInt32); break;
		case 8:mov(NativeReg, V->AsInt64); break;
		default:
			throw std::exception("not added");
			break;
		}
	}
	else if (Reg.Contains.Is<Nothing>())
	{
		return;
	}
	else if (auto Val = Reg.Contains.Get_If<GReg>())
	{
		if (*Val != NativeReg) 
		{
			switch (TypeInReg.GetSize())
			{
			case 1:_Gen.mov8(*Val, NativeReg); break;
			case 2:_Gen.mov16(*Val, NativeReg); break;
			case 4:_Gen.mov32(*Val, NativeReg); break;
			case 8:_Gen.mov64(*Val, NativeReg); break;
			default:
				throw std::exception("not added");
				break;
			}
		}
		return;
	}
	else
	{
		throw std::exception("not added");
	}
}
void X86_64JitCompiler::SynchronizeNativeRegs()
{

}

Optional<GReg> X86_64JitCompiler::GetFreeReg()
{
	Vector<GReg> Vals;
	for (auto& Item : Regs)
	{
		if (auto Val = Item.Contains.Get_If<GReg>())
		{
			Vals.push_back(*Val);
		}
	}


	for (auto& ItemVar : VarablesReg)
	{
		bool IsUseReg = false;
		
		for (auto& Item : Vals)
		{
			if (Item == ItemVar)
			{
				IsUseReg = true;
				break;
			}
		}

		if (IsUseReg == false)
		{
			return ItemVar;
		}
	}
	


	return {};
}
GReg X86_64JitCompiler::GetRegFor(RegisterID ID)
{

	return GReg::RAX;
}
GReg X86_64JitCompiler::GetFreeRegOrMovToGetFree()
{
	auto V = GetFreeReg();
	if (V.has_value())
	{
		return V.value();
	}
	return GReg::RAX;
}
UCodeLangEnd

