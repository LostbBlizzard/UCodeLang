#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "../x86/x86Data.hpp"
UCodeLangStart
namespace x86_64
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
		xmm0,
		xmm1,
		xmm2,
		xmm3,
		xmm4,
		xmm5,
		xmm6,
		xmm7,
		xmm8,
		xmm9,
		xmm10,
		xmm11,
		xmm12,
		xmm13,
		xmm14,
		xmm15,
	};

	x86::GeneralRegisters To_x86(GeneralRegisters Value)
	{
		switch (Value)
		{
		case GeneralRegisters::A:return x86::GeneralRegisters::A;
		case GeneralRegisters::B:return x86::GeneralRegisters::B;
		case GeneralRegisters::C:return x86::GeneralRegisters::C;
		case GeneralRegisters::D:return x86::GeneralRegisters::D;
		default:return x86::GeneralRegisters::Null;
		}
	}

}
UCodeLangEnd