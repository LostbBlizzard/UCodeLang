#include "Amd64Assembler.hpp"

UCodeLangStart
#define IncIns() i++; Index = i;
//https://defuse.ca/online-x86-assembler.htm#disassembly
//https://godbolt.org/
class  AMD64Set
{
public:
	static constexpr UInt8 ret = 0xC3;
	static constexpr UInt8 Mov_reg8_imm8 = 0xB0;
	static constexpr UInt8 NoOp = 0x90;

	static constexpr UInt8 push_rbp = 0x55;

	

	static constexpr UInt8 Mov = 0x48;
	static constexpr UInt8 Push_reg16 = 0x50;
	static constexpr UInt8 Pop_reg16 = 0x58;

	//Register
	static constexpr UInt8 rax = 0;
	static constexpr UInt8 rbx = 3;
	static constexpr UInt8 rcx = 1;
	static constexpr UInt8 rdx = 2;
	static constexpr UInt8 rsx = 0;//Stack pointer

	static constexpr UInt8 r8 = 0;
	static constexpr UInt8 r9 = 0;

	//Register Map
	static constexpr UInt8 AToReg = rdx;
	static constexpr UInt8 BToReg = rbx;
	static constexpr UInt8 CToReg = rcx;
	static constexpr UInt8 DToReg = rax;
	static constexpr UInt8 EToReg = r8;
	static constexpr UInt8 FToReg = r9;

	static constexpr UInt8 AToReg16 = rdx;
	static constexpr UInt8 BToReg16 = rbx;
	static constexpr UInt8 CToReg16 = rcx;
	static constexpr UInt8 DToReg16 = rax;
	static constexpr UInt8 EToReg16 = r8;
	static constexpr UInt8 FToReg16 = r9;
}; 
UInt8 GetURegToCPUReg8(RegisterID V)
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



UInt8 GetMove8Ins(UInt8 Re)
{
	return AMD64Set::Mov + Re;
}
UInt8 GetMove8Ins(RegisterID V)
{
	return GetMove8Ins(GetURegToCPUReg8(V));
}


UInt8 GetMoveimm8(UInt8 Re)
{
	return AMD64Set::Mov_reg8_imm8 + Re;
}
UInt8 GetMoveimm8(RegisterID V)
{
	return GetMoveimm8(GetURegToCPUReg8(V));
}

UInt8 GetPushReg16(UInt8 Re)
{
	return AMD64Set::Push_reg16 + Re;
}
UInt8 GetPushReg16(RegisterID V)
{
	return GetPushReg16(GetURegToCPUReg8(V));
}

UInt8 GetPopReg16(UInt8 Re)
{
	return AMD64Set::Pop_reg16 + Re;
}
UInt8 GetPopReg16(RegisterID V)
{
	return GetPopReg16(GetURegToCPUReg8(V));
}

void Amd64Assembler::Reset()
{
	NullCalls.clear();
	_Output = nullptr;
	_Data = nullptr;
	Index = 0;
	_Offset = 0;
}

UInt8 SomeStatiFunc()
{
	return 8;
}
bool Amd64Assembler::BuildFunc(const Vector<Instruction>& Data, size_t Offset, Vector<UInt8>& Output)
{
	return false;
	SomeStatiFunc();
	Reset();

	_Data = &Data;
	_Output = &Output;


	const Instruction* Ptr = &Data[Offset];
	_Offset = Offset;
	
	BuildFuncStart();
	//Body
	for (size_t i = 0; i < Data.size(); i++)
	{
		const Instruction& Ins = Ptr[i];
		Index = i;

		switch (Ins.OpCode)
		{
		case InstructionSet::Store8:
		{
			auto NIns = NextIns();
			if (NIns)
			{
				if (NIns->OpCode == InstructionSet::Store8RegOnStack)
				{
					_Output->push_back(0xC6);
					_Output->push_back(0x45);
					_Output->push_back(0x01 + NIns->Value1.AsUInt8);
					_Output->push_back(Ins.Value1.AsUInt8);
					i++;
					continue;
				}
				else if (NIns->OpCode == InstructionSet::Push8)
				{
					IncIns()
					auto NIns2 = NextIns();
					if (NIns2->OpCode == InstructionSet::Pop8)
					{
						IncIns()
						auto NIns3 = NextIns();
						if (NIns3->OpCode == InstructionSet::Return)
						{
							PushBytesC("\xb8");
							PushBytes(Ins.Value1.AsUInt32);
							goto FuncEnd;
						}
					}
				}
				
			}
			PushBytesC("\xB8");
			PushBytes(Ins.Value1.AsUInt32);
		}break;
		case InstructionSet::Call: 
		{
			auto V = BuildTepCall(Output);

			NullCalls.push_back({ Offset + i,V });
		}
		break;
		case InstructionSet::Store8RegOnStack:
		{

		}
		break;
		case InstructionSet::Get8FromStack:
		{
			auto NIns = NextIns();
			if (NIns)
			{
				//if (NIns->OpCode == InstructionSet::ret)
				{
					_Output->push_back(0xC6);
					_Output->push_back(0x45);
					_Output->push_back(0x01 + NIns->Value1.AsUInt8);
					_Output->push_back(Ins.Value1.AsUInt8);
					i++;
					continue;
				}

			}
		}
		break;
		case InstructionSet::Push8:
		{
			//Output.push_back(GetPushReg16(Ins.Value1.AsRegister));
		}
		break;
		case InstructionSet::Pop8:
		{
			//Output.push_back(GetPopReg16(Ins.Value1.AsRegister));
		}
		break;
		case InstructionSet::DoNothing:
			Output.push_back(AMD64Set::NoOp);
			break;
		case InstructionSet::Return:
			goto FuncEnd;
		default:
			return false;
		}

	}
	FuncEnd:
	//
	BuildFuncEnd();

	return true;
}
void Amd64Assembler::BuildFuncEnd()
{
	//_Output->push_back(AMD64Set::NoOp);	_Output->push_back(AMD64Set::NoOp);	_Output->push_back(AMD64Set::NoOp);

	_Output->push_back(0x5d);
	_Output->push_back(AMD64Set::ret);
}
void Amd64Assembler::BuildFuncStart()
{
	_Output->push_back(AMD64Set::push_rbp);    // push rbp

	_Output->push_back(AMD64Set::Mov);
	_Output->push_back(0x89);
	_Output->push_back(0xe5);   // mov	rbp, rsp

	PushBytesC("\x48\x89\x7D\xF8");//Input Ptr.
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