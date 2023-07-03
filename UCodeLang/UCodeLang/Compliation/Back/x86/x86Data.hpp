#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/DataType/BitValue.hpp"
UCodeLangStart
namespace x86
{
	enum class GeneralRegisters : Byte
	{
		EAX = 0,//Accumulator register.  
		EBX = 2,//Base registe
		ECX = 1,//Counter register
		EDX = 3,//Data register

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
		return (Byte)Value;
	}
	inline Byte modrm(GeneralRegisters src, GeneralRegisters dst) {
		return (0b11 << 6) | (x86::RegisterOffset(src) << 3) | x86::RegisterOffset(dst);
	}
		
}
UCodeLangEnd