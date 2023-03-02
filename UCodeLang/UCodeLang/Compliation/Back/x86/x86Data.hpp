#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
UCodeLangStart
namespace x86
{
	enum class GeneralRegisters : Byte
	{
		A,//Accumulator register.  
		B,//Base registe
		C,//Counter register
		D,//Data register

		Null,
	};
	enum class FloatingPointRegisters : Byte
	{
		ST0,
		ST1,
		ST2,
		ST3,
		ST4,
		ST5,
		ST6,
		ST7,
	};
	Byte RegisterOffset(GeneralRegisters Value)
	{
		switch (Value)
		{
		case GeneralRegisters::A:return 0;
		case GeneralRegisters::B:return 2;
		case GeneralRegisters::C:return 1;
		case GeneralRegisters::D:return 3;
		default:return 0;

		}
	}

		
}
UCodeLangEnd