#include "Amd64Assembler.hpp"

UCodeLangStart

class  AMD64Set
{
public:
	static constexpr UInt8 ret = 0xC3;
	static constexpr UInt8 Mov_reg8_imm8 = 0xB0;
}; 
void Amd64Assembler::Reset()
{
}

bool Amd64Assembler::BuildFunc(const Vector<Instruction>& Data, size_t Offset, Vector<UInt8>& Output)
{
	Reset();
	const Instruction* Ptr = &Data[Offset];
	
	for (size_t i = 0; i < Data.size(); i++)
	{
		const Instruction& Ins = Ptr[i];

		switch (Ins.OpCode)
		{
		case InstructionSet::Store8:
			Output.push_back(AMD64Set::Mov_reg8_imm8);
			Output.push_back(Ins.Value0.AsUInt8);
			break;
		case InstructionSet::DoNothing:
			break;
		case InstructionSet::Return:
			Output.push_back(AMD64Set::ret);
			return true;
			break;
		default:
			return false;
			break;
		}

	}

	return false;
}

UCodeLangEnd