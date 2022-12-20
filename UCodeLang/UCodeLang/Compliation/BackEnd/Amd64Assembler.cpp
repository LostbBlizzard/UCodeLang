#include "Amd64Assembler.hpp"

UCodeLangStart

class  AMD64Set
{
public:
	static constexpr UInt8 ret = 0xC3;
	static constexpr UInt8 Mov_reg8_imm8 = 0xB0;
	static constexpr UInt8 NoOp = 0x90;

	static constexpr UInt8 push_rbp = 0x55;
	static constexpr UInt8 Mov = 0x48;


	//Regers
	static constexpr UInt8 rax = 0;
	static constexpr UInt8 rbx = 3;
	static constexpr UInt8 rcx = 1;
	static constexpr UInt8 rdx = 2;
	static constexpr UInt8 rsx = 0;//Stack pointer

	static constexpr UInt8 r8 = 0;
	static constexpr UInt8 r9 = 0;

	//RegesrMap
	static constexpr UInt8 AToReg = rdx;
	static constexpr UInt8 BToReg = rbx;
	static constexpr UInt8 CToReg = rcx;
	static constexpr UInt8 DToReg = rax;
	static constexpr UInt8 EToReg = r8;
	static constexpr UInt8 FToReg = r9;
}; 
UInt8 GetURegToCPUReg(RegisterID V)
{
	switch (V)
	{
	case UCodeLang::RegisterID::A:return AMD64Set::AToReg;
	case UCodeLang::RegisterID::B:return AMD64Set::BToReg;
	case UCodeLang::RegisterID::C:return AMD64Set::CToReg;
	case UCodeLang::RegisterID::D:return AMD64Set::DToReg;
	case UCodeLang::RegisterID::E:return AMD64Set::EToReg;
	case UCodeLang::RegisterID::F:return AMD64Set::FToReg;
	default:return AMD64Set::DToReg;
	}
}



UInt8 GetMoveIns(UInt8 Re)
{
	return AMD64Set::Mov + Re;
}
UInt8 GetMoveIns(RegisterID V)
{
	return GetMoveIns(GetURegToCPUReg(V));
}


UInt8 GetMoveimm8(UInt8 Re)
{
	return AMD64Set::Mov_reg8_imm8 + Re;
}
UInt8 GetMoveimm8(RegisterID V)
{
	return GetMoveimm8(GetURegToCPUReg(V));
}

void Amd64Assembler::Reset()
{
	NullCalls.clear();
}

void SomeStatiFunc()
{
	
}
bool Amd64Assembler::BuildFunc(const Vector<Instruction>& Data, size_t Offset, Vector<UInt8>& Output)
{
	Reset();
	const Instruction* Ptr = &Data[Offset];
	
	Output.push_back(0x55);    // push rbp

	Output.push_back(AMD64Set::Mov);
	Output.push_back(0x89); 
	Output.push_back(0xe5);   // mov	rbp, rsp

	//Body
	for (size_t i = 0; i < Data.size(); i++)
	{
		const Instruction& Ins = Ptr[i];

		switch (Ins.OpCode)
		{
		case InstructionSet::Store8:
			Output.push_back(GetMoveimm8(Ins.Value1.AsRegister));
			Output.push_back(Ins.Value0.AsUInt8);
			break;
		case InstructionSet::Call: 
		{
			auto V = BuildTepCall(Output);

			NullCalls.push_back({ Offset + i,V });
		}break;
		case InstructionSet::DoNothing:
			Output.push_back(AMD64Set::NoOp);
			break;
		case InstructionSet::Return:
			goto FuncEnd;
			return true;
		default:
			return false;
		}

	}
	FuncEnd:
	//

	Output.push_back(0x5d);
	Output.push_back(AMD64Set::ret);

	return true;
}
void Amd64Assembler::SubCall(VoidFunc FuncOffset, size_t offset, Vector<UInt8>& Output)
{
	UInt8* Nbyte = &Output[offset];
	size_t Tepoffset = offset;

	Output[offset] = 0x48;offset++;

	Output[offset] = 0xb8;// movabs rax, <function_address>
	offset++;
	
	SubBytes(FuncOffset, offset, Output);
	offset += sizeof(VoidFunc);

	Output[offset] =0xff;
	offset++;
	
	
	Output[offset] = 0xd0; // call rax
	offset++;
}

size_t Amd64Assembler::BuildTepCall(Vector<UInt8>& Output)
{
	size_t r = Output.size();

	Output.push_back(AMD64Set::NoOp);
	Output.push_back(AMD64Set::NoOp);// movabs rax, <function_address>


	for (size_t i = 0; i < sizeof(VoidFunc); i++)
	{
		Output.push_back(AMD64Set::NoOp);
	}

	Output.push_back(AMD64Set::NoOp); Output.push_back(AMD64Set::NoOp); // call rax

	return r;
}


UCodeLangEnd