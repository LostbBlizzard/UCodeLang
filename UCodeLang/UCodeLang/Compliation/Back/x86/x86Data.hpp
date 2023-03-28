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

		ESP,
		EBP,
		ESI,
		EDI,

		Count,
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
	inline Byte RegisterOffset(GeneralRegisters Value)
	{
		switch (Value)
		{
		case GeneralRegisters::A:return 0;
		case GeneralRegisters::B:return 2;
		case GeneralRegisters::C:return 1;
		case GeneralRegisters::D:return 3;
		case GeneralRegisters::ESP:return 4;
		case GeneralRegisters::EBP:return 5;
		case GeneralRegisters::ESI:return 6;
		case GeneralRegisters::EDI:return 7;
		default:return 0;

		}
	}
	inline Byte modrm(GeneralRegisters src, GeneralRegisters dst) {
		return (0b11 << 6) | (x86::RegisterOffset(src) << 3) | x86::RegisterOffset(dst);
	}
		
}
UCodeLangEnd