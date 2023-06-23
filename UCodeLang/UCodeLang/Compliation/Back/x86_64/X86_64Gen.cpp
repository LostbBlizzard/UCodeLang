#include "X86_64Gen.hpp"
UCodeLangStart
void X86_64Gen::Push_Ins_CallNear(uint32_t CallValue)
{
	_Base.PushByte(0xFF);
	_Base.PushByte(0x15);
	_Base.PushValue_t_little_endian(CallValue);
}
void X86_64Gen::Sub_Ins_CallNear(Byte* Output, uint32_t CallValue)
{
	Output[0] = 0xFF;
	Output[1] = 0x15;
	CodeGen::SubByte_t_little_endian(&Output[2], CallValue);
}
void X86_64Gen::Push_Ins_MovImm8(GReg Reg, Value8 Value)
{
	_Base.Push_Ins_MovImm8(x86_64::To_x86(Reg), Value);
}
void X86_64Gen::Push_Ins_MovImm16(GReg Reg, Value16 Value)
{
	_Base.Push_Ins_MovImm16(x86_64::To_x86(Reg), Value);
}
void X86_64Gen::Push_Ins_MovImm32(GReg Reg, Value32 Value)
{
	if (Reg == GReg::r8)
	{
		PushByte(0x48);
		PushByte(0xB8);
		PushValue_t_little_endian(Value);
	}
	else
	{
		_Base.Push_Ins_MovImm32(x86_64::To_x86(Reg), Value);
	}
}
void X86_64Gen::Push_Ins_MovImm64(GReg Reg, Value64 Value)
{
	PushByte(0x48);
	PushByte(0xb8 + x86::RegisterOffset(x86_64::To_x86(Reg)));
	_Base.PushValue_t_little_endian(Value);

}

//mov    [reg],reg2

void X86_64Gen::Push_Ins_MovReg64ToPtrdereference(GReg Ptr, GReg reg2)
{
	PushByte(0x48);
	PushByte(0x89);
	PushByte(x86_64::modrm2(reg2, Ptr));

}
void X86_64Gen::Push_Ins_RegToReg8(GReg Reg, GReg OutReg)
{
	_Base.Push_Ins_RegToReg8(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
}
void X86_64Gen::Push_Ins_RegToReg16(GReg Reg, GReg OutReg)
{

	_Base.Push_Ins_RegToReg16(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
}
void X86_64Gen::Push_Ins_RegToReg32(GReg Reg, GReg OutReg)
{
	_Base.Push_Ins_RegToReg32(x86_64::To_x86(Reg), x86_64::To_x86(OutReg));
}
void X86_64Gen::Push_Ins_RegToReg64(GReg Reg, GReg OutReg)
{
	X86Gen_NotAdded
		//_Base.Push_Ins_RegToReg32(x86_64::To_x86(Reg), x86_64::To_x86(Reg));
}
inline void X86_64Gen::Push_Ins_Add8(GReg Reg, GReg Reg2, GReg out)
{
	_Base.Push_Ins_Add8(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
}
inline void X86_64Gen::Push_Ins_Add16(GReg Reg, GReg Reg2, GReg out)
{
	_Base.Push_Ins_Add16(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
}
inline void X86_64Gen::Push_Ins_Add32(GReg Reg, GReg Reg2, GReg out)
{
	_Base.Push_Ins_Add32(x86_64::To_x86(Reg), x86_64::To_x86(Reg2), x86_64::To_x86(out));
}
inline void X86_64Gen::Push_Ins_Add64(GReg Reg, GReg Reg2, GReg out)
{
	X86Gen_NotAdded
		//_Base.Push_Ins_MovImm64(x86_64::To_x86(Reg), Value);
}
UCodeLangEnd