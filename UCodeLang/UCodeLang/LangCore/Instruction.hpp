#pragma once
#include "InstructionSet.hpp"
UCodeLangStart

struct Instruction
{
	Instruction() :
		OpCode(InstructionSet::DoNothing)
		,Value0_AsUMaxSizeInt(NullUInt64)
		,Value1_AsUMaxSizeInt(NullUInt64)
	{

	}
	union
	{
		InstructionSet OpCode;
		InstructionSet_t OpCode_AsInt;
	};
	union 
	{
		UInt64 Value0;

		Int8  Value0_AsInt8;
		Int16 Value0_AsInt16;
		Int32 Value0_AsInt32;
		Int64 Value0_AsInt64;

		UInt8  Value0_AsUInt8;
		UInt16 Value0_AsUInt16;
		UInt32 Value0_AsUInt32;
		UInt64 Value0_AsUInt64;

		UIntNative Value0_AsUIntNative;
		SIntNative Value0_AsSIntNative;

		PtrType Value0_AsPtr;
		UAddress Value0_AsAddress;
		RegisterID Value0_RegisterID;
		
		UInt64 Value0_AsUMaxSizeInt;
	};
	union
	{
		UInt64 Value1;

		Int8  Value1_AsInt8;
		Int16 Value1_AsInt16;
		Int32 Value1_AsInt32;
		Int64 Value1_AsInt64;

		UInt8  Value1_AsUInt8;
		UInt16 Value1_AsUInt16;
		UInt32 Value1_AsUInt32;
		UInt64 Value1_AsUInt64;

		UIntNative Value1_AsUIntNative;
		SIntNative Value1_AsSIntNative;
		PtrType Value1_AsPtr;
		UAddress Value1_AsAddress;
		RegisterID Value1_RegisterID;

		UInt64 Value1_AsUMaxSizeInt;
	};
};

UCodeLangEnd
