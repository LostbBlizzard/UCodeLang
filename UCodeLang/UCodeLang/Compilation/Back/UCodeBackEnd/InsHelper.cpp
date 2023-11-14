#pragma once

#ifndef UCodeLangNoCompiler
#include "InsHelper.hpp"
#include "../../Helpers/InstructionBuilder.hpp"
UCodeLangStart


bool InsHelper::ReadReg(Instruction& Op, RegisterID A)
{
	switch (Op.OpCode)
	{

	default:
		UCodeLangUnreachable();;
		break;
	}
	return false;
}
bool InsHelper::WriteReg(Instruction& Op, RegisterID A)
{
	switch (Op.OpCode)
	{
	case InstructionSet::Debug_FuncStart:
	case InstructionSet::Debug_FuncEnd:
	case InstructionSet::Debug_LineEnter:
	case InstructionSet::DoNothing:return false;
	case InstructionSet::bitwiseXor32:return Op.Op_ThreeReg.C == A;
	default:
		UCodeLangUnreachable();;
		break;
	}

	return false;
}

bool InsHelper::ReadPtrReg(Instruction& Op, RegisterID A)
{
	switch (Op.OpCode)
	{

	default:
		UCodeLangUnreachable();;
		break;
	}

	return false;
}
bool InsHelper::WritePtrReg(Instruction& Op, RegisterID A)
{
	switch (Op.OpCode)
	{

	default:
		UCodeLangUnreachable();
		break;
	}

	return false;
}

void InsHelper::SetWriteReg(Instruction& Op, RegisterID A)
{
	switch (Op.OpCode)
	{
	case InstructionSet::bitwiseXor32:
		InstructionBuilder::bitwise_Xor32(Op, Op.Op_ThreeReg.A,Op.Op_ThreeReg.B, A);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

UCodeLangEnd

#endif
