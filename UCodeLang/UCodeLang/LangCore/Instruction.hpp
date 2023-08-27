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

	enum class OpType : UInt8
	{
		NoneOp,
		ThreeUInt8,

		OneReg,
		TwoReg,
		ThreeReg,

		RegUInt8,
		RegUInt16,

		ValUInt8,
		ValUInt16,

		TwoRegInt8,
	};
	static OpType GetOpType(InstructionSet OpCode);

	static Optional<Int32> IsLoad32(const Span<Instruction> Data,size_t I)
	{
		auto& Ins = Data[I];
		if (Data[I].OpCode == InstructionSet::Store32v1)
		{
			if (Data.Size() > I + 1)
			{
				auto& NextIns = Data[I + 1];
				if (NextIns.OpCode == InstructionSet::Store32v2)
				{
					if (Ins.Op_RegUInt16.A == NextIns.Op_RegUInt16.A)
					{
						Int32 V = 0;
						((UInt16*)&V)[0] = Ins.Op_RegUInt16.B;
						((UInt16*)&V)[1] = NextIns.Op_RegUInt16.B;
						return V;
					}

				}
			}
		}
		return {};
	}
	static Optional<float32> IsLoadf32(const Span<Instruction> Data, size_t I)
	{
		auto& Ins = Data[I];
		if (Data[I].OpCode == InstructionSet::Storef32v1)
		{
			if (Data.Size() > I + 1)
			{
				auto& NextIns = Data[I + 1];
				if (NextIns.OpCode == InstructionSet::Storef32v2)
				{
					if (Ins.Op_RegUInt16.A == NextIns.Op_RegUInt16.A)
					{
						float32 V = 0;
						((UInt16*)&V)[0] = Ins.Op_RegUInt16.B;
						((UInt16*)&V)[1] = NextIns.Op_RegUInt16.B;
						return V;
					}

				}
			}
		}
		return {};
	}
	static Optional<Int64> IsLoad64(const Span<Instruction> Data, size_t I)
	{
		auto& Ins = Data[I];
		if (Data[I].OpCode == InstructionSet::Store64v1)
		{
			if (Data.Size() > I + 1)
			{
				auto& NextIns = Data[I + 1];
				if (NextIns.OpCode == InstructionSet::Store64v2)
				{
					if (Data.Size() > I + 2)
					{
						auto& NextIns2 = Data[I + 2];
						if (NextIns2.OpCode == InstructionSet::Store64v3)
						{
							if (Data.Size() > I + 3)
							{
								auto& NextIns3 = Data[I + 3];
								if (NextIns3.OpCode == InstructionSet::Store64v4)
								{
									if (Ins.Op_RegUInt16.A == NextIns.Op_RegUInt16.A
										&& NextIns2.Op_RegUInt16.A == NextIns3.Op_RegUInt16.A
										&& Ins.Op_RegUInt16.A == NextIns3.Op_RegUInt16.A)
									{
										Int64 V = 0;
										((UInt16*)&V)[0] = Ins.Op_RegUInt16.B;
										((UInt16*)&V)[1] = NextIns.Op_RegUInt16.B;
										((UInt16*)&V)[2] = NextIns2.Op_RegUInt16.B;
										((UInt16*)&V)[3] = NextIns3.Op_RegUInt16.B;

										return V;
									}
								}
							}
						}
					}
				}
			}
		}
		return {};
	}
	static Optional<float64> IsLoadf64(const Span<Instruction> Data, size_t I)
	{
		auto& Ins = Data[I];
		if (Data[I].OpCode == InstructionSet::Storef64v1)
		{
			if (Data.Size() > I + 1)
			{
				auto& NextIns = Data[I + 1];
				if (NextIns.OpCode == InstructionSet::Storef64v2)
				{
					if (Data.Size() > I + 2)
					{
						auto& NextIns2 = Data[I + 2];
						if (NextIns2.OpCode == InstructionSet::Storef64v3)
						{
							if (Data.Size() > I + 3)
							{
								auto& NextIns3 = Data[I + 3];
								if (NextIns3.OpCode == InstructionSet::Storef64v4)
								{
									if (Ins.Op_RegUInt16.A == NextIns.Op_RegUInt16.A
										&& NextIns2.Op_RegUInt16.A == NextIns3.Op_RegUInt16.A
										&& Ins.Op_RegUInt16.A == NextIns3.Op_RegUInt16.A)
									{
										float64 V = 0;
										((UInt16*)&V)[0] = Ins.Op_RegUInt16.B;
										((UInt16*)&V)[1] = NextIns.Op_RegUInt16.B;
										((UInt16*)&V)[2] = NextIns2.Op_RegUInt16.B;
										((UInt16*)&V)[3] = NextIns3.Op_RegUInt16.B;

										return V;
									}
								}
							}
						}
					}
				}
			}
		}
		return {};
	}
};

static_assert(sizeof(Instruction) == 4,"Instruction must be 4 bytes");
UCodeLangEnd
