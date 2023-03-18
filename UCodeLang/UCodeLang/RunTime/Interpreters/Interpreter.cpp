#include "Interpreter.hpp"

UCodeLangStart



bool Interpreter::CheckIfFunctionExist(const String& FunctionName)
{
	auto address = _State->FindAddress(FunctionName);
	if (address == NullAddress)
	{
		return false;
	}
	return true;
}

void Interpreter::Get_Return(void* Output, size_t OutputSize)
{
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
	auto OldStackoffset = _CPU.Stack.StackOffSet;//for when UCode calls C++ calls UCode.
	FlushParametersIntoCPU();

	_CPU.Stack.PushStack(_CPU.ProgramCounter);
	_CPU.ProgramCounter = address;
	
	
	
	while (_CPU.Stack.StackOffSet != OldStackoffset)
	{
		Instruction& Inst = _State->GetInst(_CPU.ProgramCounter);
		Extecute(Inst);	
		_CPU.ProgramCounter++;
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

	return Return_t(State, Get_OutRegister());
}

#pragma region MyRegion
#define IntSet(Bits,signedCType,unsignedCType,signedAnyIntValue,unsignedAnyIntValue) \
case InstructionSet::Store##Bits: \
	Get_Register((RegisterID)Inst.Value0.AsRegister).Value = Inst.Value1.##signedAnyIntValue;\
	break;\
case InstructionSet::StoreRegToReg##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue\
	= Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::StoreFromPtrToReg##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value =\
		*(signedCType*)(Get_Register(Inst.Value0.AsRegister).Value.AsPtr);\
	break;\
case InstructionSet::StoreRegToPtr##Bits:\
	*(signedCType*)(Get_Register(Inst.Value1.AsRegister).Value.AsPtr) =\
		Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::Push##Bits:\
	_CPU.Stack.PushStack(Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue);\
	break;\
case InstructionSet::Pop##Bits:\
	Get_Register(Inst.Value0.AsRegister).Value = _CPU.Stack.PopStack<##signedCType>();\
	break;\
case InstructionSet::StoreRegOnStack##Bits:\
	_CPU.Stack.SetValue<##signedCType>(Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue\
		, Inst.Value1.AsUIntNative);\
	break;\
case InstructionSet::GetFromStack##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue = _CPU.Stack.GetValue<##signedCType>(Inst.Value0.AsUIntNative);\
	break;\
case InstructionSet::GetFromStackSub##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue = _CPU.Stack.GetValueSub<##signedCType>(Inst.Value0.AsUIntNative);\
	break;\
case InstructionSet::Add##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue +\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::Sub##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue -\
		Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue;\
	break;\
case InstructionSet::MultS##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue *\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::MultU##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue *\
		Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue;\
	break;\
case InstructionSet::DivS##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue *\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::DivU##Bits:\
	Get_MathOutRegister().Value.##signedAnyIntValue = Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue *\
		Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue;\
	break;\
case InstructionSet::LogicalAnd##Bits:\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue\
		&& Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue;\
break;\
case InstructionSet::Logicalor##Bits:\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue\
		|| Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue;\
break;\
case InstructionSet::LogicalNot##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue = !Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue;\
break;\
case InstructionSet::equalto##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue ==\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::notequalto##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue !=\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::lessthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue <\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::greaterthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue >\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::equal_lessthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue <=\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::equal_greaterthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue >=\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwiseAnd##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue &\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwiseOr##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue |\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwiseLeftShift##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue <<\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwiseRightShift##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue >>\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwiseXor##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.##signedAnyIntValue ^\
		Get_Register(Inst.Value1.AsRegister).Value.##signedAnyIntValue;\
	break;\
case InstructionSet::bitwise_Not##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.##unsignedAnyIntValue = ~Get_Register(Inst.Value0.AsRegister).Value.##unsignedAnyIntValue;\
	break;\




#pragma endregion

#define floatSet(Bits,CType,AnyValue) \
case InstructionSet::Store##Bits##f: \
	Get_Register((RegisterID)Inst.Value0.AsRegister).Value = Inst.Value1.##AnyValue; \
	break; \


void Interpreter::Extecute(Instruction& Inst)
{
	switch (Inst.OpCode)
	{
	case InstructionSet::Exit:
		_CPU.RetValue._Succeed = (ExitState)Inst.Value0.AsInt8;
		_CPU.Stack.StackOffSet = 0;
		break;
	case InstructionSet::Return:
		_CPU.RetValue._Succeed = (ExitState)Inst.Value0.AsInt8;
		_CPU.ProgramCounter = _CPU.Stack.PopStack<UAddress>();
		break;
	case InstructionSet::Call:
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = Inst.Value0.AsAddress;
		break;
	case InstructionSet::CallIf:
		if (Get_Register(Inst.Value1.AsRegister).Value.Asbool) {
			_CPU.Stack.PushStack(_CPU.ProgramCounter);
			_CPU.ProgramCounter = Inst.Value0.AsAddress;
		}break;
	case InstructionSet::CallReg:
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = Get_Register(Inst.Value0.AsRegister).Value.AsAddress;
		break;
	case InstructionSet::Jump:
		_CPU.ProgramCounter = Inst.Value0.AsAddress;
		break;
	case InstructionSet::Jumpif:
		if (Get_Register(Inst.Value1.AsRegister).Value.Asbool) {
			_CPU.ProgramCounter = Inst.Value0.AsAddress;
		}break;
	case InstructionSet::JumpReg:
		_CPU.ProgramCounter = Get_Register(Inst.Value0.AsRegister).Value.AsAddress;
		break;
	case InstructionSet::DoNothing:break;
	
	IntSet(8,Int8,UInt8, AsInt8, AsUInt8)
	IntSet(16,Int16,UInt16, AsInt16, AsUInt16)
	IntSet(32,Int32,UInt32, AsInt32, AsUInt32)
	IntSet(64,Int64,UInt64, AsInt64, AsUInt64)
	
	floatSet(32,float32,Asfloat32)
	floatSet(64, float64,Asfloat64)

	#pragma region Cpp func Set
	case InstructionSet::GetPointerOfStack:
		Get_Register(Inst.Value0.AsRegister).Value = _CPU.Stack.GetTopOfStackWithoffset(Inst.Value1.AsUIntNative);
		break;
	case InstructionSet::GetPointerOfStackSub:
		Get_Register(Inst.Value0.AsRegister).Value = _CPU.Stack.GetTopOfStackWithoffsetSub(Inst.Value1.AsUIntNative);
		break;
	case InstructionSet::IncrementStackPointer:
		_CPU.Stack.IncrementStack(Get_Register(Inst.Value0.AsRegister).Value.AsUIntNative);
		break;
	case InstructionSet::DecrementStackPointer:
		_CPU.Stack.DecrementStack(Get_Register(Inst.Value0.AsRegister).Value.AsUIntNative);
		break;
	case InstructionSet::GetPointerOfStaticMem:
		Get_Register(Inst.Value0.AsRegister).Value = Get_StaticMemPtr();
		break;
	case InstructionSet::Malloc:
		Get_Register(Inst.Value1.AsRegister).Value = 
			         Malloc(Get_Register(Inst.Value0.AsRegister).Value.AsUIntNative);
		break;
	case InstructionSet::Free:
			Free(Get_Register(Inst.Value0.AsRegister).Value.AsPtr);
		break;
	case InstructionSet::MemCopy:
			MemCopy(Get_Register(Inst.Value1.AsRegister).Value.AsPtr
				   ,Get_Register(Inst.Value0.AsRegister).Value.AsPtr
			       ,Get_InRegister().Value.AsUIntNative);
		break;
	case InstructionSet::Calloc:
		Get_Register(Inst.Value1.AsRegister).Value =
			Calloc(Get_Register(Inst.Value0.AsRegister).Value.AsUIntNative);
		break;
	case InstructionSet::ReAlloc:
		Get_OutRegister().Value =
			Realloc(Get_Register(Inst.Value0.AsRegister).Value.AsPtr
				   ,Get_Register(Inst.Value1.AsRegister).Value.AsUIntNative);
		break;
	case InstructionSet::Log: 
	{
		const char* Ptr = (const char*)Get_Register(Inst.Value0.AsRegister).Value.AsPtr;
		Log(Ptr);
	}
		break;
		//Linking
	case InstructionSet::CppCallNamed:
	{
		auto Ptr = (UIntNative)Get_StaticMemPtr();
		auto NewPtr = Ptr + Inst.Value0.AsUIntNative;
		_CPU.Stack.PushStack(Inst.Value1.AsUIntNative);
		String Str((const char*)NewPtr);
		auto Cpp = _State->FindCPPCall(Str);
		if (Cpp)
		{
			auto CppV = *Cpp;
			auto& inter = *(InterpreterCPPinterface*)&_CPPHelper;
			inter = InterpreterCPPinterface(this);
			(*CppV)(inter);
		}
		else
		{
			String CantFindMsg = "Cant find cpp Call named " + Str;
			_State->Log(CantFindMsg.c_str());

			throw std::exception("bad cpp call.");//Cant find CPPCall.
			_CPU.RetValue._Succeed = ExitState::Failure;
			_CPU.Stack.StackOffSet = 0;
		}
	}break;
	case InstructionSet::CPPCall:
	{
		auto CppV = (RunTimeLib::CPPCallBack)Inst.Value0.AsPtr;
		auto& inter = *(InterpreterCPPinterface*)&_CPPHelper;
		inter = InterpreterCPPinterface(this);

		throw std::exception("unknown instruction");

		//CppV(inter);
	}break;
	case InstructionSet::LoadFuncPtr:
	{
		Get_Register((RegisterID)Inst.Value1.AsRegister).Value = Inst.Value0.AsAddress; \
	}
	break;
	#pragma endregion
	break;
	default:
		#if CompliationTypeSafety
		throw std::exception("unknown instruction");
		#endif // DEBUG
		_CPU.RetValue._Succeed = ExitState::Failure;
		_CPU.Stack.StackOffSet = 0;
		break;
	}
}

UCodeLangEnd


