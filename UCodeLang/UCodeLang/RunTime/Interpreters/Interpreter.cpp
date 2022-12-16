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

#define NumMathSet(Bits) // for numbers


void Interpreter::Extecute(Instruction& Inst)
{
	switch (Inst.OpCode)
	{
	case InstructionSet::Exit:
		_CPU.RetValue._Succeed = (ExitState)Inst.Value0_AsInt8;
		_CPU.Stack.StackOffSet = 0;
		break;
	case InstructionSet::Return:
		_CPU.RetValue._Succeed = (ExitState)Inst.Value0_AsInt8;
		_CPU.ProgramCounter = _CPU.Stack.PopStack<UAddress>();
		break;
	case InstructionSet::Call:
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = Inst.Value0_AsAddress;
		break;
	case InstructionSet::CallIf:
		if (Get_BoolRegister().Value.Asbool) {
			_CPU.Stack.PushStack(_CPU.ProgramCounter);
			_CPU.ProgramCounter = Inst.Value0_AsAddress;
		}break;
	case InstructionSet::CallPtr:
		_CPU.Stack.PushStack(_CPU.ProgramCounter);
		_CPU.ProgramCounter = *(UAddress*)Inst.Value0_AsAddress;
		break;
	case InstructionSet::Jump:
		_CPU.ProgramCounter = Inst.Value0_AsAddress;
		break;
	case InstructionSet::Jumpif:
		if (Get_BoolRegister().Value.Asbool) {
			_CPU.ProgramCounter = Inst.Value0_AsAddress;
		}break;
	case InstructionSet::JumpPtr:
		_CPU.ProgramCounter = *(UAddress*)Inst.Value0_AsAddress;
		break;
	case InstructionSet::DoNothing:break;
		
	#pragma region Set 8Bits
	case InstructionSet::Store8:
		Get_Register((RegisterID)Inst.Value0_AsInt8).Value = Inst.Value1_AsInt8;
		break;
	case InstructionSet::Store8FromPtrToReg:
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value =
			*(Int8*)(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsPtr);
		break;
	case InstructionSet::Store8RegToPtr:
			*(Int8*)(Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsPtr) =
			 Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::Push8:
		_CPU.Stack.PushStack(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8);
		break;
	case InstructionSet::Pop8:
		Get_Register((RegisterID)Inst.Value0_AsInt8).Value = _CPU.Stack.PopStack<Int8>();
		break;
	case InstructionSet::Store8RegOnStack:
		_CPU.Stack.SetValue<Int8>(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8
			                     ,Inst.Value1_AsUIntNative);
		break;
	case InstructionSet::Get8FromStack:
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8 = _CPU.Stack.GetValue<Int8>(Inst.Value0_AsUIntNative);
		break;
	case InstructionSet::Get8FromStackSub:
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8 = _CPU.Stack.GetValueSub<Int8>(Inst.Value0_AsUIntNative);
		break;

	case InstructionSet::AddS8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 +
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::AddU8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8 +
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
		break;
	case InstructionSet::SubS8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 -
		                     	  Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::SubU8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8 -
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
		break;
	case InstructionSet::MultS8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 *
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::MultU8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8 *
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
		break;
	case InstructionSet::DivS8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 *
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::DivU8:
		Get_MathOutRegister().Value.AsInt8 = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8 *
			                      Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
		break;
	case InstructionSet::LogicalAnd8:
	{
		Get_BoolRegister().Value =
			Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8
			&& Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
	}break;
	case InstructionSet::Logicalor8:
	{
		Get_BoolRegister().Value =
			Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8
			|| Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8;
	}break;
	case InstructionSet::LogicalNot8:
	{	
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8 = !Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8;
	}break;
	case InstructionSet::equalto8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 ==
			          Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::notequalto8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 !=
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::lessthan8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 <
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::greaterthan8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 >
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::equal_lessthan8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 <=
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::equal_greaterthan8:
		Get_BoolRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 >=
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwiseAnd8:
		Get_bitwiseRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 &
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwiseOr:
		Get_bitwiseRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 |
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwiseLeftShift8:
		Get_bitwiseRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 <<
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwiseRightShift8:
		Get_bitwiseRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 >>
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwiseXor8:
		Get_bitwiseRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt8 ^
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt8;
		break;
	case InstructionSet::bitwise_Not8:
	{
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt8 = ~Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt8;
	}	break;
#pragma endregion

	#pragma region Set 16Bits

	#pragma endregion
	#pragma region Set 32Bits

	#pragma endregion
	#pragma region Set 64Bits
	case InstructionSet::Store64:
		Get_Register(Inst.Value0_RegisterID).Value = Inst.Value1_AsInt64;
		break;
	case InstructionSet::Store64FromPtrToReg:
		Get_Register(Inst.Value1_RegisterID).Value =
			*(Int64*)(Get_Register(Inst.Value0_RegisterID).Value.AsPtr);
		break;
	case InstructionSet::Store64RegToPtr:
		*(Int64*)(Get_Register(Inst.Value0_RegisterID).Value.AsPtr) =
			      Get_Register(Inst.Value1_RegisterID).Value.AsInt64;
		break;
	case InstructionSet::Push64:
		_CPU.Stack.PushStack(Get_Register(Inst.Value0_RegisterID).Value.AsInt64);
		break;
	case InstructionSet::Pop64:
		Get_Register(Inst.Value0_RegisterID).Value = _CPU.Stack.PopStack<Int64>();
		break;
	case InstructionSet::Store64RegToReg:
		Get_Register(Inst.Value1_RegisterID).Value.AsUInt64 = Get_Register(Inst.Value0_RegisterID).Value.AsUInt64;
		break;

	case InstructionSet::AddS64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt64 +
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt64;
		break;
	case InstructionSet::AddU64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt64 +
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt64;
		break;
	case InstructionSet::SubS64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt64 -
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt64;
		break;
	case InstructionSet::SubU64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt64 -
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt64;
		break;
	case InstructionSet::MultS64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt64 *
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt64;
		break;
	case InstructionSet::MultU64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt64 *
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt64;
		break;
	case InstructionSet::DivS64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsInt64 *
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsInt64;
		break;
	case InstructionSet::DivU64:
		Get_MathOutRegister().Value = Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUInt64 *
			Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUInt64;
		break;
	#pragma endregion
	
	#pragma region Cpp func Set
	case InstructionSet::GetPointerOfStack:
		Get_Register((RegisterID)Inst.Value0_AsInt8).Value = _CPU.Stack.GetTopOfStack();
		break;
	case InstructionSet::IncrementStackPointer:
		_CPU.Stack.IncrementStack(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUIntNative);
		break;
	case InstructionSet::DecrementStackPointer:
		_CPU.Stack.DecrementStack(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUIntNative);
		break;
	case InstructionSet::GetPointerOfStaticMem:
		Get_Register((RegisterID)Inst.Value0_AsInt8).Value = Get_StaticMemPtr();
		break;
	case InstructionSet::Malloc:
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value = 
			         Malloc(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUIntNative);
		break;
	case InstructionSet::Free:
			Free(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsPtr);
		break;
	case InstructionSet::MemCopy:
			MemCopy(Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsPtr
				   ,Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsPtr
			       ,Get_InRegister().Value.AsUIntNative);
		break;
	case InstructionSet::Calloc:
		Get_Register((RegisterID)Inst.Value1_AsInt8).Value =
			Calloc(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsUIntNative);
		break;
	case InstructionSet::ReAlloc:
		Get_OutRegister().Value =
			Realloc(Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsPtr
				   ,Get_Register((RegisterID)Inst.Value1_AsInt8).Value.AsUIntNative);
		break;
	case InstructionSet::Log: 
	{
		const char* Ptr = (const char*)Get_Register((RegisterID)Inst.Value0_AsInt8).Value.AsPtr;
		Log(Ptr);
	}
		break;
		//Linking
	case InstructionSet::CppCall:
	{
		auto Ptr = (UIntNative)Get_StaticMemPtr();
		auto NewPtr = Ptr + Inst.Value0_AsUIntNative;
		_CPU.Stack.PushStack(Inst.Value1_AsUIntNative);
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


