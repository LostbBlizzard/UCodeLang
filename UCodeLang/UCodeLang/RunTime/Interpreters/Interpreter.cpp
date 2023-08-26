#include "Interpreter.hpp"

UCodeLangStart



bool Interpreter::CheckIfFunctionExist(const String& FunctionName)
{
	auto address = _State->FindAddress(FunctionName);
	return address.has_value();
}

void Interpreter::Get_Return(void* Output, size_t OutputSize)
{
#if UCodeLangDebug
	GotRetValue = true;
#endif
	if (OutputSize <= sizeof(Register))
	{
		MemCopy(Output, &Get_OutRegister().Value, OutputSize);
	}
	else
	{
		MemCopy(Output, *(void**)&Get_OutRegister().Value, OutputSize);
	}
}

void Interpreter::FlushParametersIntoCPU()
{
	RegisterID ParRegister = RegisterID::StartParameterRegister;

	auto State = _Parameters.StartLoop();

	while (_Parameters.Next(State))
	{
		auto Data = _Parameters.GetLoopData(State);

		if (Data.DataSize <= sizeof(Register) && ParRegister < RegisterID::EndParameterRegister)
			//the if must be the same for UCodeBackEnd CallFunc/Func_Parameter
		{
			void* RegPtr = &Get_Register(ParRegister).Value;

			MemCopy(RegPtr, (const PtrType)Data.Pointer, Data.DataSize);


			(*(RegisterID_t*)&ParRegister)++;
		}
		else
		{
			_CPU.Stack.PushBytes(Data.Pointer, Data.DataSize);
		}
	}

	_Parameters.Clear();
}

Interpreter::Return_t Interpreter::Call(const String& FunctionName)
{
	auto address = _State->FindAddress(FunctionName);
	if (!address.has_value())
	{
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	
	return Call(address.value());
}
Interpreter::Return_t Interpreter::Call(UAddress address)
{
	#if UCodeLangDebug
	{
		
		if (CalledFuncBefor)
		{
			UCodeLangAssert(GotRetValue == true);//you did not call Get_Return() on last call and it last call was not void
		}
		CalledFuncBefor = true;
		GotRetValue = false;
		auto method = Get_State()->GetMethod(address);
		
		{
			auto funcname = Get_State()->GetName(address);
			if (funcname == StaticVariablesInitializeFunc
				|| funcname == StaticVariablesUnLoadFunc
				|| funcname == ThreadVariablesInitializeFunc
				|| funcname == ThreadVariablesUnLoadFunc)
			{
				UCodeLangAssert(_Parameters.GetParameterCount() == 0);//incorrect parameter count
			}
			else
			{
				UCodeLangAssert(method != nullptr);//Must be Method
				if (method)
				{
					UCodeLangAssert(_Parameters.GetParameterCount() == method->ParsType.size());//incorrect parameter count

					auto State = _Parameters.StartLoop();
					size_t i = 0;
					while (_Parameters.Next(State))
					{
						auto Data = _Parameters.GetLoopData(State);
						auto ParSize = Get_State()->Get_Assembly().GetSize(method->ParsType[i], sizeof(void*) == 4).value_or(0);
						UCodeLangAssert(Data.DataSize == ParSize);//incorrect parameter size

						i++;
					}
					GotRetValue = method->RetType._Type == ReflectionTypes::Void;
				}
			}
		}
		
	}
	#endif
	auto OldStackPrePars = _CPU.Stack.StackOffSet;
	FlushParametersIntoCPU();

	auto OldStackoffset = _CPU.Stack.StackOffSet;
	_CPU.Stack.PushStack(_CPU.ProgramCounter);
	_CPU.ProgramCounter = address;
	
	while (_CPU.Stack.StackOffSet != OldStackoffset)
	{
		Instruction& Inst = _State->GetInst(_CPU.ProgramCounter);
		Extecute(Inst);	
		_CPU.ProgramCounter++;

		#if UCodeLangDebug
		if (_CPU.ProgramCounter == 7)
		{
			int a = 0;
		}
		#endif // DEBUG
	}

	RetState State;
	switch (_CPU.RetValue._Succeed)
	{

	case ExitState::Success:
		State = RetState::Success;
		break;
	case ExitState::Failure:
		State = RetState::Error;
		break;
	default:
		State = RetState::Null;
		break;
	}

	_CPU.Stack.StackOffSet = OldStackPrePars;

	return Return_t(State, Get_OutRegister());
}


#if UCodeLangGNUC
#define HasLabelAsValues 1
#else
#define HasLabelAsValues 0
#endif // 


#if HasLabelAsValues && !UCodeLangDebug
#define UseJumpTable 1
#else
#define UseJumpTable 0
#endif // HasLabelAsValues && 



#if UseJumpTable
#define InsCase(OpCode) Ins_##OpCode 
#else
#define InsCase(OpCode) case InstructionSet:: OpCode 
#endif

#if UseJumpTable
#define InsBreak()
#else
#define InsBreak() break;
#endif

#pragma region MyRegion
#define IntSet(Bits,signedCType,unsignedCType,signedAnyIntValue,unsignedAnyIntValue) \
InsCase(StoreRegToReg##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue\
	= Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(StoreFromPtrToReg##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value =\
		*(signedCType*)(Get_Register(Inst.Op_TwoReg.A).Value.AsPtr);\
	 InsBreak();\
InsCase(StoreRegToPtr##Bits):\
	*(signedCType*)(Get_Register(Inst.Op_TwoReg.B).Value.AsPtr) =\
		Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(Push##Bits):\
	_CPU.Stack.PushStack(Get_Register(Inst.Op_OneReg.A).Value. signedAnyIntValue);\
	 InsBreak();\
InsCase(Pop##Bits):\
	Get_Register(Inst.Op_OneReg.A).Value = _CPU.Stack.PopStack< signedCType>();\
	 InsBreak();\
InsCase(StoreRegOnStack##Bits):\
	_CPU.Stack.SetValue< signedCType>(Get_Register(Inst.Op_RegUInt16.A).Value. signedAnyIntValue\
		, Inst.Op_RegUInt16.B);\
	 InsBreak();\
InsCase(StoreRegOnStackSub##Bits):\
	_CPU.Stack.SetValueSub< signedCType>(Get_Register(Inst.Op_RegUInt16.A).Value. signedAnyIntValue\
		, Inst.Op_RegUInt16.B);\
	 InsBreak();\
InsCase(GetFromStack##Bits):\
	Get_Register(Inst.Op_RegUInt16.A).Value. signedAnyIntValue = _CPU.Stack.GetValue< signedCType>(Inst.Op_RegUInt16.B);\
	 InsBreak();\
InsCase(GetFromStackSub##Bits):\
	Get_Register(Inst.Op_RegUInt16.A).Value. signedAnyIntValue = _CPU.Stack.GetValueSub< signedCType>(Inst.Op_RegUInt16.B);\
	 InsBreak();\
InsCase(Add##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue +\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(Sub##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue -\
		Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue;\
	 InsBreak();\
InsCase(MultS##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue *\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(MultU##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue *\
		Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue;\
	 InsBreak();\
InsCase(DivS##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue / \
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(DivU##Bits):\
	Get_MathOutRegister().Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue / \
		Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue;\
	 InsBreak();\
InsCase(LogicalAnd##Bits):\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue\
		&& Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue;\
 InsBreak();\
InsCase(Logicalor##Bits):\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue\
		|| Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue;\
 InsBreak();\
InsCase(LogicalNot##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue = !Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue;\
 InsBreak();\
InsCase(equalto##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue ==\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(notequalto##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue !=\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(lessthan##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue <\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(greaterthan##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue >\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(equal_lessthan##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue <=\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(equal_greaterthan##Bits):\
	Get_BoolRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue >=\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwiseAnd##Bits):\
	Get_BitwiseRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue &\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwiseOr##Bits):\
	Get_BitwiseRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue |\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwiseLeftShift##Bits):\
	Get_BitwiseRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue <<\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwiseRightShift##Bits):\
	Get_BitwiseRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue >>\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwiseXor##Bits):\
	Get_BitwiseRegister().Value = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue ^\
		Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue;\
	 InsBreak();\
InsCase(bitwise_Not##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue = ~Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue;\
	 InsBreak();\
InsCase(UInt##Bits##To##SInt##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value. signedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. unsignedAnyIntValue;\
	 InsBreak();\
InsCase(SInt##Bits##ToUInt##Bits):\
	Get_Register(Inst.Op_TwoReg.B).Value. unsignedAnyIntValue = Get_Register(Inst.Op_TwoReg.A).Value. signedAnyIntValue;\
	 InsBreak();\



#pragma endregion

#define floatSet(Bits,CType,AnyValue) \



void Interpreter::Extecute(Instruction& Inst)
{
	#if UseJumpTable
	static const void* InsJumpTable[] = {&& Ins_Exit,&& Ins_Return,&& Ins_Call};

	constexpr size_t JumpTableSize = sizeof(InsJumpTable) / sizeof(InsJumpTable[0]);
	static_assert(JumpTableSize == (InstructionSet_t)InstructionSet::MAXVALUE,"Jump Table does not contain all Instructions");
	
	goto (*InsJumpTable[(InstructionSet_t)Inst.OpCode]);
	#endif
	
	#if !UseJumpTable
	switch (Inst.OpCode)
	{
	#endif
	InsCase(Exit):
		_CPU.RetValue._Succeed = (ExitState)Inst.Op_ValUInt8.A;
		_CPU.Stack.StackOffSet = 0;
		 InsBreak();
	InsCase(Return):
		_CPU.RetValue._Succeed = (ExitState)Inst.Op_ValUInt8.A;
		_CPU.ProgramCounter = _CPU.Stack.PopStack<UAddress>();
		 InsBreak();
	InsCase(Call):
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = Inst.Op_ValUInt16.A;
		 InsBreak();
	InsCase(CallIf):
		if (Get_Register(Inst.Op_RegUInt16.A).Value.Asbool) {
			_CPU.Stack.PushStack(_CPU.ProgramCounter);
			_CPU.ProgramCounter = Inst.Op_RegUInt16.B;
		} InsBreak();
	InsCase(CallReg):
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = Get_Register(Inst.Op_OneReg.A).Value.AsAddress;
		 InsBreak();
	InsCase(Jump):
		_CPU.ProgramCounter = Inst.Op_ValUInt16.A;
		 InsBreak();
	InsCase(Jumpif):
		if (Get_Register(Inst.Op_RegUInt16.A).Value.Asbool) {
			_CPU.ProgramCounter = Inst.Op_RegUInt16.B;
		} InsBreak();
	InsCase(JumpReg):
		_CPU.ProgramCounter = Get_Register(Inst.Op_OneReg.A).Value.AsAddress;
		 InsBreak();
	InsCase(DoNothing): InsBreak();
	
	

	IntSet(8,Int8,UInt8, AsInt8, AsUInt8)
	InsCase(Store8): 
		Get_Register((RegisterID)Inst.Op_RegUInt8.A).Value = Inst.Op_RegUInt8.B;
		 InsBreak(); 
	
	IntSet(16,Int16,UInt16, AsInt16, AsUInt16)
	InsCase(Store16):
		Get_Register((RegisterID)Inst.Op_RegUInt16.A).Value = Inst.Op_RegUInt16.B;
		 InsBreak();
	
	IntSet(32,Int32,UInt32, AsInt32, AsUInt32)
	InsCase(Store32v1):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[0] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Store32v2):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[1] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();

	IntSet(64,Int64,UInt64, AsInt64, AsUInt64)
	InsCase(Store64v1):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[0] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Store64v2):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[1] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Store64v3):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[2] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Store64v4):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[3] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();

	floatSet(32,float32,Asfloat32)
	
	InsCase(Storef64v1):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[0] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Storef64v2):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[1] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Storef64v3):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[2] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	InsCase(Storef64v4):
	{
		auto& _Register = Get_Register((RegisterID)Inst.Op_RegUInt16.A);
		((UInt16*)&_Register)[3] = Inst.Op_RegUInt16.B;
	}
	 InsBreak();
	floatSet(64, float64,Asfloat64)


	//Cast Set
	InsCase(Int8ToInt16):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int16)Get_Register(Inst.Op_TwoReg.A).Value.AsInt8;
		 InsBreak();
	InsCase(Int16ToInt32):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int32)Get_Register(Inst.Op_TwoReg.A).Value.AsInt16;
		 InsBreak();
	InsCase(Int32ToInt64):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int64)Get_Register(Inst.Op_TwoReg.A).Value.AsInt32;
		 InsBreak();

	InsCase(Int64ToInt32):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int32)Get_Register(Inst.Op_TwoReg.A).Value.AsInt64;
		 InsBreak();

	InsCase(Int32ToInt16):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int16)Get_Register(Inst.Op_TwoReg.A).Value.AsInt32;
		 InsBreak();

	InsCase(Int16ToInt8):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int8)Get_Register(Inst.Op_TwoReg.A).Value.AsInt16;
		 InsBreak();

	InsCase(float32ToInt32):
		Get_Register(Inst.Op_TwoReg.B).Value = (float32)Get_Register(Inst.Op_TwoReg.A).Value.AsInt32;
		 InsBreak();

	InsCase(float64ToInt64):
		Get_Register(Inst.Op_TwoReg.B).Value = (float64)Get_Register(Inst.Op_TwoReg.A).Value.AsInt64;
		 InsBreak();

	InsCase(Int32Tofloat32):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int32)Get_Register(Inst.Op_TwoReg.A).Value.Asfloat32;
		 InsBreak();

	InsCase(Int64Tofloat64):
		Get_Register(Inst.Op_TwoReg.B).Value = (Int64)Get_Register(Inst.Op_TwoReg.A).Value.Asfloat64;
		 InsBreak();

	InsCase(GetPointerOfStack):
		Get_Register(Inst.Op_RegUInt16.A).Value = _CPU.Stack.GetTopOfStackWithoffset(Inst.Op_RegUInt16.B);
		 InsBreak();
	InsCase(GetPointerOfStackSub):
		Get_Register(Inst.Op_RegUInt16.A).Value = _CPU.Stack.GetTopOfStackWithoffsetSub(Inst.Op_RegUInt16.B);
		 InsBreak();
	InsCase(IncrementStackPointer):
		_CPU.Stack.IncrementStack(Get_Register(Inst.Op_TwoReg.A).Value.AsUIntNative);
		 InsBreak();
	InsCase(DecrementStackPointer):
		_CPU.Stack.DecrementStack(Get_Register(Inst.Op_TwoReg.A).Value.AsUIntNative);
		 InsBreak();
	InsCase(GetPointerOfStaticMem):
		Get_Register(Inst.Op_RegUInt16.A).Value = (uintptr_t)Get_StaticMemPtr() + Inst.Op_RegUInt16.B;
		 InsBreak();
	InsCase(GetPointerOfThreadMem):
		Get_Register(Inst.Op_RegUInt16.A).Value = (uintptr_t)GetThreadPtr() + Inst.Op_RegUInt16.B;
		 InsBreak();
	InsCase(Malloc):
		Get_Register(Inst.Op_TwoReg.B).Value = 
			         Malloc(Get_Register(Inst.Op_TwoReg.A).Value.AsUIntNative);
		 InsBreak();
	InsCase(Free):
			Free(Get_Register(Inst.Op_TwoReg.A).Value.AsPtr);
		 InsBreak();
	InsCase(MemCopy):
			MemCopy(Get_Register(Inst.Op_TwoReg.B).Value.AsPtr
				   ,Get_Register(Inst.Op_TwoReg.A).Value.AsPtr
			       ,Get_InputRegister().Value.AsUIntNative);
		 InsBreak();
	InsCase(Calloc):
		Get_Register(Inst.Op_TwoReg.B).Value =
			Calloc(Get_Register(Inst.Op_TwoReg.A).Value.AsUIntNative);
		 InsBreak();
	InsCase(ReAlloc):
		Get_OutRegister().Value =
			Realloc(Get_Register(Inst.Op_TwoReg.A).Value.AsPtr
				   ,Get_Register(Inst.Op_TwoReg.B).Value.AsUIntNative);
		 InsBreak();
	InsCase(PointerMemberRead8):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		Get_Register(RegOut).Value = *(Int8*)(Offset + Get_Register(Ptr).Value.AsAddress);
	}	
	 InsBreak();
	InsCase(PointerMemberRead16):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		Get_Register(RegOut).Value = *(Int16*)(Offset + Get_Register(Ptr).Value.AsAddress);
	}
	 InsBreak();
	InsCase(PointerMemberRead32):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		Get_Register(RegOut).Value = *(Int32*)(Offset + Get_Register(Ptr).Value.AsAddress);
	}
	 InsBreak();
	InsCase(PointerMemberRead64):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		Get_Register(RegOut).Value = *(Int64*)(Offset + Get_Register(Ptr).Value.AsAddress);
	}
	 InsBreak();

	InsCase(PointerMemberLoad64):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		*(Int64*)(Offset + Get_Register(Ptr).Value.AsAddress) = Get_Register(RegOut).Value.AsInt64;
	}
	 InsBreak();
	InsCase(PointerMemberLoad32):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		*(Int32*)(Offset + Get_Register(Ptr).Value.AsAddress) = Get_Register(RegOut).Value.AsInt32;
	}
	 InsBreak();
	InsCase(PointerMemberLoad16):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		*(Int16*)(Offset + Get_Register(Ptr).Value.AsAddress) = Get_Register(RegOut).Value.AsInt16;
	}
	 InsBreak();
	InsCase(PointerMemberLoad8):
	{
		RegisterID Ptr = Inst.Op_TwoRegInt8.A;
		RegisterID RegOut = Inst.Op_TwoRegInt8.B;
		size_t Offset = Inst.Op_TwoRegInt8.C;

		*(Int8*)(Offset + Get_Register(Ptr).Value.AsAddress) = Get_Register(RegOut).Value.AsInt8;
	}
	 InsBreak();
	//Linking
	InsCase(CppCallNamed):
	{
		auto Ptr = (UIntNative)Get_StaticMemPtr();
		auto NewPtr = Ptr + Inst.Op_ValUInt16.A;

		String Str((const char*)NewPtr);
		auto Cpp = _State->FindCPPCall(Str);
		if (Cpp)
		{
			auto CppV = *Cpp;
			auto& inter = *(InterpreterCPPinterface*)&_CPPHelper;
			inter = InterpreterCPPinterface(this);
			(*CppV->InterpreterCall)(inter);
		}
		else
		{
			#if UCodeLangDebug
			String CantFindMsg = "Cant find cpp Call named " + Str;
			_State->Log(CantFindMsg.c_str());

			UCodeLangThrowException("bad cpp call.");//Cant find CPPCall.
			#endif // DEBUG

			

			UCodeLangUnreachable();//not added  instruction?
		}
	} InsBreak();
	InsCase(CPPCall):
	{
		UCodeLangUnreachable();//not added  instruction?
	}
	 InsBreak();
	InsCase(LoadFuncPtr):
	{
		Get_Register((RegisterID)Inst.Op_RegUInt16.B).Value = Inst.Op_RegUInt16.A; 
	}
	 InsBreak();
	InsCase(Debug_FuncStart):
	{
		Get_State()->Get_DebugContext().TryFuncStart(*Get_State(), { this,DebugContext::Type::Interpreter });
	}
	 InsBreak();
	InsCase(Debug_FuncEnd):
	{
		Get_State()->Get_DebugContext().TryFuncEnd(*Get_State(), {this,DebugContext::Type::Interpreter});
	}
	 InsBreak();
	InsCase(Debug_LineEnter):
	{
		Get_State()->Get_DebugContext().TryFuncOnLine(*Get_State(), { this,DebugContext::Type::Interpreter });
	}
	 InsBreak();
	
	//Await Set
	InsCase(Await_NewTask):
	{
		PackagedTask* task = new (Malloc(sizeof(PackagedTask))) PackagedTask();
		task->Set_Func(Get_Register((RegisterID)Inst.Op_OneReg.A).Value.AsAddress);
		Get_Register((RegisterID)Inst.Op_TwoReg.B).Value = task;
	}
	 InsBreak();
	InsCase(Await_PassPar):
	{
		PackagedTask* task = (PackagedTask*)Get_Register((RegisterID)Inst.Op_OneReg.A).Value.AsPtr;
		void* Ptr = Get_Register((RegisterID)Inst.Op_TwoReg.B).Value.AsPtr;
		size_t Size = Get_InputRegister().Value.AsUIntNative;
		task->PushParameter(Ptr,Size);
	}
	 InsBreak();
	InsCase(Await_Run):
	{
		auto& InReg = Get_Register((RegisterID)Inst.Op_OneReg.A);

		PackagedTask* task = (PackagedTask*)InReg.Value.AsPtr;

		auto awaittask = Get_State()->AwaitNewTask(std::move(*task));

		task->~PackagedTask();
		Free(task);

		InReg.Value.AsPtr = awaittask;
	}
	 InsBreak();
	InsCase(Await_IsDone):
	{
		RunTimeLangState::AwaitedTask task = (RunTimeLangState::AwaitedTask)Get_Register((RegisterID)Inst.Op_OneReg.A).Value.AsPtr;
		Get_Register((RegisterID)Inst.Op_OneReg.A).Value.Asbool = Get_State()->AwaitIsDone(task);
	}
	 InsBreak();
	InsCase(Await_GetValue):
	{
		RunTimeLangState::AwaitedTask task = (RunTimeLangState::AwaitedTask)Get_Register((RegisterID)Inst.Op_OneReg.A).Value.AsPtr;

		size_t valuesize = Get_InputRegister().Value.AsUIntNative;
		void* ptr = _CPU.Stack.GetTopOfStack();
		Get_State()->AwaitGetValue(task,ptr);

		if (valuesize > sizeof(Register))
		{
			Get_OutRegister().Value.AsPtr = ptr;
		}
		else
		{
			void* mem = &Get_OutRegister().Value;
			memcpy(mem,ptr,valuesize);
		}
	}
	InsBreak();
	InsCase(Await_FreeTask):
	{
		RunTimeLangState::AwaitedTask task = (RunTimeLangState::AwaitedTask)Get_Register((RegisterID)Inst.Op_OneReg.A).Value.AsPtr;
		Get_State()->AwaitFreeTask(task);
	}
	InsBreak();
	
#if !UseJumpTable
	default:
		UCodeLangUnreachable();
		break;
	}
#endif
}


UCodeLangEnd


