#pragma once
#include "../BackEndInterface.hpp"
#include "x86Data.hpp"
#include "../InsCodeGen.h"
UCodeLangStart

//resources
//https://en.wikipedia.org/wiki/X86_instruction_listings
//https://www.felixcloutier.com/x86/

class X86Gen
{
#define Use86 using namespace x86;
	
public:
	using Value8 = CodeGen::Value8;
	using Value16 = CodeGen::Value16;
	using Value32 = CodeGen::Value32;
	using Value64 = CodeGen::Value64;
	X86Gen() {}
	~X86Gen() {}

	void Reset()
	{

	}
	void Build(const IRBuilder* Input);
	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);

	//
	void PushByte(Byte Value)
	{
		_Output.PushByte(Value);
	}
	void PushByte(const Byte* Value,size_t Size)
	{
		_Output.PushByte(Value, Size);
	}
	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		_Output.PushValue_t_little_endian((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		_Output.PushValue_t_Big_endian((const Byte*)&Value, sizeof(Value));
	}

	//https://www.felixcloutier.com/x86/index.html
	//x86 instructions
	void Push_Ins_syscall()
	{
		Use86
		PushByte(0xf);
		PushByte(0x05);
	}
	void Push_Ins_ret()
	{Use86
		PushByte(0xc3);
	}
	//move imm
	void Push_Ins_MovImm8(x86::GeneralRegisters Reg, Value8 Value)
	{
		Use86
		PushByte(0xb0 + RegisterOffset(Reg));
		PushByte(Value);
	}
	void Push_Ins_MovImm16(x86::GeneralRegisters Reg, Value16 Value)
	{
		Use86
		PushByte(0x66);
		PushByte(0xb9 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}
	void Push_Ins_MovImm32(x86::GeneralRegisters Reg, Value32 Value)
	{
		Use86
		PushByte(0xb8 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}

	//reg to reg
	void Push_Ins_RegToReg8(x86::GeneralRegisters Reg, x86::GeneralRegisters OutReg)
	{
		Use86
		
	}
	void Push_Ins_RegToReg16(x86::GeneralRegisters Reg, x86::GeneralRegisters OutReg)
	{
		Use86

	}
	void Push_Ins_RegToReg32(x86::GeneralRegisters Reg, x86::GeneralRegisters OutReg)
	{
		Use86

	}

	//add

	//sub

	//mult

	//div

	//jump 

	//call

	

	//members
	const IRBuilder* _Input;
	CodeGen _Output;
};
UCodeLangEnd