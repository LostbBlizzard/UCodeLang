#pragma once
#include "../../LangCore.hpp"
UCodeLangStart


#pragma region MyRegion

#define BuilderIntSet(bitsize) \
UCodeLangForceinline static void StoreFromPtrToReg##bitsize(Instruction& Out, RegisterID pointer, RegisterID OutReg)\
{\
	Out = Instruction(InstructionSet::StoreFromPtrToReg##bitsize,Instruction::TwoReg(pointer,OutReg));\
}\
UCodeLangForceinline static void StoreRegToPtr##bitsize(Instruction& Out, RegisterID Reg, RegisterID pointer)\
{\
	Out = Instruction(InstructionSet::StoreRegToPtr##bitsize,Instruction::TwoReg(Reg,pointer));\
}\
UCodeLangForceinline static void StoreRegToReg##bitsize(Instruction& Out, RegisterID Reg, RegisterID OutReg)\
{\
	Out = Instruction(InstructionSet::StoreRegToReg##bitsize,Instruction::TwoReg(Reg,OutReg));\
}\
UCodeLangForceinline static void Push##bitsize(Instruction& Out, RegisterID Reg)\
{\
	Out = Instruction(InstructionSet::Push##bitsize,Instruction::OneReg(Reg));\
}\
UCodeLangForceinline static void Pop##bitsize(Instruction& Out, RegisterID Reg)\
{\
	Out = Instruction(InstructionSet::Pop##bitsize,Instruction::OneReg(Reg));\
}\
UCodeLangForceinline static void StoreRegOnStack##bitsize(Instruction& Out, RegisterID Reg,UInt16  Stackoffset)\
{\
	Out = Instruction(InstructionSet::StoreRegOnStack##bitsize,Instruction::RegUInt16(Reg,Stackoffset));\
}\
UCodeLangForceinline static void StoreRegOnStackSub##bitsize(Instruction& Out, RegisterID Reg,UInt16  Stackoffset)\
{\
	Out = Instruction(InstructionSet::StoreRegOnStackSub##bitsize,Instruction::RegUInt16(Reg,Stackoffset));\
}\
UCodeLangForceinline static void GetFromStack##bitsize(Instruction& Out, UInt16 Stackoffset, RegisterID Reg)\
{\
	Out = Instruction(InstructionSet::GetFromStack##bitsize,Instruction::RegUInt16(Reg,Stackoffset));\
}\
UCodeLangForceinline static void GetFromStackSub##bitsize(Instruction& Out, UInt16 Stackoffset, RegisterID Reg)\
{\
  Out = Instruction(InstructionSet::GetFromStackSub##bitsize,Instruction::RegUInt16(Reg,Stackoffset));\
}\
UCodeLangForceinline static void Add##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::Add##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void Sub##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::Sub##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void MultS##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::MultS##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void MultU##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::MultU##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void DivS##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::DivS##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void DivU##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::DivU##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void LogicalAnd##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2) \
{\
	Out = Instruction(InstructionSet::LogicalAnd##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void LogicalOr##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::Logicalor##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void LogicalNot##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out = Instruction(InstructionSet::LogicalNot##bitsize, Instruction::TwoReg(reg,out));\
}\
UCodeLangForceinline static void equalto##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::equalto##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void notequalto##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::notequalto##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void lessthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{ \
	Out = Instruction(InstructionSet::lessthan##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void greaterthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::greaterthan##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void equal_lessthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::equal_lessthan##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void equal_greaterthan##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::equal_greaterthan##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_and##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::bitwiseAnd##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_or##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::bitwiseOr##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_LeftShift##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::bitwiseLeftShift##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_RightShift##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::bitwiseRightShift##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_Xor##bitsize(Instruction& Out, RegisterID reg, RegisterID reg2)\
{\
	Out = Instruction(InstructionSet::bitwiseXor##bitsize, Instruction::TwoReg(reg,reg2));\
}\
UCodeLangForceinline static void bitwise_Not##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out = Instruction(InstructionSet::bitwise_Not##bitsize, Instruction::TwoReg(reg,out));\
}\
UCodeLangForceinline static void UInt##bitsize##To##SInt##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out = Instruction(InstructionSet::UInt##bitsize##ToSInt##bitsize, Instruction::TwoReg(reg,out));\
}\
UCodeLangForceinline static void SInt##bitsize##To##UInt##bitsize(Instruction& Out, RegisterID reg, RegisterID out)\
{\
	Out = Instruction(InstructionSet::SInt##bitsize##ToUInt##bitsize, Instruction::TwoReg(reg,out));\
}\


#define BuilderfloatSet(bitsize,CType,AnyIntValue) \

#pragma endregion
	
class InstructionBuilder
{
public:
	UCodeLangForceinline static void Exit(ExitState State, PtrType RetValue, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Exit, Instruction::ValUInt8((ExitState_t)State));
	}
	UCodeLangForceinline  static void Exit(ExitState State, Instruction& Out)
	{
		Exit(State, nullptr, Out);
	}
	//Return Value In OutPut RegisterID or push/pop
	UCodeLangForceinline static void Return(ExitState State, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Return, Instruction::ValUInt8((ExitState_t)State));
	}
	UCodeLangForceinline static void Call(UInt16 address, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Call, Instruction::ValUInt16(address));
	}
	UCodeLangForceinline static void CallIf(UAddress address, RegisterID Bool, Instruction& Out)
	{
		Out = Instruction(InstructionSet::CallIf, Instruction::RegUInt16(Bool,address));
	}
	UCodeLangForceinline static void CallReg(RegisterID Reg, Instruction& Out)
	{
		Out = Instruction(InstructionSet::CallReg, Instruction::OneReg(Reg));
	}
	UCodeLangForceinline static void CPPCall(UInt16 StaticAddress, Instruction& Out)
	{
		Out = Instruction(InstructionSet::CppCallNamed, Instruction::ValUInt16(StaticAddress));
	}

	UCodeLangForceinline static void Jump(UInt16 address, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Jump, Instruction::ValUInt16(address));
	}
	UCodeLangForceinline static void Jumpif(UInt16 address,RegisterID Bool, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Jumpif, Instruction::RegUInt16(Bool,address));
	}
	UCodeLangForceinline static void JumpReg(RegisterID Reg, Instruction& Out)
	{
		Out = Instruction(InstructionSet::JumpReg, Instruction::OneReg(Reg));
	}


	UCodeLangForceinline static void DoNothing(Instruction& Out)
	{
		Out = Instruction(InstructionSet::DoNothing, Instruction::NoneOp());
	}

	/*
	UCodeLangForceinline  static void GenInst(InstructionSet_t Inst, const UInt64 Value0, const UInt64 Value1, Instruction& Out)
	{
		Out.OpCode_AsInt = Inst;
		Out.Value0.AsUInt64 = Value0;
		Out.Value1.AsUInt64 = Value1;
	}
	*/

	
	//set 8
	
	UCodeLangForceinline static void Store8(Instruction& Out, RegisterID reg, Int8 Value) \
	{ 
		Store8(Out, reg, *(UInt8*)&Value); 
	}
	UCodeLangForceinline static void Store8(Instruction& Out, RegisterID reg, UInt8 Value)\
	{
		Out = Instruction(InstructionSet::Store8, Instruction::RegUInt8(reg,Value)); 
	}

	BuilderIntSet(8)
	//set 16 
	
	UCodeLangForceinline static void Store16(Instruction& Out, RegisterID reg, Int16 Value) \
	{
		Store16(Out, reg, *(UInt16*)&Value);
	}
	UCodeLangForceinline static void Store16(Instruction& Out, RegisterID reg, UInt16 Value)\
	{
		Out = Instruction(InstructionSet::Store16, Instruction::RegUInt16(reg, Value));
	}
	
	BuilderIntSet(16)
	//set 32 
	
	//Loading 32bit
	UCodeLangForceinline static void Store32_V1(Instruction& Out, RegisterID reg, Int32 Value) \
	{
		Store32_V1(Out, reg, *(UInt32*)&Value);
	}
	UCodeLangForceinline static void Store32_V1(Instruction& Out, RegisterID reg, UInt32 Value)\
	{
		Out = Instruction(InstructionSet::Storef32v1, Instruction::RegUInt16(reg, Value));
	}

	UCodeLangForceinline static void Store32_V2(Instruction& Out, RegisterID reg, Int32 Value) \
	{
		Store32_V2(Out, reg, *(UInt32*)&Value);
	}
	UCodeLangForceinline static void Store32_V2(Instruction& Out, RegisterID reg, UInt32 Value)\
	{
		Out = Instruction(InstructionSet::Storef32v2, Instruction::RegUInt16(reg, Value));
	}
	
	BuilderIntSet(32)
	//set 64 
	
	//Loading 64bit
	UCodeLangForceinline static void Store64_V1(Instruction& Out, RegisterID reg,UInt64 Value) \
	{
		Store64_V1(Out, reg, *(Int64*)&Value);
	}
	UCodeLangForceinline static void Store64_V1(Instruction& Out, RegisterID reg,Int64 Value)\
	{
		UInt16& Val = ((UInt16*)(&Value))[0];

		Out = Instruction(InstructionSet::Store64v1, Instruction::RegUInt16(reg, Val));
	}

	UCodeLangForceinline static void Store64_V2(Instruction& Out, RegisterID reg, UInt64 Value) \
	{
		Store64_V2(Out, reg, *(Int64*)&Value);
	}
	UCodeLangForceinline static void Store64_V2(Instruction& Out, RegisterID reg, Int64 Value)\
	{
		UInt16& Val = ((UInt16*)(&Value))[1];

		Out = Instruction(InstructionSet::Store64v2, Instruction::RegUInt16(reg, Val));
	}

	UCodeLangForceinline static void Store64_V3(Instruction& Out, RegisterID reg, UInt64 Value) \
	{
		Store64_V3(Out, reg, *(Int64*)&Value);
	}
	UCodeLangForceinline static void Store64_V3(Instruction& Out, RegisterID reg, Int64 Value)\
	{
		UInt16& Val = ((UInt16*)(&Value))[2];

		Out = Instruction(InstructionSet::Store64v3, Instruction::RegUInt16(reg, Val));
	}

	UCodeLangForceinline static void Store64_V4(Instruction& Out, RegisterID reg, UInt64 Value) \
	{
		Store64_V4(Out, reg, *(Int64*)&Value);
	}
	UCodeLangForceinline static void Store64_V4(Instruction& Out, RegisterID reg, Int64 Value)\
	{
		UInt16& Val = ((UInt16*)(&Value))[3];

		Out = Instruction(InstructionSet::Store64v4, Instruction::RegUInt16(reg, Val));
	}
	BuilderIntSet(64, Int64, UInt64, AsInt64, AsUInt64)

	//float set 32

	UCodeLangForceinline static void Storef32_V1(Instruction& Out, RegisterID reg, float32 Value) \
	{ 
		UInt16& Val = ((UInt16*)(&Value))[0];

		Out = Instruction(InstructionSet::Storef32v1, Instruction::RegUInt16(reg, Val));
	}
	UCodeLangForceinline static void Storef32_V2(Instruction& Out, RegisterID reg, float32 Value) \
	{
		UInt16& Val = ((UInt16*)(&Value))[1];

		Out = Instruction(InstructionSet::Storef32v2, Instruction::RegUInt16(reg, Val));
	}

	BuilderfloatSet(32)
	//float set 64

	UCodeLangForceinline static void Storef64_V1(Instruction& Out, RegisterID reg, float64 Value) \
	{
		UInt16& Val = ((UInt16*)(&Value))[0];

		Out = Instruction(InstructionSet::Storef64v1, Instruction::RegUInt16(reg, Val));
	}
	UCodeLangForceinline static void Storef64_V2(Instruction& Out, RegisterID reg, float64 Value) \
	{
		UInt16& Val = ((UInt16*)(&Value))[1];

		Out = Instruction(InstructionSet::Storef64v2, Instruction::RegUInt16(reg, Val));
	}
	UCodeLangForceinline static void Storef64_V3(Instruction& Out, RegisterID reg, float64 Value) \
	{
		UInt16& Val = ((UInt16*)(&Value))[2];

		Out = Instruction(InstructionSet::Storef64v3, Instruction::RegUInt16(reg, Val));
	}
	UCodeLangForceinline static void Storef64_V4(Instruction& Out, RegisterID reg, float64 Value) \
	{
		UInt16& Val = ((UInt16*)(&Value))[3];

		Out = Instruction(InstructionSet::Storef64v4, Instruction::RegUInt16(reg, Val));
	}


	BuilderfloatSet(64)

	//Casting
	UCodeLangForceinline static void Int8ToInt16(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int8ToInt16, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void Int16ToInt32(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int16ToInt32, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void Int32ToInt64(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int32ToInt64, Instruction::TwoReg(Regin, Regout));
	}

	UCodeLangForceinline static void Int64ToInt32(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int64ToInt32, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void Int32ToInt16(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int32ToInt16, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void Int16ToInt8(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int16ToInt8, Instruction::TwoReg(Regin, Regout));
	}


	UCodeLangForceinline static void float64ToInt64(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::float64ToInt64, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void float32ToInt32(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::float32ToInt32, Instruction::TwoReg(Regin, Regout));
	}

	UCodeLangForceinline static void Int64Tofloat64(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::float64ToInt64, Instruction::TwoReg(Regin, Regout));
	}
	UCodeLangForceinline static void Int32Tofloat32(Instruction& Out, RegisterID Regin, RegisterID  Regout)
	{
		Out = Instruction(InstructionSet::Int32Tofloat32, Instruction::TwoReg(Regin, Regout));
	}


	UCodeLangForceinline static void LoadFuncPtr(UInt16 address, RegisterID ptr, Instruction& Out)
	{
		Out = Instruction(InstructionSet::LoadFuncPtr, Instruction::RegUInt16(ptr, address));
	}

	//Stack,Thread,Static
	UCodeLangForceinline static void GetPointerOfStack(Instruction& Out, RegisterID out,UInt16 offset)
	{
		Out = Instruction(InstructionSet::GetPointerOfStack, Instruction::RegUInt16(out, offset));
	}
	UCodeLangForceinline static void GetPointerOfStackSub(Instruction& Out, RegisterID out, UInt16 offset)
	{
		Out = Instruction(InstructionSet::GetPointerOfStackSub, Instruction::RegUInt16(out, offset));
	}

	UCodeLangForceinline static void GetPointerOfStaticMem(Instruction& Out, RegisterID out, UInt16 offset)
	{
		Out = Instruction(InstructionSet::GetPointerOfStaticMem, Instruction::RegUInt16(out, offset));
	}
	UCodeLangForceinline static void GetPointerOfThreadMem(Instruction& Out, RegisterID out, UInt16 offset)
	{
		Out = Instruction(InstructionSet::GetPointerOfThreadMem, Instruction::RegUInt16(out, offset));
	}

	UCodeLangForceinline static void IncrementStackPointer(Instruction& Out, RegisterID Value)
	{
		Out = Instruction(InstructionSet::IncrementStackPointer, Instruction::OneReg(Value));
	}
	UCodeLangForceinline static void DecrementStackPointer(Instruction& Out, RegisterID Value)
	{
		Out = Instruction(InstructionSet::DecrementStackPointer, Instruction::OneReg(Value));
	}
	
	//C funcs
	UCodeLangForceinline static void Malloc(Instruction& Out, RegisterID regin, RegisterID regout)
	{
		Out = Instruction(InstructionSet::Malloc, Instruction::TwoReg(regin, regout));
	}
	UCodeLangForceinline static void Free(Instruction& Out, RegisterID reg)
	{
		Out = Instruction(InstructionSet::Free, Instruction::OneReg(reg));
	}


	UCodeLangForceinline static void PointerMemberLoad8(Instruction& Out,RegisterID Ptr,RegisterID RegOut, UInt16 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberLoad8, Instruction::TwoRegInt8(Ptr,RegOut,Offset));
	}
	UCodeLangForceinline static void PointerMemberLoad16(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt16 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberLoad16, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void PointerMemberLoad32(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt16 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberLoad32, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void PointerMemberLoad64(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt16 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberLoad64, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}

	UCodeLangForceinline static void PointerMemberRead8(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt8 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberRead8, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void PointerMemberRead16(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt8 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberRead16, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void PointerMemberRead32(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt8 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberRead32, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void PointerMemberRead64(Instruction& Out, RegisterID Ptr, RegisterID RegOut, UInt8 Offset)
	{
		Out = Instruction(InstructionSet::PointerMemberRead64, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	//EffectiveAddress
	UCodeLangForceinline static void LoadEffectiveAddressA(Instruction& Out, RegisterID Ptr, UInt8 Offset, RegisterID RegOut)
	{
		Out = Instruction(InstructionSet::LoadEffectiveAddressA, Instruction::TwoRegInt8(Ptr, RegOut,Offset));
	}
	UCodeLangForceinline static void LoadEffectiveAddressS(Instruction& Out, RegisterID Ptr, UInt8 Offset, RegisterID RegOut)
	{
		Out = Instruction(InstructionSet::LoadEffectiveAddressS, Instruction::TwoRegInt8(Ptr, RegOut, Offset));
	}
	UCodeLangForceinline static void LoadEffectiveAddressX(Instruction& Out, RegisterID Ptr, UInt8 Mult, RegisterID RegOut)
	{
		Out = Instruction(InstructionSet::LoadEffectiveAddressX, Instruction::TwoRegInt8(Ptr, RegOut, Mult));
	}

	UCodeLangForceinline static void MemCopy(Instruction& Out, RegisterID dest, RegisterID size, RegisterID source)
	{
		Out = Instruction(InstructionSet::MemCopy, Instruction::ThreeReg(dest, size, source));
	}


	UCodeLangForceinline static void Calloc(Instruction& Out, RegisterID regin, RegisterID regout)
	{
		Out = Instruction(InstructionSet::Calloc, Instruction::TwoReg(regin, regout));
	}

	
	UCodeLangForceinline static void Realloc(Instruction& Out, RegisterID ptr, RegisterID size, RegisterID output)
	{
		Out = Instruction(InstructionSet::ReAlloc, Instruction::ThreeReg(ptr, size,output));
	}

	UCodeLangForceinline static void Memset(Instruction& Out, RegisterID Ptr, RegisterID Val, RegisterID size)
	{
		Out = Instruction(InstructionSet::Memset, Instruction::ThreeReg(Ptr,Val,size));
	}

	//Value is in Out
	UCodeLangForceinline static void Memcmp(Instruction& Out, RegisterID Ptr, RegisterID Ptr2, RegisterID size)
	{
		Out = Instruction(InstructionSet::Memcmp, Instruction::ThreeReg(Ptr, Ptr2, size));
	}

	UCodeLangForceinline static void Strlen(Instruction& Out, RegisterID Ptr, RegisterID OutV)
	{
		Out = Instruction(InstructionSet::Strlen, Instruction::TwoReg(Ptr, OutV));
	}


	//Calls the native code in _Code in UClib/
	UCodeLangForceinline static void CallCode(UInt16 address, Instruction& Out)
	{
		Out = Instruction(InstructionSet::Call_Code, Instruction::ValUInt16(address));
	}
	

	//Sys Calls
	UCodeLangForceinline static void Log(Instruction& Out, RegisterID CharPtr)
	{
		Out = Instruction(InstructionSet::Cout_CString, Instruction::OneReg(CharPtr));
	}
	UCodeLangForceinline static void LogChar(Instruction& Out, RegisterID Char)
	{
		Out = Instruction(InstructionSet::Cout_Char, Instruction::OneReg(Char));
	}

	UCodeLangForceinline static void LogBuffer(Instruction& Out, RegisterID ptr, RegisterID size)
	{
		Out = Instruction(InstructionSet::Cout_Buffer, Instruction::TwoReg(ptr,size));
	}

	UCodeLangForceinline static void ReadChar(Instruction& Out, RegisterID Char)
	{
		Out = Instruction(InstructionSet::Cout_ReadChar, Instruction::OneReg(Char));
	}

	UCodeLangForceinline static void ReadBuffer(Instruction& Out, RegisterID ptr, RegisterID size)
	{
		Out = Instruction(InstructionSet::Cout_ReadBuffer, Instruction::TwoReg(ptr,size));
	}


	//Debug
	UCodeLangForceinline static void Debug_FuncStart(Instruction& Out)
	{
		Out = Instruction(InstructionSet::Debug_FuncStart, Instruction::NoneOp());
	}
	UCodeLangForceinline static void Debug_FuncEnd(Instruction& Out)
	{
		Out = Instruction(InstructionSet::Debug_FuncEnd, Instruction::NoneOp());
	}
	UCodeLangForceinline static void Debug_LineEnter(Instruction& Out)
	{
		Out = Instruction(InstructionSet::Debug_LineEnter, Instruction::NoneOp());
	}

	
	//Others
	/*
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
	*/
};
UCodeLangEnd

