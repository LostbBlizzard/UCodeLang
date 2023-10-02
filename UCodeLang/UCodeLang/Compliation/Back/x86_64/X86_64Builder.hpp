#pragma once
#include "../x86/X86Builder.hpp"
#include "x86_64Data.hpp"
UCodeLangStart

//resources
//https://en.wikipedia.org/wiki/X86_instruction_listings
//https://www.felixcloutier.com/x86/

class X86_64Builder
{
public:
	using Value8 = NativeCodeBuilder::Value8;
	using Value16 = NativeCodeBuilder::Value16;
	using Value32 = NativeCodeBuilder::Value32;
	using Value64 = NativeCodeBuilder::Value64;

	using Valueu8 = NativeCodeBuilder::Valueu8;
	using Valueu16 = NativeCodeBuilder::Valueu16;
	using Valueu32 = NativeCodeBuilder::Valueu32;
	using Valueu64 = NativeCodeBuilder::Valueu64;


	using GReg = x86_64::GeneralRegisters;
	using FReg = x86_64::FloatingPointRegisters;
	using ModRM = x86_64::ModRM;
	using Rm = x86_64::Rm;
	using IndrReg = x86_64::IndrGeneralRegister;
	X86_64Builder(){}
	~X86_64Builder(){}

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
	

	template<typename T>
	struct Near
	{
		T Value;
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
		T Value;
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

	template<typename T>
	struct Add
	{
		T Value = 0;
		explicit Add(T Offset)
		{
			Value = Offset;
		}
	};

	using Add8 = Add<Value8>;

	using Add16 = Add<Value16>;

	using Add32 = Add<Value32>;

	using Add64 = Add<Value64>;

	using Addu8 = Add<Valueu8>;

	using Addu16 = Add<Valueu16>;

	using Addu32 = Add<Valueu32>;

	using Addu64 = Add<Valueu64>;

	template<typename T>
	struct Sub
	{
		T Value = 0;
		explicit Sub(T Offset)
		{
			Value = Offset;
		}
	};
	using Sub8 = Sub<Value8>;

	using Sub16 =Sub<Value16>;

	using Sub32 =Sub<Value32>;

	using Sub64 =Sub<Value64>;

	using Subu8 =Sub<Valueu8>;

	using Subu16 =Sub<Valueu16>;

	using Subu32 =Sub<Valueu32>;

	using Subu64 =Sub<Valueu64>;

	#define ImportUseing86x64Gen \
	using GReg = X86_64Builder::GReg; \
    using FReg = X86_64Builder::FReg; \
	using ModRM = X86_64Builder::ModRM; \
	using Rm = X86_64Builder::Rm; \
	using IndrReg = X86_64Builder::IndrReg; \
    \
	using Near8 = X86_64Builder::Near8; \
	using Near16 = X86_64Builder::Near16; \
	using Near32 = X86_64Builder::Near32; \
	using Near64 = X86_64Builder::Near64; \
	using Nearu8 = X86_64Builder::Nearu8; \
	using Nearu16 = X86_64Builder::Nearu16; \
	using Nearu32 = X86_64Builder::Nearu32; \
	using Nearu64 = X86_64Builder::Nearu64; \
	\
	using Absolute8 = X86_64Builder::Absolute8; \
	using Absolute16 = X86_64Builder::Absolute16; \
	using Absolute32 = X86_64Builder::Absolute32; \
	using Absolute64 = X86_64Builder::Absolute64; \
	using Absoluteu8 = X86_64Builder::Absoluteu8;  \
	using Absoluteu16 = X86_64Builder::Absoluteu16; \
	using Absoluteu32 = X86_64Builder::Absoluteu32; \
	using Absoluteu64 = X86_64Builder::Absoluteu64; \
	\
	using Add8 = X86_64Builder::Add8; \
	using Add16 = X86_64Builder::Add16; \
	using Add32 = X86_64Builder::Add32; \
	using Add64 = X86_64Builder::Add64; \
	using Addu8 = X86_64Builder::Addu8; \
	using Addu16 = X86_64Builder::Addu16; \
	using Addu32 = X86_64Builder::Addu32; \
	using Addu64 = X86_64Builder::Addu64; \
	\
	using Sub8 = X86_64Builder::Sub8; \
	using Sub16 = X86_64Builder::Sub16; \
	using Sub32 = X86_64Builder::Sub32; \
	using Sub64 = X86_64Builder::Sub64; \
	using Subu8 = X86_64Builder::Subu8; \
	using Subu16 = X86_64Builder::Subu16; \
	using Subu32 = X86_64Builder::Subu32; \
	using Subu64 = X86_64Builder::Subu64; 
	
	//x86_64 instructions



	inline void syscall(){_Base.syscall();}
	inline void ret(){_Base.ret();}
	inline size_t GetIndex() {return _Base.GetIndex(); }
	inline Byte* GetData(size_t offset) { return _Base.GetData(offset); }
	inline Byte* GetData() { return GetData(0); }
	inline size_t Size() const { return _Base.Size(); }

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

	void mov8(GReg dest, GReg src);
	void mov16(GReg dest, GReg src);
	void mov32(GReg dest, GReg src);
	void mov64(GReg dest,GReg src);
	
	void mov64(IndrReg dest, GReg src);
	void mov64(GReg dest,IndrReg src);

	void mov64(GReg dest, IndrReg src, Value8 src_offset);
	void mov64(IndrReg dest, Value8 dest_offset, GReg src);
	/// loads the address of a variable into register
	void lea(GReg dest,IndrReg src,Value8 scale, GReg index, UInt64 disp);
	/* 
	inline  void push8(GReg Reg);
	{
		UCodeLangThrowException("not added");
	}
	*/
	void push16(GReg Reg);

	/* 86_64 does not support.use push64
	inline void push32(GReg Reg)
	{
		UCodeLangThrowException("not added");
	}
	*/

	void push64(GReg Reg);

	void push8(Value8 Value);
	void push16(Value16 Value);
	void push32(Value32 Value);

	void pop8(GReg Reg);
	void pop16(GReg Reg);

	/* 86_64 does not support.use push64
	inline void pop32(GReg Reg)
	{
		UCodeLangThrowException("not added");
	}
	*/

	void pop64(GReg Reg);

	//dest := src + dest;
	void add8(GReg dest, GReg src);
	//dest := src + dest;
	void add16(GReg dest, GReg src);
	//dest := src + dest;
	void add32(GReg dest, GReg src);
	//dest := src + dest;
	void add64(GReg dest, GReg src);

	//dest := src + dest;
	void add32(GReg dest, Value32 src);
	void add64(GReg dest, Value64 src);

	//dest := src - Value;
	void sub32(GReg dest, Value32 Value);
	
	//dest := src - Value;
	void sub64(GReg dest, Value64 Value);
	//dest := src - dest;
	void sub8(GReg dest, GReg src);
	//dest := src - dest;
	void sub16(GReg dest, GReg src);
	//dest := src - dest;
	void sub32(GReg dest, GReg src);
	//dest := src - dest;
	void sub64(GReg dest, GReg src);

	//dest := src XOR dest;
	void XOr8(GReg dest, GReg src);

	void XOr16(GReg dest, GReg src);

	void XOr32(GReg dest, GReg src);

	void XOr64(GReg dest, GReg src);



	X86Builder _Base;//because 86x64 is an extension of x86
};
UCodeLangEnd
