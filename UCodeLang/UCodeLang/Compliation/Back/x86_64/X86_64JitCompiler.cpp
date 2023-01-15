#include "X86_64JitCompiler.hpp"
UCodeLangStart


struct X86_64Registers
{

	//volatile 64-bit registers
	static constexpr UInt8 rax = 0;
	static constexpr UInt8 rcx = 0;
	static constexpr UInt8 rdx = 0;
	static constexpr UInt8 r8 = 0;
	static constexpr UInt8 r9 = 0;
	static constexpr UInt8 r10 = 0;

	static constexpr UInt8 al = 0;
	static constexpr UInt8 cl = 1;
	static constexpr UInt8 dl = 2;
	static constexpr UInt8 r8b = 0;
	static constexpr UInt8 r9b = 1;
	static constexpr UInt8 r10b = 2;
	static UInt8 Get8BitRegister(RegisterID Value)
	{
		switch (Value)
		{
		case RegisterID::A:return al;
		case RegisterID::B:return cl;
		case RegisterID::C:return dl;
		case RegisterID::D:return r8b;
		case RegisterID::E:return r9b;
		case RegisterID::F:return r10b;
		default:return al;
		}
	}
	static bool IsNewRegister8(RegisterID Value)
	{
		switch (Value)
		{
		case RegisterID::D:return true;
		case RegisterID::E:return true;
		case RegisterID::F:return true;
		default:return false;
		}
	}
};

X86_64JitCompiler::X86_64JitCompiler()
{
}

X86_64JitCompiler::~X86_64JitCompiler()
{
}

void X86_64JitCompiler::Reset()
{
}

bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output)
{
	Output = &X64Output;
	_Ins = &Ins;
	Index = funcAddress;
	IsGood = true;
	
	//func start
	PushBytes("\x55\x48\x89\xE5");
	//push rbp
	//mov rbp, rsp
	
	BuildBody();

	//func end
	PushBytes("\x5D\xC3");
	//pop rbp
	//ret

	
	_Ins = nullptr;
	Output = nullptr;

	return IsGood;
}

void X86_64JitCompiler::SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, Vector<UInt8>& X64Output)
{
}

void X86_64JitCompiler::BuildBody()
{
	Instruction& V = Get_Ins();
	
	while (V.OpCode != InstructionSet::Return)
	{

		switch (V.OpCode)
		{

		case InstructionSet::Store8:
		{
			auto Register = V.Value0.AsRegister;
			if (X86_64Registers::IsNewRegister8(Register)){PushBytes(0x41);}

			PushBytes( ((UInt8)0xb0) + X86_64Registers::Get8BitRegister(Register)
				, V.Value1.AsUInt8);
		}
		break;
		default:
			IsGood = false;
			return;
		}

		//
		Next_Ins(); V = Get_Ins();
	}
}

UCodeLangEnd

