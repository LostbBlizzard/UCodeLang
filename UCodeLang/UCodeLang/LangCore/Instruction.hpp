#pragma once
#include "InstructionSet.hpp"
#include "LangTypes.hpp"
UCodeLangStart

/*
struct Instruction
{
	constexpr Instruction() :
		OpCode(InstructionSet::DoNothing)
		, Value0(NullUInt64)
		, Value1(NullUInt64)
	{

	}
	union
	{
		InstructionSet OpCode;
		InstructionSet_t OpCode_AsInt;
	};
	AnyInt64 Value0;
	AnyInt64 Value1;
};
*/
struct Instruction
{
	#pragma pack(push, 1)
	struct OneReg
	{
		RegisterID A;
		constexpr OneReg(RegisterID A) :
			A(A)
		{

		}
	};
	struct TwoReg
	{
		RegisterID A; 
		RegisterID B;
		constexpr TwoReg(RegisterID A, RegisterID B):
			A(A),B(B)
		{

		}
	};
	struct ThreeReg
	{
		RegisterID A;
		RegisterID B;
		RegisterID C;
		constexpr ThreeReg(RegisterID A, RegisterID B, RegisterID C):
			 A(A), B(B), C(C)
		{

		}
	};

	struct TwoRegInt8
	{
		RegisterID A;
		RegisterID B;
		UInt8 C;
		constexpr TwoRegInt8(RegisterID A, RegisterID B, UInt8 C) :
			A(A), B(B), C(C)
		{

		}
	};

	struct RegUInt16
	{
		RegisterID A;
		UInt16 B;
		constexpr RegUInt16(RegisterID A, UInt16 B) :
			A(A), B(B)
		{

		}
	};
	
	struct RegUInt8
	{
		RegisterID A;
		UInt8 B;
		constexpr RegUInt8(RegisterID A, UInt8 B) :
			A(A), B(B)
		{

		}
	};
	struct ValUInt16
	{
		UInt16 A;
		constexpr ValUInt16(UInt16 A) :
			A(A)
		{

		}
	};
	struct ValUInt8
	{
		UInt8 A;
		constexpr ValUInt8(UInt8 A) :
			A(A)
		{

		}
	};
	struct ThreeUInt8
	{
		UInt8 A;
		UInt8 B;
		UInt8 C;
		constexpr ThreeUInt8(UInt8 A, UInt8 B, UInt8 C):
			A(A), B(B),C(C)
		{

		}
	};
	struct NoneOp
	{

	};
	#pragma pack(pop)

	InstructionSet OpCode = InstructionSet::DoNothing;
	union 
	{ 
		NoneOp Op_None;
		ThreeUInt8 Op_ThreeUInt8;

		OneReg Op_OneReg;
		TwoReg Op_TwoReg;
		ThreeReg Op_ThreeReg;

		RegUInt8 Op_RegUInt8;
		RegUInt16 Op_RegUInt16;

		ValUInt8 Op_ValUInt8;
		ValUInt16 Op_ValUInt16;

		TwoRegInt8 Op_TwoRegInt8;
	};
	
	constexpr Instruction() :
		OpCode(InstructionSet::DoNothing),
		Op_ThreeUInt8(0,0,0){}

	constexpr Instruction(InstructionSet OpCode, NoneOp Op) :
		OpCode(OpCode),
		Op_None(Op){}

	constexpr Instruction(InstructionSet OpCode, TwoReg Op) :
		OpCode(OpCode),
		Op_TwoReg(Op){}

	constexpr Instruction(InstructionSet OpCode, ThreeReg Op) :
		OpCode(OpCode),
		Op_ThreeReg(Op){}

	constexpr Instruction(InstructionSet OpCode, RegUInt16 Op) :
		OpCode(OpCode),
		Op_RegUInt16(Op){}

	constexpr Instruction(InstructionSet OpCode, RegUInt8 Op) :
		OpCode(OpCode),
		Op_RegUInt8(Op){}
	
	constexpr Instruction(InstructionSet OpCode, ThreeUInt8 Op) :
		OpCode(OpCode),
		Op_ThreeUInt8(Op){}

	constexpr Instruction(InstructionSet OpCode, ValUInt8 Op) :
		OpCode(OpCode),
		Op_ValUInt8(Op){}

	constexpr Instruction(InstructionSet OpCode, ValUInt16 Op) :
		OpCode(OpCode),
		Op_ValUInt16(Op) {}

	constexpr Instruction(InstructionSet OpCode, OneReg Op) :
		OpCode(OpCode),
		Op_OneReg(Op) {}

	constexpr Instruction(InstructionSet OpCode, TwoRegInt8 Op) :
		OpCode(OpCode),
		Op_TwoRegInt8(Op) {}
};

static_assert(sizeof(Instruction) == 4,"Instruction must be 4 bytes");
UCodeLangEnd
