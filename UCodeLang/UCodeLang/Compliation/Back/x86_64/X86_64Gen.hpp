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



	void Push_Ins_syscall(){_Base.Push_Ins_syscall();}
	void Push_Ins_ret(){_Base.Push_Ins_ret();}

	void Push_Ins_MovImm8(x86::GeneralRegisters Reg, Value8 Value)
	{
		_Base.Push_Ins_MovImm8(Reg, Value);
	}
	void Push_Ins_MovImm16(x86::GeneralRegisters Reg, Value16 Value)
	{
		_Base.Push_Ins_MovImm16(Reg, Value);
	}
	void Push_Ins_MovImm32(x86::GeneralRegisters Reg, Value32 Value)
	{
		_Base.Push_Ins_MovImm32(Reg, Value);
	}

	X86Gen _Base;//because 86x64 is an extension of x86
};
UCodeLangEnd
