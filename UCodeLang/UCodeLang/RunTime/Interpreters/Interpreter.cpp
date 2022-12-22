#include "Interpreter.hpp"

UCodeLangStart


Interpreter::Return_t Interpreter::ThisCall(UAddress This, const String& FunctionName, parameters Pars)
{
	auto address = _State->FindAddress(FunctionName);
	if (address == NullAddress)
	{
		return Return_t(RetState::Error_Function_doesnt_exist);
	}

	return ThisCall(This,address, Pars);
}
Interpreter::Return_t Interpreter::ThisCall(UAddress This, UAddress address, parameters Pars)
{
	Get_ThisRegister().Value = This;
	return Call(address, Pars);
}

Interpreter::Return_t Interpreter::Call(const String& FunctionName, parameters Pars)
{
	auto address = _State->FindAddress(FunctionName);
	if (address == NullAddress)
	{
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	
	return Call(address,Pars);
}
Interpreter::Return_t Interpreter::Call(UAddress address, parameters Pars)
{
	auto OldStackoffset = _CPU.Stack.StackOffSet;//for when UCode calls C++ calls UCode.

	_CPU.Stack.PushParameters(Pars);
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

#define IntSet(Bits,signedCType,unsignedCType,signedAnyIntValue,unsignedAnyIntValue) \
case InstructionSet::Store##Bits: \
	Get_Register((RegisterID)Inst.Value0.AsRegister).Value = Inst.Value1.AsInt8;\
	break;\
case InstructionSet::StoreFromPtrToReg##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value =\
		*(Int8*)(Get_Register(Inst.Value0.AsRegister).Value.AsPtr);\
	break;\
case InstructionSet::StoreRegToPtr##Bits:\
	*(Int8*)(Get_Register(Inst.Value1.AsRegister).Value.AsPtr) =\
		Get_Register(Inst.Value0.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::Push##Bits:\
	_CPU.Stack.PushStack(Get_Register(Inst.Value0.AsRegister).Value.AsInt8);\
	break;\
case InstructionSet::Pop##Bits:\
	Get_Register(Inst.Value0.AsRegister).Value = _CPU.Stack.PopStack<Int8>();\
	break;\
case InstructionSet::StoreRegOnStack##Bits:\
	_CPU.Stack.SetValue<Int8>(Get_Register(Inst.Value0.AsRegister).Value.AsInt8\
		, Inst.Value1.AsUIntNative);\
	break;\
case InstructionSet::GetFromStack##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.AsInt8 = _CPU.Stack.GetValue<Int8>(Inst.Value0.AsUIntNative);\
	break;\
case InstructionSet::GetFromStackSub##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.AsInt8 = _CPU.Stack.GetValueSub<Int8>(Inst.Value0.AsUIntNative);\
	break;\
case InstructionSet::Add##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 +\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::Sub##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsUInt8 +\
		Get_Register(Inst.Value1.AsRegister).Value.AsUInt8;\
	break;\
case InstructionSet::MultS##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 *\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::MultU##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsUInt8 *\
		Get_Register(Inst.Value1.AsRegister).Value.AsUInt8;\
	break;\
case InstructionSet::DivS##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 *\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::DivU##Bits:\
	Get_MathOutRegister().Value.AsInt8 = Get_Register(Inst.Value0.AsRegister).Value.AsUInt8 *\
		Get_Register(Inst.Value1.AsRegister).Value.AsUInt8;\
	break;\
case InstructionSet::LogicalAnd##Bits:\
{\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Value0.AsRegister).Value.AsUInt8\
		&& Get_Register(Inst.Value1.AsRegister).Value.AsUInt8;\
}break;\
case InstructionSet::Logicalor##Bits:\
{\
	Get_BoolRegister().Value =\
		Get_Register(Inst.Value0.AsRegister).Value.AsUInt8\
		|| Get_Register(Inst.Value1.AsRegister).Value.AsUInt8;\
}break;\
case InstructionSet::LogicalNot##Bits:\
{\
	Get_Register(Inst.Value1.AsRegister).Value.AsUInt8 = !Get_Register(Inst.Value0.AsRegister).Value.AsUInt8;\
}break;\
case InstructionSet::equalto##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 ==\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::notequalto##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 !=\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::lessthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 <\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::greaterthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 >\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::equal_lessthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 <=\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::equal_greaterthan##Bits:\
	Get_BoolRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 >=\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwiseAnd##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 &\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwiseOr##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 |\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwiseLeftShift##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 <<\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwiseRightShift##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 >>\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwiseXor##Bits:\
	Get_bitwiseRegister().Value = Get_Register(Inst.Value0.AsRegister).Value.AsInt8 ^\
		Get_Register(Inst.Value1.AsRegister).Value.AsInt8;\
	break;\
case InstructionSet::bitwise_Not##Bits:\
	Get_Register(Inst.Value1.AsRegister).Value.AsUInt8 = ~Get_Register(Inst.Value0.AsRegister).Value.AsUInt8;\
	break;\


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
		if (Get_BoolRegister().Value.Asbool) {
			_CPU.Stack.PushStack(_CPU.ProgramCounter);
			_CPU.ProgramCounter = Inst.Value0.AsAddress;
		}break;
	case InstructionSet::CallPtr:
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = *(UAddress*)Inst.Value0.AsAddress;
		break;
	case InstructionSet::Jump:
		_CPU.ProgramCounter = Inst.Value0.AsAddress;
		break;
	case InstructionSet::Jumpif:
		if (Get_BoolRegister().Value.Asbool) {
			_CPU.ProgramCounter = Inst.Value0.AsAddress;
		}break;
	case InstructionSet::JumpPtr:
		_CPU.ProgramCounter = *(UAddress*)Inst.Value0.AsAddress;
		break;
	case InstructionSet::DoNothing:break;
		
	IntSet(8,Int8,UInt8, AsInt8, AsUInt8)
	IntSet(16,Int8,UInt16, AsInt8, AsUInt16)
	IntSet(32,Int8,UInt32, AsInt8, AsUInt32)
	IntSet(64,Int8,UInt64, AsInt8, AsUInt64)
	
	#pragma region Cpp func Set
	case InstructionSet::GetPointerOfStack:
		Get_Register(Inst.Value0.AsRegister).Value = _CPU.Stack.GetTopOfStack();
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
			CppV(inter);
		}
		else
		{
			_CPU.RetValue._Succeed = ExitState::Failure;
			_CPU.Stack.StackOffSet = 0;
		}
	}break;
	case InstructionSet::CPPCall:
	{
		auto CppV = (RunTimeLib::CPPCallBack)Inst.Value0.AsPtr;
		auto& inter = *(InterpreterCPPinterface*)&_CPPHelper;
		inter = InterpreterCPPinterface(this);
		CppV(inter);
	}break;
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


