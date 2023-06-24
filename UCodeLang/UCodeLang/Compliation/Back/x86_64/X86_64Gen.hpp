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

	using Valueu8 = CodeGen::Valueu8;
	using Valueu16 = CodeGen::Valueu16;
	using Valueu32 = CodeGen::Valueu32;
	using Valueu64 = CodeGen::Valueu64;


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
	
	struct IndrReg
	{
		GReg _Reg = GReg::Null;
		explicit IndrReg(GReg V)
		{
			_Reg = V;
		}
	};

	template<typename T>
	struct Near
	{
		T Value = 0;
		explicit Near(T Offset)
		{
			Value = Offset;
		}
	};

	using Near8 = Near<Value8>;

	using Near16 = Near<Value16>;

	using Near32 = Near<Value32>;

	using Near64 = Near<Value64>;

	using Nearu8 = Near<Valueu8>;

	using Nearu16 = Near<Valueu16>;

	using Nearu32 = Near<Valueu32>;

	using Nearu64 = Near<Valueu64>;

	template<typename T>
	struct Absolute
	{
		T Value = 0;
		explicit Absolute(T Offset)
		{
			Value = Offset;
		}
	};
	using Absolute8 = Absolute<Value8>;

	using Absolute16 = Absolute<Value16>;

	using Absolute32 = Absolute<Value32>;

	using Absolute64 = Absolute<Value64>;

	using Absoluteu8 = Absolute<Valueu8>;

	using Absoluteu16 = Absolute<Valueu16>;

	using Absoluteu32 = Absolute<Valueu32>;

	using Absoluteu64 = Absolute<Valueu64>;
	//x86_64 instructions



	inline void syscall(){_Base.syscall();}
	inline void ret(){_Base.ret();}
	inline size_t GetIndex() {return _Base.GetIndex(); }
	inline Byte* GetData(size_t offset) { return _Base.GetData(offset); }

	void call(Absoluteu32 CallValue);
	static void r_call(Byte* Output, Absoluteu32 CallValue);

	void call(Near32 displacement);
	static void r_call(Byte* Output, Near32 displacement);

	inline void call(GReg Value)
	{
		_Base.PushByte(0xFF);
		_Base.PushByte(0xd0 + x86_64::GetIndex(Value));
	}
	//
	
	void mov(GReg Reg, Value8 Value);
	void mov(GReg Reg, Value16 Value);
	void mov(GReg Reg, Value32 Value);
	void mov(GReg Reg, Value64 Value);

	
	

	/// loads the address of a variable into register
	void lea(ModRM Mod, GReg Reg, Rm rm, Value8 scale, GReg index, UInt64 disp);

	void mov8(GReg dest, GReg src);
	void mov16(GReg dest, GReg src);
	void mov32(GReg dest, GReg src);
	void move64(GReg dest,GReg src);
	
	void move64(IndrReg dest, GReg src);
	void move64(GReg dest,IndrReg src);

	void push8(GReg Reg);
	void push16(GReg Reg);
	void push32(GReg Reg);
	void push64(GReg Reg);


	void pop8(GReg Reg);
	void pop16(GReg Reg);
	void pop32(GReg Reg);
	void pop64(GReg Reg);

	//dest := src + dest;
	void add8(GReg dest, GReg src);
	//dest := src + dest;
	void add16(GReg dest, GReg src);
	//dest := src + dest;
	void add32(GReg dest, GReg src);
	//dest := src + dest;
	void add64(GReg dest, GReg src);

	//dest := src - Value;
	void sub64(GReg dest, Value64 Value);

	X86Gen _Base;//because 86x64 is an extension of x86
};
UCodeLangEnd
