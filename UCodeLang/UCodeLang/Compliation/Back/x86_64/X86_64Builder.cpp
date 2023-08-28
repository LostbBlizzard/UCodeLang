#include "X86_64Builder.hpp"
#include "Zydis/Encoder.h"
UCodeLangStart


ZydisRegister_ To(X86_64Builder::GReg Value)
{
	switch (Value)
	{
	case X86_64Builder::GReg::RAX:return ZydisRegister_::ZYDIS_REGISTER_RAX;
	case X86_64Builder::GReg::RBX:return ZydisRegister_::ZYDIS_REGISTER_RBX;
	case X86_64Builder::GReg::RCX:return ZydisRegister_::ZYDIS_REGISTER_RCX;
	case X86_64Builder::GReg::RDX:return ZydisRegister_::ZYDIS_REGISTER_RDX;
	case X86_64Builder::GReg::RSP:return ZydisRegister_::ZYDIS_REGISTER_RSP;
	case X86_64Builder::GReg::RBP:return ZydisRegister_::ZYDIS_REGISTER_RBP;
	case X86_64Builder::GReg::RSI:return ZydisRegister_::ZYDIS_REGISTER_RSI;
	case X86_64Builder::GReg::RDI:return ZydisRegister_::ZYDIS_REGISTER_RDI;
	case X86_64Builder::GReg::r8:return ZydisRegister_::ZYDIS_REGISTER_R8;
	case X86_64Builder::GReg::r9:return ZydisRegister_::ZYDIS_REGISTER_R9;
	case X86_64Builder::GReg::r10:return ZydisRegister_::ZYDIS_REGISTER_R10;
	case X86_64Builder::GReg::r11:return ZydisRegister_::ZYDIS_REGISTER_R11;
	case X86_64Builder::GReg::r12:return ZydisRegister_::ZYDIS_REGISTER_R12;
	case X86_64Builder::GReg::r13:return ZydisRegister_::ZYDIS_REGISTER_R13;
	case X86_64Builder::GReg::r14:return ZydisRegister_::ZYDIS_REGISTER_R14;
	case X86_64Builder::GReg::r15:return ZydisRegister_::ZYDIS_REGISTER_R15;
	default:
		UCodeLangThrowException("Failed to encode instruction");
		break;
	}
}
void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::Value32 Value)
{
	ToUpdate.type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
	ToUpdate.imm.s = Value;
}
void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::Value64 Value)
{
	ToUpdate.type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
	ToUpdate.imm.s = Value;
}
void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::Value8 Value)
{
	As(ToUpdate, (X86_64Builder::Value32)Value);
}
void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::Value16 Value)
{
	As(ToUpdate, (X86_64Builder::Value32)Value);
}
void As(ZydisEncoderOperand& ToUpdate,X86_64Builder::GReg Value)
{
	ToUpdate.type = ZYDIS_OPERAND_TYPE_REGISTER;
	ToUpdate.reg.value = To(Value);
}

void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::IndrReg Value)
{
	ToUpdate.type = ZYDIS_OPERAND_TYPE_POINTER;
	ToUpdate.reg.value = To(Value._Reg);
}
void As(ZydisEncoderOperand& ToUpdate, X86_64Builder::IndrReg Base, X86_64Builder::Value8 scale, X86_64Builder::GReg index, UInt64 disp, ZyanU16 Opsize)
{
	ToUpdate.type = ZYDIS_OPERAND_TYPE_MEMORY;
	ToUpdate.mem.displacement = disp;
	ToUpdate.mem.index = To(index);
	ToUpdate.mem.scale = scale;
	ToUpdate.mem.base = To(Base._Reg);
	ToUpdate.mem.size = Opsize;
}
void X86_64Builder::call(Absoluteu32 CallValue)
{
	_Base.PushByte(0x9A);
	_Base.PushValue_t_little_endian(CallValue);
}
void X86_64Builder::r_call(Byte* Output, Absoluteu32 CallValue)
{
	Output[0] = 0x9A;
	NativeCodeBuilder::SubByte_t_little_endian(&Output[1], CallValue);
}
void X86_64Builder::call(Near32 displacement)
{
	PushByte(0xE8);
	PushValue_t_little_endian(displacement);
}
void X86_64Builder::r_call(Byte* Output, Near32 displacement)
{
	Output[0] = 0xE8;
	NativeCodeBuilder::SubByte_t_little_endian(&Output[1],displacement);
}
thread_local ZydisEncoderRequest req;
void X86_64Builder::mov(GReg Reg, Value8 Value)
{
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	req.operand_count = 2;
	As(req.operands[0], Reg);
	As(req.operands[1], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}
	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov(GReg Reg, Value16 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	req.operand_count = 2;
	As(req.operands[0], Reg);
	As(req.operands[1], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}
	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov(GReg Reg, Value32 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	req.operand_count = 2;
	As(req.operands[0], Reg);
	As(req.operands[1], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}
	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov(GReg Reg, Value64 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0],Reg);
	As(req.operands[1],Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}
	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);

}

void X86_64Builder::mov8(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}
	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov16(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov32(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov64(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov64(IndrReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov64(GReg dest, IndrReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_MOV;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::mov64(GReg dest, IndrReg src, Value8 src_offset)
{
	
}
void X86_64Builder::mov64(IndrReg dest, Value8 dest_offset, GReg src)
{
	
}

/// loads the address of a variable into register

void X86_64Builder::lea(GReg dest, IndrReg src, Value8 scale, GReg index, UInt64 disp)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_LEA;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	//req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0],dest);
	As(req.operands[1],src,scale,index,disp,64);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}

/*
inline  void push8(GReg Reg);
{
UCodeLangThrowException("not added");
}
*/

void X86_64Builder::push16(GReg Reg)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], Reg);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::push64(GReg Reg)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], Reg);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::push8(Value8 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0],Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::push16(Value16 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::push32(Value32 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::pop8(GReg Reg)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0], Reg);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::pop16(GReg Reg)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], Reg);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::pop64(GReg Reg)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_PUSH;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 1;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], Reg);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add8(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add16(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add32(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add64(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add32(GReg dest, Value32 src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::add64(GReg dest, Value64 src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_ADD;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub32(GReg dest, Value32 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub64(GReg dest, Value64 Value)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], Value);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub8(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub16(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub32(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::sub64(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_SUB;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::XOr8(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_XOR;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_8;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}

void X86_64Builder::XOr16(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_XOR;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_16;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}

void X86_64Builder::XOr32(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_XOR;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_32;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
void X86_64Builder::XOr64(GReg dest, GReg src)
{
	
	memset(&req, 0, sizeof(req));

	req.mnemonic = ZYDIS_MNEMONIC_XOR;
	req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	req.operand_count = 2;
	req.operand_size_hint = ZydisOperandSizeHint_::ZYDIS_OPERAND_SIZE_HINT_64;
	As(req.operands[0], dest);
	As(req.operands[1], src);

	ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
	ZyanUSize encoded_length = sizeof(encoded_instruction);

	if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
	{
		UCodeLangThrowException("Failed to encode instruction");
	}

	_Base.PushByte((const Byte*)&encoded_instruction, encoded_length);
}
UCodeLangEnd