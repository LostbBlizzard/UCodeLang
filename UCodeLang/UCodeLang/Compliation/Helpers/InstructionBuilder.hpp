#pragma once
#include "../../LangCore.hpp"
UCodeLangStart


#pragma region MyRegion

#define BuilderIntSet(bitsize,signedCType,unsignedCType,signedAnyIntValue,unsignedAnyIntValue) \
UCodeLangForceinline static void Store##bitsize(Instruction& Out, RegisterID reg, signedCType Value) \
{ \
	Store##bitsize(Out, reg, *(unsignedCType*)&Value); \
}\
UCodeLangForceinline static void Store##bitsize(Instruction& Out, RegisterID reg, unsignedCType Value)\
{\
	Out.OpCode = InstructionSet::Store##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.##unsignedAnyIntValue = Value;\
}\
UCodeLangForceinline static void StoreFromPtrToReg##bitsize(Instruction& Out, RegisterID pointer, RegisterID OutReg)\
{\
	Out.OpCode = InstructionSet::StoreFromPtrToReg##bitsize;\
	Out.Value0.AsRegister = pointer;\
	Out.Value1.AsRegister = OutReg;\
}\
UCodeLangForceinline static void StoreRegToPtr##bitsize(Instruction& Out, RegisterID Reg, RegisterID pointer)\
{\
	Out.OpCode = InstructionSet::StoreRegToPtr##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsRegister = pointer;\
}\
UCodeLangForceinline static void StoreRegToReg##bitsize(Instruction& Out, RegisterID Reg, RegisterID OutReg)\
{\
	Out.OpCode = InstructionSet::StoreRegToReg##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsRegister = OutReg;\
}\
UCodeLangForceinline static void Push##bitsize(Instruction& Out, RegisterID Reg)\
{\
	Out.OpCode = InstructionSet::Push##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsInt64 = NullUInt64;\
}\
UCodeLangForceinline static void Pop##bitsize(Instruction& Out, RegisterID Reg)\
{\
	Out.OpCode = InstructionSet::Pop##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsInt64 = NullUInt64;\
}\
UCodeLangForceinline static void StoreRegOnStack##bitsize(Instruction& Out, RegisterID Reg, UAddress Stackoffset)\
{\
	Out.OpCode = InstructionSet::StoreRegOnStack##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsAddress = Stackoffset;\
}\
UCodeLangForceinline static void StoreRegOnStackSub##bitsize(Instruction& Out, RegisterID Reg, UAddress Stackoffset)\
{\
	Out.OpCode = InstructionSet::StoreRegOnStackSub##bitsize;\
	Out.Value0.AsRegister = Reg;\
	Out.Value1.AsAddress = Stackoffset;\
}\
UCodeLangForceinline static void GetFromStack##bitsize(Instruction& Out, UAddress Stackoffset, RegisterID Reg)\
{\
	Out.OpCode = InstructionSet::GetFromStack##bitsize;\
	Out.Value0.AsAddress = Stackoffset;\
	Out.Value1.AsRegister = Reg;\
}\
UCodeLangForceinline static void GetFromStackSub##bitsize(Instruction& Out, UAddress Stackoffset, RegisterID Reg)\
{\
	Out.OpCode = InstructionSet::GetFromStackSub##bitsize;\
	Out.Value0.AsAddress = Stackoffset;\
	Out.Value1.AsRegister = Reg;\
}\
UCodeLangForceinline static void Add##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::Add##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void Sub##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::Sub##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void MultS##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::MultS##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void MultU##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::MultU##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void DivS##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::DivS##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void DivU##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::DivU##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void LogicalAnd##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2) \
{\
	Out.OpCode = InstructionSet::LogicalAnd##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void LogicalOr##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::Logicalor##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void LogicalNot##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out.OpCode = InstructionSet::LogicalNot##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = out;\
}\
UCodeLangForceinline static void equalto##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::equalto##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void notequalto##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::notequalto##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void lessthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{ \
	Out.OpCode = InstructionSet::lessthan##bitsize; \
	Out.Value0.AsRegister = reg; \
	Out.Value1.AsRegister = reg2; \
}\
UCodeLangForceinline static void greaterthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::greaterthan##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void equal_lessthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::equal_lessthan##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void equal_greaterthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::equal_greaterthan##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_and##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::bitwiseAnd##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_or##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::bitwiseOr##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_LeftShift##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::bitwiseLeftShift##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_RightShift##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::bitwiseRightShift##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_Xor##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out.OpCode = InstructionSet::bitwiseXor##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = reg2;\
}\
UCodeLangForceinline static void bitwise_Not##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out.OpCode = InstructionSet::bitwise_Not##bitsize;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.AsRegister = out;\
}\

#define BuilderfloatSet(bitsize,CType,AnyIntValue) \
UCodeLangForceinline static void Storef##bitsize(Instruction& Out, RegisterID reg, CType Value) \
{ \
	Out.OpCode = InstructionSet::Store##bitsize##f;\
	Out.Value0.AsRegister = reg;\
	Out.Value1.##AnyIntValue = Value;\
}\

#pragma endregion
	
class InstructionBuilder
{
public:
	UCodeLangForceinline static void Exit(ExitState State, PtrType RetValue, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Exit;
		Out.Value0.AsUInt8 = (ExitState_t)State;
		Out.Value1.AsPtr = RetValue;
	}
	UCodeLangForceinline  static void Exit(ExitState State, Instruction& Out)
	{
		Exit(State, nullptr, Out);
	}
	//Return Value In OutPut RegisterID or push/pop
	UCodeLangForceinline static void Return(ExitState State, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Return;
		Out.Value0.AsUInt8 = (ExitState_t)State;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Call(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Call;
		Out.Value0.AsAddress = address;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void CallIf(UAddress address, RegisterID Bool, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CallIf;
		Out.Value0.AsAddress = address;
		Out.Value1.AsRegister = Bool;
	}
	UCodeLangForceinline static void CallReg(RegisterID Reg, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CallReg;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void CPPCall(UAddress StaticAddress, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CppCallNamed;
		Out.Value0.AsAddress = StaticAddress;
		Out.Value1.AsUInt64 = NullUInt64;
	}

	UCodeLangForceinline static void Jump(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Jump;
		Out.Value0.AsAddress = address;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Jumpif(UAddress address,RegisterID Bool, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Jumpif;
		Out.Value0.AsAddress = address;
		Out.Value1.AsRegister = Bool;
	}
	UCodeLangForceinline static void JumpReg(RegisterID Reg, Instruction& Out)
	{
		Out.OpCode = InstructionSet::JumpReg;
		Out.Value0.AsRegister= Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}


	UCodeLangForceinline static void DoNothing(Instruction& Out)
	{
		Out.OpCode = InstructionSet::DoNothing;
		Out.Value0.AsUInt64 = NullUInt64;
		Out.Value1.AsUInt64 = NullUInt64;
	}

	UCodeLangForceinline  static void GenInst(InstructionSet_t Inst, const UInt64 Value0, const UInt64 Value1, Instruction& Out)
	{
		Out.OpCode_AsInt = Inst;
		Out.Value0.AsUInt64 = Value0;
		Out.Value1.AsUInt64 = Value1;
	}

	//set 8
	BuilderIntSet(8, Int8, UInt8, AsInt8, AsUInt8)
	//set 16 
	BuilderIntSet(16, Int16, UInt16, AsInt16, AsUInt16)
	//set 32 
	BuilderIntSet(32, Int32, UInt32, AsInt32, AsUInt32)
	//set 64 
	BuilderIntSet(64, Int64, UInt64, AsInt64, AsUInt64)

	//float set 32
	BuilderfloatSet(32, float32,Asfloat32)
	//float set 64
	BuilderfloatSet(64, float64,Asfloat64)

	
	//C funcs
	UCodeLangForceinline static void GetPointerOfStack(Instruction& Out, RegisterID out,UAddress offset)
	{
		Out.OpCode = InstructionSet::GetPointerOfStack;
		Out.Value0.AsRegister = out;
		Out.Value1.AsUInt64 = offset;
	}
	UCodeLangForceinline static void GetPointerOfStackSub(Instruction& Out, RegisterID out, UAddress offset)
	{
		Out.OpCode = InstructionSet::GetPointerOfStackSub;
		Out.Value0.AsRegister = out;
		Out.Value1.AsUInt64 = offset;
	}

	UCodeLangForceinline static void GetPointerOfStaticMem(Instruction& Out, RegisterID out, UAddress offset)
	{
		Out.OpCode = InstructionSet::GetPointerOfStaticMem;
		Out.Value0.AsRegister = out;
		Out.Value1.AsUInt64 = offset;
	}
	UCodeLangForceinline static void IncrementStackPointer(Instruction& Out, RegisterID Value)
	{
		Out.OpCode = InstructionSet::IncrementStackPointer;
		Out.Value0.AsRegister = Value;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void DecrementStackPointer(Instruction& Out, RegisterID Value)
	{
		Out.OpCode = InstructionSet::DecrementStackPointer;
		Out.Value0.AsRegister = Value;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Malloc(Instruction& Out, RegisterID regin, RegisterID regout)
	{
		Out.OpCode = InstructionSet::Malloc;
		Out.Value0.AsRegister = regin;
		Out.Value1.AsRegister = regout;
	}
	UCodeLangForceinline static void Free(Instruction& Out, RegisterID reg)
	{
		Out.OpCode = InstructionSet::Free;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}

	//Size should be in InputRegister
	UCodeLangForceinline static void MemCopy(Instruction& Out, RegisterID dest, RegisterID source)
	{
		Out.OpCode = InstructionSet::MemCopy;
		Out.Value0.AsRegister = dest;
		Out.Value1.AsRegister = source;
	}


	UCodeLangForceinline static void Calloc(Instruction& Out, RegisterID regin, RegisterID regout)
	{
		Out.OpCode = InstructionSet::Calloc;
		Out.Value0.AsRegister = regin;
		Out.Value1.AsRegister = regout;
	}

	//Output is in OutPutRegister
	UCodeLangForceinline static void Realloc(Instruction& Out, RegisterID ptr, RegisterID size)
	{
		Out.OpCode = InstructionSet::ReAlloc;
		Out.Value0.AsRegister = ptr;
		Out.Value1.AsRegister = size;
	}

	UCodeLangForceinline static void LoadFuncPtr(UAddress address, RegisterID ptr, Instruction& Out)
	{
		Out.OpCode = InstructionSet::LoadFuncPtr;
		Out.Value0.AsAddress = address;
		Out.Value1.AsRegister= ptr;
	}


	//Calls the native code in _Code in UClib/
	UCodeLangForceinline static void CallCode(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Call_Code;
		Out.Value0.AsAddress = address;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	

	UCodeLangForceinline static void SysCall(Instruction& Out, InstructionSysCall Type, Optional<RegisterID> InReg = {})
	{
		Out.OpCode = InstructionSet::SysCall;
		Out.Value0.AsUInt64 = *(UInt64*)&Type;

		if (InReg.has_value()) 
		{
			Out.Value1.AsRegister = InReg.value();
		}
		else
		{
			Out.Value1.AsUInt64 = NullUInt64;
		}
	}

	//Sys Calls
	UCodeLangForceinline static void Log(Instruction& Out, RegisterID CharPtr)
	{
		SysCall(Out,InstructionSysCall::Cout_CString, CharPtr);
	}
	UCodeLangForceinline static void LogChar(Instruction& Out, RegisterID Char)
	{
		SysCall(Out, InstructionSysCall::Cout_Char, Char);
	}

	//size will be in Input Reg
	UCodeLangForceinline static void LogBuffer(Instruction& Out, RegisterID Char)
	{
		SysCall(Out, InstructionSysCall::Cout_Buffer, Char);
	}

	UCodeLangForceinline static void ReadChar(Instruction& Out, RegisterID Char)
	{
		SysCall(Out, InstructionSysCall::Cout_ReadChar, Char);
	}

	


	//Others
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, const UInt64 Value0, const UInt64 Value1, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, Value0, Value1, Out);
	};
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, const void* Value0, const void* Value1, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)Value1, Out);
	};
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, const void* Value0, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)NullAddress, Out);
	};
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, size_t Value0, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)NullAddress, Out);
	};
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, RegisterID Register, size_t Value0, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Register, (UInt64)Value0, Out);
	};
	UCodeLangForceinline static void GenInst(Intermediate_Set Inst, Instruction& Out)
	{
		return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)NullAddress, (UInt64)NullAddress, Out);
	};
};
UCodeLangEnd

