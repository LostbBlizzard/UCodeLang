#pragma once
#include "InstructionSet.hpp"
#include "LangTypes.hpp"
UCodeLangStart

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

UCodeLangEnd
