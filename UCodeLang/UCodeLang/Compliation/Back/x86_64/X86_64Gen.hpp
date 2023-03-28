#pragma once
#include "../x86/X86Gen.hpp"
#include "x86_64Data.h"
UCodeLangStart

//resources
//https://en.wikipedia.org/wiki/X86_instruction_listings
//https://www.felixcloutier.com/x86/

class X86_64Gen
{
public:
	using Value8 = CodeGen::Value8;
	using Value16 = CodeGen::Value16;
	using Value32 = CodeGen::Value32;
	using Value64 = CodeGen::Value64;

	using GReg = x86_64::GeneralRegisters;
	X86_64Gen(){}
	~X86_64Gen(){}

	//
	void PushByte(Byte Value)
	{
		_Base._Output.PushByte(Value);
	}
	void PushByte(const Byte* Value, size_t Size)
	{
		_Base._Output.PushByte(Value, Size);
	}
	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		_Base._Output.PushValue_t_little_endian((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		_Output.PushValue_t_Big_endian((const Byte*)&Value, sizeof(Value));
	}

	//x86_64 instructions



	inline void Push_Ins_syscall(){_Base.Push_Ins_syscall();}
	inline void Push_Ins_ret(){_Base.Push_Ins_ret();}
	inline size_t GetIndex() {return _Base.GetIndex(); }

	inline void Push_Ins_CallAbsolute(uint32_t CallValue)
	{
		_Base.PushByte(0x9A);
		_Base.PushValue_t_little_endian(CallValue);
	}
 	static inline void Sub_Ins_CallAbsolute(Byte* Output, uint32_t CallValue)
	{
		Output[0] = 0x9A;
		CodeGen::SubByte_t_little_endian(&Output[1],CallValue);
	}

	inline void Push_Ins_CallNear(uint32_t  CallValue)
	{
		_Base.PushByte(0xFF);
		_Base.PushByte(0x15);
		_Base.PushValue_t_little_endian(CallValue);
	}
	static inline void Sub_Ins_CallNear(Byte* Output, uint32_t  CallValue)
	{
		Output[0] = 0xFF;
		Output[1] = 0x15;
		CodeGen::SubByte_t_little_endian(&Output[2], CallValue);
	}




	inline void Push_Ins_Callptr(uint64_t  CallValue, GReg freeReg = GReg::A)
	{
		Push_Ins_MovImm64(freeReg,CallValue);
		Push_Ins_CallFuncPtr(freeReg);
	}
	static inline void Sub_Ins_Callptr(Byte* Output, uint64_t  CallValue)
	{
		CodeGen::SubByte_t_little_endian(&Output[2], CallValue);
	}

	inline void Push_Ins_CallFuncPtr(GReg Value)
	{
		_Base.PushByte(0xFF);
		_Base.PushByte(0xd0 + x86::RegisterOffset(x86_64::To_x86(Value)));
	}
	//
	
	inline void Push_Ins_MovImm8(GReg Reg, Value8 Value)
	{
		_Base.Push_Ins_MovImm8(x86_64::To_x86(Reg), Value);
	}
	inline void Push_Ins_MovImm16(GReg Reg, Value16 Value)
	{
		_Base.Push_Ins_MovImm16(x86_64::To_x86(Reg), Value);
	}
	inline void Push_Ins_MovImm32(GReg Reg, Value32 Value)
	{
		_Base.Push_Ins_MovImm32(x86_64::To_x86(Reg), Value);
	}
	inline void Push_Ins_MovImm64(GReg Reg, Value64 Value)
	{
		PushByte(0x48);
		PushByte(0xb8 +x86::RegisterOffset(x86_64::To_x86(Reg)));
		_Base.PushValue_t_little_endian(Value);

	}



	//mov    [reg],reg2
	inline void Push_Ins_MovReg64ToPtrdereference(GReg Ptr, GReg reg2)
	{
		PushByte(0x48);
		PushByte(0x89);
		PushByte(x86_64::modrm2(reg2, Ptr));

	}

	inline void Push_Ins_RegToReg8(GReg Reg, GReg OutReg)
	{
		_Base.Push_Ins_RegToReg8(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
	}
	inline void Push_Ins_RegToReg16(GReg Reg, GReg OutReg)
	{

		_Base.Push_Ins_RegToReg16(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
	}
	inline void Push_Ins_RegToReg32(GReg Reg, GReg OutReg)
	{
		_Base.Push_Ins_RegToReg32(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
	}
	inline void Push_Ins_RegToReg64(GReg Reg, GReg OutReg)
	{
		X86Gen_NotAdded
		//_Base.Push_Ins_RegToReg32(x86_64::To_x86(Reg), x86_64::To_x86(Reg));
	}





	inline void Push_Ins_Add8(GReg Reg, GReg Reg2, GReg out)
	{
		_Base.Push_Ins_Add8(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
	}
	inline void Push_Ins_Add16(GReg Reg, GReg Reg2, GReg out)
	{
		_Base.Push_Ins_Add16(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
	}
	inline void Push_Ins_Add32(GReg Reg, GReg Reg2, GReg out)
	{
		_Base.Push_Ins_Add32(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
	}
	inline void Push_Ins_Add64(GReg Reg, GReg Reg2, GReg out)
	{
		X86Gen_NotAdded
			//_Base.Push_Ins_MovImm64(x86_64::To_x86(Reg), Value);
	}

	X86Gen _Base;//because 86x64 is an extension of x86
};
UCodeLangEnd
