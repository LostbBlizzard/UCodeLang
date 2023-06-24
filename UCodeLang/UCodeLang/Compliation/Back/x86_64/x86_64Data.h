#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "../x86/x86Data.hpp"
UCodeLangStart
namespace x86_64
{
	
	enum class GeneralRegisters : Byte
	{
		RAX = 0,//Accumulator register.  
		RBX = 2,//Base registe
		RCX = 1,//Counter register
		RDX = 3,//Data register
	
		RSP = 4,
		RBP = 5,
		RSI = 6,
		RDI = 7,

		r8,
		r9,
		r10,

		Count,
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
	inline Byte GetIndex(GeneralRegisters Reg)
	{
		return (Byte)Reg;
	}

	inline x86::GeneralRegisters To_x86(GeneralRegisters Value)
	{
		switch (Value)
		{
		case GeneralRegisters::RAX:return x86::GeneralRegisters::EAX;
		case GeneralRegisters::RBX:return x86::GeneralRegisters::EBX;
		case GeneralRegisters::RCX:return x86::GeneralRegisters::ECX;
		case GeneralRegisters::RDX:return x86::GeneralRegisters::EDX;
		case GeneralRegisters::RSP:return x86::GeneralRegisters::ESP;
		case GeneralRegisters::RBP:return x86::GeneralRegisters::EBP;
		case GeneralRegisters::RSI:return x86::GeneralRegisters::ESI;
		case GeneralRegisters::RDI:return x86::GeneralRegisters::EDI;
		default:return x86::GeneralRegisters::Null;
		}
	}
	
	enum class Rm : Byte
	{
		RAX = 0,//Accumulator register.  
		RBX = 2,//Base registe
		RCX = 1,//Counter register
		RDX = 3,//Data register

		RSP = 4,
		RBP = 5,
		RSI = 6,
		RDI = 7,

		r8,
		r9,
		r10,
		Null,
	};

	enum ModRM 
	{
		Indirect = 0x00,
		Direct = 0xC0,
		Disp8 = 0x40,
		Disp32 = 0x80
	};
	struct ModRMByte
	{
		Byte _Base;
	};
	inline Byte modrm(GeneralRegisters src, GeneralRegisters dst) {
		return (Direct) | ((GetIndex(src)) << 3) | GetIndex(dst);
	}

}
UCodeLangEnd