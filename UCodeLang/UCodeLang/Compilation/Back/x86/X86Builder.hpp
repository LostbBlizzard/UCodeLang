#pragma once
#include "../BackEndInterface.hpp"
#include "x86Data.hpp"
#include "../NativeCodeBuilder.hpp"
UCodeLangStart

//resources
//https://en.wikipedia.org/wiki/X86_instruction_listings
//https://www.felixcloutier.com/x86/

class X86Builder
{
#define Use86 using namespace x86;
#define X86Gen_NotAdded UCodeLangThrowException("not added");
public:
	enum class GeneralRegisterDataState
	{
		notUsed,
		HasIRValue,
	};
	struct GeneralRegisterData
	{
		GeneralRegisterDataState State = GeneralRegisterDataState::notUsed;
		#ifndef UCodeLangNoCompiler
		IRInstruction* HasValue = nullptr;
		#endif
	};
	using Value8 = NativeCodeBuilder::Value8;
	using Value16 = NativeCodeBuilder::Value16;
	using Value32 = NativeCodeBuilder::Value32;
	using Value64 = NativeCodeBuilder::Value64;

	using GReg = x86::GeneralRegisters;
	//
	X86Builder() {}
	~X86Builder() {}
	//members
	NativeCodeBuilder _Output;

	void Reset()
	{

	}

	//
	void PushByte(Byte Value)
	{
		_Output.PushByte(Value);
	}
	void PushByte(const Byte* Value, size_t Size)
	{
		_Output.PushByte(Value, Size);
	}
	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		_Output.PushValue_t_little_endian(Value);
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		_Output.PushValue_t_Big_endian(Value);
	}

	//https://www.felixcloutier.com/x86/index.html
	//x86 instructions
	
	inline size_t GetIndex() { return _Output.GetIndex(); }
	inline Byte* GetData(size_t offset) { return _Output.ByteOutput.data() + offset; }
	inline Byte* GetData() { return GetData(0); }
	inline size_t Size() const { return _Output.ByteOutput.size(); }
	//move
	inline void mov(GReg Reg, Value8 Value)
	{
		Use86
		PushByte(0xb0 + RegisterOffset(Reg));
		PushByte(Value);
	}
	inline void mov(GReg Reg, Value16 Value)
	{
		Use86
		PushByte(0x66);
		PushByte(0xb9 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}
	inline void mov(GReg Reg, Value32 Value)
	{
		Use86
			PushByte(0xb8 + RegisterOffset(Reg));
		PushValue_t_little_endian(Value);
	}

	inline void mov8(GReg dest, GReg src)
	{
		Use86
		PushByte(0x88);
		PushByte(modrm(src,dest));
	}
	inline void mov16(GReg dest, GReg src)
	{
		Use86
		PushByte(0x66);
		PushByte(0x89);
		PushByte(modrm(src, dest));
	}
	inline void mov32(GReg dest, GReg src)
	{
		Use86
		PushByte(0x89);
		PushByte(modrm(src, dest));
	}

	//add

	//dest := src + dest;
	inline void add8(GReg dest, GReg src)
	{
		Use86
		PushByte(0x00);
		PushByte(modrm(src, dest));
	}

	//dest := src + dest;
	inline void add16(GReg dest, GReg src)
	{
		Use86
		PushByte(0x66);
		PushByte(0x01);
		PushByte(modrm(src, dest));
	}

	//dest := src + dest;
	inline void add32(GReg dest, GReg src)
	{
		Use86
		PushByte(0x01);
		PushByte(modrm(src, dest));
	}
	

	//control flow
	inline void syscall()
	{
		Use86
		PushByte(0xf);
		PushByte(0x05);
	}
	inline void ret()
	{
		Use86
		PushByte(0xc3);
	}
};

UCodeLangEnd