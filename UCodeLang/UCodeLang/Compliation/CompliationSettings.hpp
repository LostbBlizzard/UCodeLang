#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "../LangCore/LangTypes.hpp"
UCodeLangStart
enum class OutPutType : UInt8
{
	Lib,
	DLL,
	IRAndSymbols,
};
typedef UInt8 OptimizationFlags_t;
enum class OptimizationFlags : OptimizationFlags_t
{
	O_None = 0,
	O_1 = 1 << 0,
	O_2 = 1 << 1,
	Debug = 1 << 2,

	O_3 = O_2 | O_1,




	//PreSets
	NoOptimization = O_None,
	ForSize = O_1,
	ForSpeed = O_2,
	ForMaxSpeed = O_3,
	ForDebuging = O_1 | Debug,
};
struct CompliationSettings
{
	OutPutType _Type = OutPutType::Lib;
	OptimizationFlags _Flags = OptimizationFlags::ForDebuging;
	IntSizes PtrSize = IntSizes::Native;
};
UCodeLangEnd