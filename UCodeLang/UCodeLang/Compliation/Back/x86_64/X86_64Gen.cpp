#include "X86_64Gen.hpp"
UCodeLangStart
void X86_64Gen::call(Absoluteu32 CallValue)
{
	_Base.PushByte(0x9A);
	_Base.PushValue_t_little_endian(CallValue);
}
void X86_64Gen::r_call(Byte* Output, Absoluteu32 CallValue)
{
	Output[0] = 0x9A;
	CodeGen::SubByte_t_little_endian(&Output[1], CallValue);
}
void X86_64Gen::call(Near32 displacement)
{
	PushByte(0xE8);
	PushValue_t_little_endian(displacement);
}
void X86_64Gen::r_call(Byte* Output, Near32 displacement)
{
	Output[0] = 0xE8;
	CodeGen::SubByte_t_little_endian(&Output[1],displacement);
}
void X86_64Gen::mov(GReg Reg, Value8 Value)
{
	if (x86_64::To_x86(Reg) ==x86::GeneralRegisters::Null){ throw std::exception("not added"); }
	_Base.mov(x86_64::To_x86(Reg), Value);
}
void X86_64Gen::mov(GReg Reg, Value16 Value)
{
	if (x86_64::To_x86(Reg) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }
	_Base.mov(x86_64::To_x86(Reg), Value);
}
void X86_64Gen::mov(GReg Reg, Value32 Value)
{
	if (Reg == GReg::r8)
	{
		PushByte(0x41);
		PushByte(0xB8);
		PushValue_t_little_endian(Value);
	}
	else
	{
		if (x86_64::To_x86(Reg) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }
		_Base.mov(x86_64::To_x86(Reg), Value);
	}
}
void X86_64Gen::mov(GReg Reg, Value64 Value)
{
	PushByte(0x48);
	PushByte(0xb8 + x86::RegisterOffset(x86_64::To_x86(Reg)));
	_Base.PushValue_t_little_endian(Value);

}

void X86_64Gen::lea(ModRM Mod, GReg Reg, Rm rm, Value8 scale, GReg index, UInt64 disp)
{
	PushByte(0x48);
	PushByte(0x8D);

	PushByte(((Byte)Mod << 6) | ((Byte)Reg << 3) | (Byte)rm);

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

void X86_64Gen::mov8(GReg dest, GReg src)
{
	if (x86_64::To_x86(src) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }
	if (x86_64::To_x86(dest) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }

	_Base.mov8(x86_64::To_x86(dest), x86_64::To_x86(src));
}
void X86_64Gen::mov16(GReg dest, GReg src)
{
	if (x86_64::To_x86(src) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }
	if (x86_64::To_x86(dest) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }

	_Base.mov16(x86_64::To_x86(dest),x86_64::To_x86(src));
}
void X86_64Gen::mov32(GReg dest, GReg src)
{
	if (x86_64::To_x86(src) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }
	if (x86_64::To_x86(dest) == x86::GeneralRegisters::Null) { throw std::exception("not added"); }

	_Base.mov32(x86_64::To_x86(dest),x86_64::To_x86(src));
}
void X86_64Gen::move64(GReg dest, GReg src)
{
	if (dest == GReg::RSP && src == GReg::RBX)
	{
		PushByte(0x48);
		PushByte(0x89);
		PushByte(0xE3);
	}
	else if (dest == GReg::RSP && src == GReg::RDX)
	{
		PushByte(0x48);
		PushByte(0x89);
		PushByte(0xE2);
	}
	else 
	{
		PushByte(0x48);
		PushByte(0x89);
		PushByte(x86_64::modrm(src, dest));
	}
}
void X86_64Gen::move64(IndrReg Reg, GReg src)
{
	PushByte(0x48);
	PushByte(0x89);
	PushByte(x86_64::modrm(src, Reg._Reg));
}
void X86_64Gen::move64(GReg dest, IndrReg src)
{
	PushByte(0x48);
	PushByte(0x89);
	PushByte(x86_64::modrm(src._Reg, dest));
}
 void X86_64Gen::push8(GReg Reg)
{
	throw std::exception("not added");
}
 void X86_64Gen::push16(GReg Reg)
{
	throw std::exception("not added");
}
 void X86_64Gen::push32(GReg Reg)
{
	throw std::exception("not added");
}
 void X86_64Gen::push64(GReg Reg)
 {
	 PushByte(0x50 + (Byte)Reg);
 }
 void X86_64Gen::pop8(GReg Reg)
 {
	 throw std::exception("not added");
 }
 void X86_64Gen::pop16(GReg Reg)
 {
	 throw std::exception("not added");
 }
 void X86_64Gen::pop32(GReg Reg)
 {
	 throw std::exception("not added");
 }
 void X86_64Gen::pop64(GReg Reg)
 {
	 PushByte(0x58 + (Byte)Reg);
 }
void X86_64Gen::add8(GReg dest, GReg src)
{
	throw std::exception("not added");
}
void X86_64Gen::add16(GReg dest, GReg src)
{
	throw std::exception("not added");
}
void X86_64Gen::add32(GReg dest, GReg src)
{
	throw std::exception("not added");
}
void X86_64Gen::add64(GReg dest, GReg src)
{
	throw std::exception("not added");
}
void X86_64Gen::sub64(GReg dest, Value64 Value)
{
	if (dest == GReg::RSP)
	{
		PushByte(0x48);
		PushByte(0x81);
		PushByte(0xEC);
		PushValue_t_little_endian(Value);
	}
	else
	{
		throw std::exception("not added");
	}
}
UCodeLangEnd