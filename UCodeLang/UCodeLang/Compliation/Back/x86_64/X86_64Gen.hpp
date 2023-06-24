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
	using ModRM = x86_64::ModRM;
	using Rm = x86_64::Rm;
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
		_Base._Output.PushByte_little_endian((const Byte*)&Value, sizeof(Value));
	}
	

	//x86_64 instructions



	inline void Push_Ins_syscall(){_Base.Push_Ins_syscall();}
	inline void Push_Ins_ret(){_Base.Push_Ins_ret();}
	inline size_t GetIndex() {return _Base.GetIndex(); }
	inline Byte* GetData(size_t offset) { return _Base.GetData(offset); }

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

	void Push_Ins_CallNear(uint32_t  CallValue);
	static void Sub_Ins_CallNear(Byte* Output, uint32_t  CallValue);




	inline void Push_Ins_Callptr(uint64_t  CallValue, GReg freeReg = GReg::RAX)
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
		_Base.PushByte(0xd0 + x86_64::GetIndex(Value));
	}
	//
	
	void Push_Ins_MovImm8(GReg Reg, Value8 Value);
	void Push_Ins_MovImm16(GReg Reg, Value16 Value);
	void Push_Ins_MovImm32(GReg Reg, Value32 Value);
	void Push_Ins_MovImm64(GReg Reg, Value64 Value);

	void Push_Ins_SubRspImm32(Value32 Value)
	{
		PushByte(0x48);
		PushByte(0x81);
		PushByte(0xEC);
		PushValue_t_little_endian(Value);
	}
	

	/// loads the address of a variable into register
	///
	/// 
	void Push_Ins_LEA(ModRM Mod,GReg Reg,Rm rm,Value8 scale, GReg index,UInt64 disp)
	{
		PushByte(0x48);
		PushByte(0x8D);
		
		PushByte((Mod << 6) | ((Byte)Reg << 3) | (Byte)rm);

		if (rm == Rm::RSP && index != GReg::RSP)
		{
			PushByte((scale << 6) | ((Byte)index << 3) | (Byte)Rm::RSP);
			PushValue_t_little_endian(disp);
		}
		else
		{
			PushValue_t_little_endian(disp);

		}
	}

	//mov    [reg],reg2
	void Push_Ins_MovReg64ToPtrdereference(GReg Ptr, GReg reg2);

	void Push_Ins_RegToReg8(GReg Reg, GReg OutReg);
	void Push_Ins_RegToReg16(GReg Reg, GReg OutReg);
	void Push_Ins_RegToReg32(GReg Reg, GReg OutReg);
	void Push_Ins_RegToReg64(GReg Reg, GReg OutReg);


	void Push_Ins_Push8(GReg Reg)
	{

	}
	void Push_Ins_Push16(GReg Reg)
	{
		
	}
	void Push_Ins_Push32(GReg Reg)
	{
		
	}
	void Push_Ins_Push64(GReg Reg)
	{
		PushByte(0x50 + (Byte)Reg);
	}


	void Push_Ins_Pop8(GReg Reg)
	{
		
	}
	void Push_Ins_Pop16(GReg Reg)
	{
	}
	void Push_Ins_Pop32(GReg Reg)
	{
		
	}
	void Push_Ins_Pop64(GReg Reg)
	{
		PushByte(0x58 + (Byte)Reg);
	}

	void Push_Ins_Add8(GReg Reg, GReg Reg2, GReg out);
	void Push_Ins_Add16(GReg Reg, GReg Reg2, GReg out);
	void Push_Ins_Add32(GReg Reg, GReg Reg2, GReg out);
	void Push_Ins_Add64(GReg Reg, GReg Reg2, GReg out);

	X86Gen _Base;//because 86x64 is an extension of x86
};
UCodeLangEnd
