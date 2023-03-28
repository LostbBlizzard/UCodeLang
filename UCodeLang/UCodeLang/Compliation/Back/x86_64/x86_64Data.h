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
	
		rdi,
		r8,
		r9,
		r10,
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

	inline x86::GeneralRegisters To_x86(GeneralRegisters Value)
	{
		switch (Value)
		{
		case GeneralRegisters::A:return x86::GeneralRegisters::A;
		case GeneralRegisters::B:return x86::GeneralRegisters::B;
		case GeneralRegisters::C:return x86::GeneralRegisters::C;
		case GeneralRegisters::D:return x86::GeneralRegisters::D;
		case GeneralRegisters::rdi:return x86::GeneralRegisters::ESI;
		default:return x86::GeneralRegisters::Null;
		}
	}
	
	
	enum ModRM {
		mod = 0xC0,
		spare_register = 0x38,
		rm = 0x07
	};
	
	inline Byte modrm(GeneralRegisters src, GeneralRegisters dst) {
		return (mod) | (x86::RegisterOffset(To_x86(src)) << 3) | x86::RegisterOffset(To_x86(dst));
	}

	inline Byte modrm2(GeneralRegisters src, GeneralRegisters dst) {
		return (rm) | (x86::RegisterOffset(To_x86(src)) << 3) | x86::RegisterOffset(To_x86(dst));
	}
	

}
UCodeLangEnd