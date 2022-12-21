#pragma once
#include "../../LangCore.hpp"
UCodeLangStart
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
	UCodeLangForceinline static void CallIf(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CallIf;
		Out.Value0.AsAddress = address;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void CallPtr(UAddress addresstoReadFrom, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CallPtr;
		Out.Value0.AsAddress = addresstoReadFrom;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void CPPCall(UAddress StaticAddress, NSize_t ParsSize, Instruction& Out)
	{
		Out.OpCode = InstructionSet::CppCallNamed;
		Out.Value0.AsAddress = StaticAddress;
		Out.Value1.AsUInt64 = ParsSize;
	}

	UCodeLangForceinline static void Jump(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Jump;
		Out.Value0.AsAddress = address;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Jumpif(UAddress address, Instruction& Out)
	{
		Out.OpCode = InstructionSet::Jumpif;
		Out.Value0.AsAddress = address;
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
	#pragma region Set8
	UCodeLangForceinline static void Store8(Instruction& Out, RegisterID reg, Int8 Value)
	{
		Store8(Out, reg, *(UInt8*)&Value);
	}
	UCodeLangForceinline static void Store8(Instruction& Out, RegisterID reg, UInt8 Value)
	{
		Out.OpCode = InstructionSet::Store8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsInt8 = Value;
	}
	UCodeLangForceinline static void Store8FromPtrToReg(Instruction& Out, RegisterID pointer, RegisterID OutReg)
	{
		Out.OpCode = InstructionSet::Store8FromPtrToReg;
		Out.Value0.AsRegister = pointer;
		Out.Value1.AsRegister = OutReg;
	}
	UCodeLangForceinline static void Store8RegToPtr(Instruction& Out, RegisterID Reg, RegisterID pointer)
	{
		Out.OpCode = InstructionSet::Store8RegToPtr;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsRegister = pointer;
	}
	UCodeLangForceinline static void Push8(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Push8;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsInt8 = NullUInt64;
	}
	UCodeLangForceinline static void Pop8(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Pop8;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Store8RegOnStack(Instruction& Out, RegisterID Reg, UAddress Stackoffset)
	{
		Out.OpCode = InstructionSet::Store8RegOnStack;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsAddress = Stackoffset;
	}
	UCodeLangForceinline static void Get8FromStack(Instruction& Out, UAddress Stackoffset, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Get8FromStack;
		Out.Value0.AsAddress= Stackoffset;
		Out.Value1.AsRegister = Reg;
	}
	UCodeLangForceinline static void Get8FromStackSub(Instruction& Out, UAddress Stackoffset, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Get8FromStackSub;
		Out.Value0.AsAddress = Stackoffset;
		Out.Value1.AsRegister = Reg;
	}

	//math
	UCodeLangForceinline static void Add8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::Add8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Sub8U(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::Sub8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Mult8S(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::MultS8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Mult8U(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::MultU8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Div8S(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::DivS8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Div8U(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::DivU8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}

	UCodeLangForceinline static void LogicalAnd8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::LogicalAnd8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}

	UCodeLangForceinline static void LogicalOr8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::Logicalor8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void LogicalNot8(Instruction& Out, RegisterID reg, RegisterID out)
	{
		Out.OpCode = InstructionSet::LogicalNot8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = out;
	}



	UCodeLangForceinline static void equalto8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::equalto8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void notequalto8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::notequalto8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void lessthan8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::lessthan8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void greaterthan8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::greaterthan8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void equal_lessthan8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::equal_lessthan8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void equal_greaterthan8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::equal_greaterthan8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}

	UCodeLangForceinline static void bitwise_and8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::bitwiseAnd8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void bitwise_or8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::bitwiseOr;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void bitwise_LeftShift8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::bitwiseLeftShift8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void bitwise_RightShift8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::bitwiseRightShift8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void bitwise_Xor8(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::bitwiseXor8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}

	UCodeLangForceinline static void bitwise_Not8(Instruction& Out, RegisterID reg, RegisterID out)
	{
		Out.OpCode = InstructionSet::bitwise_Not8;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = out;
	}
#pragma endregion
	//set 32 
	#pragma region 32
	UCodeLangForceinline static void Store32(Instruction& Out, RegisterID reg, Int32 Value)
	{
		Store32(Out, reg, *(UInt32*)&Value);
	}
	UCodeLangForceinline static void Store32(Instruction& Out, RegisterID reg, UInt32 Value)
	{
		Out.OpCode = InstructionSet::Store32;
		Out.Value1.AsRegister = reg;
		Out.Value1.AsUInt32 = Value;
	}
	UCodeLangForceinline static void Store32FromPtrToReg(Instruction& Out, RegisterID pointer, RegisterID OutReg)
	{
		Out.OpCode = InstructionSet::Store32FromPtrToReg;
		Out.Value0.AsRegister = pointer;
		Out.Value1.AsRegister = OutReg;
	}
	UCodeLangForceinline static void Store32RegToPtr(Instruction& Out, RegisterID Reg, RegisterID pointer)
	{
		Out.OpCode = InstructionSet::Store32RegToPtr;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsRegister = pointer;
	}
	UCodeLangForceinline static void Push32(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Push32;
		Out.Value0.AsRegister = Reg;
		Out.Value0.AsInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Pop32(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Pop32;
		Out.Value0.AsRegister = Reg;
		Out.Value0.AsInt64 = NullUInt64;
	}
#pragma endregion
	//set 64 
	#pragma region 64
	UCodeLangForceinline  static void Store64(Instruction& Out, RegisterID reg, Int64 Value)
	{
		Store64(Out, reg, *(UInt64*)&Value);
	}
	UCodeLangForceinline static void Store64(Instruction& Out, RegisterID reg, UInt64 Value)
	{
		Out.OpCode = InstructionSet::Store64;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsUInt64 = Value;
	}
	UCodeLangForceinline static void Store64FromPtrToReg(Instruction& Out, RegisterID pointer, RegisterID OutReg)
	{
		Out.OpCode = InstructionSet::Store64FromPtrToReg;
		Out.Value0.AsRegister = pointer;
		Out.Value1.AsRegister = OutReg;
	}
	UCodeLangForceinline static void Store64RegToPtr(Instruction& Out, RegisterID Reg, RegisterID pointer)
	{
		Out.OpCode = InstructionSet::Store64RegToPtr;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsRegister = pointer;
	}
	UCodeLangForceinline static void Push64(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Push64;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Pop64(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::Pop64;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void Store64RegToReg(Instruction& Out, RegisterID in, RegisterID out)
	{
		Out.OpCode = InstructionSet::Store64RegToReg;
		Out.Value0.AsRegister = in;
		Out.Value1.AsRegister = out;
	}

	UCodeLangForceinline static void Add64S(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::AddS64;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void Add64U(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::AddU64;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	static void Sub64S(Instruction& Out, RegisterID reg, RegisterID reg2);
	static void Sub64U(Instruction& Out, RegisterID reg, RegisterID reg2);
	static void Mult64S(Instruction& Out, RegisterID reg, RegisterID reg2);
	static void Mult64U(Instruction& Out, RegisterID reg, RegisterID reg2);
	static void Div64S(Instruction& Out, RegisterID reg, RegisterID reg2);
	static void Div64U(Instruction& Out, RegisterID reg, RegisterID reg2);
#pragma endregion
	
	#pragma region Native Set
	UCodeLangForceinline static void StoreNativeU(Instruction& Out, RegisterID reg, UIntNative Value)
	{

		Out.OpCode = InstructionSet::StoreNativeU;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsUInt64 = Value;
	}
	UCodeLangForceinline static void StoreNativeS(Instruction& Out, RegisterID reg, SIntNative Value)
	{
		Out.OpCode = InstructionSet::StoreNativeU;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsInt64 = Value;
	}
	UCodeLangForceinline static void StoreNativeFromPtrToReg(Instruction& Out, RegisterID pointer, RegisterID OutReg)
	{
		Out.OpCode = InstructionSet::StoreNativeFromPtrToReg;
		Out.Value0.AsRegister = pointer;
		Out.Value1.AsRegister = OutReg;
	}
	UCodeLangForceinline static void StoreNativeRegToPtr(Instruction& Out, RegisterID Reg, RegisterID pointer)
	{
		Out.OpCode = InstructionSet::StoreNativeRegToPtr;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsRegister = pointer;
	}
	UCodeLangForceinline static void PushNative(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::PushNative;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void PopNative(Instruction& Out, RegisterID Reg)
	{
		Out.OpCode = InstructionSet::PopNative;
		Out.Value0.AsRegister = Reg;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void StoreNativeRegToReg(Instruction& Out, RegisterID in, RegisterID out)
	{

		Out.OpCode = InstructionSet::StoreNativeRegToReg;
		Out.Value0.AsRegister = in;
		Out.Value1.AsRegister = out;
	}

	UCodeLangForceinline static void AddNativeS(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::AddSNative;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	UCodeLangForceinline static void AddNativeU(Instruction& Out, RegisterID reg, RegisterID reg2)
	{
		Out.OpCode = InstructionSet::AddUNative;
		Out.Value0.AsRegister = reg;
		Out.Value1.AsRegister = reg2;
	}
	#pragma endregion
	//C funcs
	UCodeLangForceinline static void GetPointerOfStack(Instruction& Out, RegisterID out)
	{
		Out.OpCode = InstructionSet::GetPointerOfStack;
		Out.Value0.AsRegister = out;
		Out.Value1.AsUInt64 = NullUInt64;
	}
	UCodeLangForceinline static void GetPointerOfStaticMem(Instruction& Out, RegisterID out)
	{
		Out.OpCode = InstructionSet::GetPointerOfStaticMem;
		Out.Value0.AsRegister = out;
		Out.Value1.AsUInt64 = NullUInt64;
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
	
	UCodeLangForceinline static void Log(Instruction& Out, RegisterID CharPtr)
	{
		Out.OpCode = InstructionSet::Log;
		Out.Value0.AsRegister = CharPtr; 
		Out.Value1.AsUInt64 = NullUInt64;
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

