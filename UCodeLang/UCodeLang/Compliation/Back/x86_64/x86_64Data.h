#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "../x86/x86Data.hpp"
UCodeLangStart
namespace x86_64
{
	
	enum class GeneralRegisters : Byte
	{
		RAX = 0,//Accumulator register.  
		RCX = 1,//Counter register
		RDX = 2,//Data register
		RBX = 3,//Base registe
		
	
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

	struct IndrGeneralRegister
	{
		GeneralRegisters _Reg = GeneralRegisters::Null;
		explicit IndrGeneralRegister(GeneralRegisters V)
		{
			_Reg = V;
		}
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
		RBX = 3,//Base registe
		RCX = 1,//Counter register
		RDX = 2,//Data register

		RSP = 4,
		RBP = 5,
		RSI = 6,
		RDI = 7,

		r8,
		r9,
		r10,
		Null,
	};

	enum class ModRM :Byte
	{
		Indirect = 0x00,
		Direct = 0xC0,
		Disp8 = 0x40,
		Disp32 = 0x80,
	
	};
	
	inline Byte modrm(GeneralRegisters src, GeneralRegisters dst) 
	{
		BitByte Value = BitByte(0);
		Value.SetValue(0, false);

		return ((Byte)ModRM::Direct) | ((GetIndex(src)) << 3) | GetIndex(dst);
	}
	inline Byte modrm(IndrGeneralRegister src, GeneralRegisters dst) {
		return ((Byte)ModRM::Indirect) | ((GetIndex(src._Reg)) << 3) | GetIndex(dst);
	}
	inline Byte modrm(GeneralRegisters src, IndrGeneralRegister dst) {
		return ((Byte)ModRM::Indirect) | ((GetIndex(src)) << 3) | GetIndex(dst._Reg);
	}
	inline Array<Byte,2> modrm(GeneralRegisters src, GeneralRegisters dst,UInt8 offset) 
	{
		Array<Byte, 2> R;
		R[0] = (3 << 6) | ((Byte)src << 3) | (Byte)dst;
		R[1] = offset;
		return R;
	}

	enum class ModRMbyteFields
	{
		MOD_bit_7 = 7,
		MOD_bit_6 = 6,

		Reg_bit_5 = 5,
		Reg_bit_4 = 4,
		Reg_bit_3 = 3,

		RM_bit_2= 2,
		RM_bit_1 = 1,
		RM_bit_0 = 0,
	};
	struct ModRMByte
	{
		BitByte _Base;
	};
}
UCodeLangEnd