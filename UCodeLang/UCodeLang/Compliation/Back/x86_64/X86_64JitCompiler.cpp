#include "X86_64JitCompiler.hpp"

#include <sstream>
#include <iostream>
UCodeLangStart


x86_64::GeneralRegisters GetGeneralRegister(RegisterID Value)
{
	switch (Value)
	{
	case RegisterID::A:return x86_64::GeneralRegisters::D;
	case RegisterID::B:return x86_64::GeneralRegisters::C;
	case RegisterID::C:return x86_64::GeneralRegisters::r8;
	case RegisterID::D:return x86_64::GeneralRegisters::r9;
	case RegisterID::E:return x86_64::GeneralRegisters::A;
	case RegisterID::F:return x86_64::GeneralRegisters::B;

	default:return x86_64::GeneralRegisters::Null;
	}
}


X86_64JitCompiler::X86_64JitCompiler()
{
}

X86_64JitCompiler::~X86_64JitCompiler()
{
}

void X86_64JitCompiler::Reset()
{
}

const char HEX_MAP[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char replace(unsigned char c)
{
	return HEX_MAP[c & 0x0f];
}

std::string char_to_hex(unsigned char c)
{
	std::string hex;

	// First four bytes
	char left = (c >> 4);
	// Second four bytes
	char right = (c & 0x0f);

	hex += replace(left);
	hex += replace(right);

	return hex;
}
bool X86_64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output)
{
	Output = &X64Output;
	_Ins = &Ins;
	
	//
	for (size_t i = funcAddress; i < Ins.size(); i++)
	{
		auto& Item = Ins[i];
	
		switch (Item.OpCode)
		{
		case InstructionSet::StoreRegToReg8:_Gen.Push_Ins_RegToReg8(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister)); break;
		case InstructionSet::StoreRegToReg16:_Gen.Push_Ins_RegToReg16(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister)); break;
		case InstructionSet::StoreRegToReg32:_Gen.Push_Ins_RegToReg32(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister)); break;
		case InstructionSet::StoreRegToReg64:_Gen.Push_Ins_RegToReg64(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister)); break;
		
		case InstructionSet::Store8:_Gen.Push_Ins_MovImm8(GetGeneralRegister(Item.Value0.AsRegister), Item.Value1.AsInt8); break;
		case InstructionSet::Store16:_Gen.Push_Ins_MovImm16(GetGeneralRegister(Item.Value0.AsRegister), Item.Value1.AsInt16); break;
		case InstructionSet::Store32:_Gen.Push_Ins_MovImm32(GetGeneralRegister(Item.Value0.AsRegister), Item.Value1.AsInt32); break;
		case InstructionSet::Store64:_Gen.Push_Ins_MovImm64(GetGeneralRegister(Item.Value0.AsRegister), Item.Value1.AsInt64); break;



		case InstructionSet::Add8:_Gen.Push_Ins_Add8(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister), GetGeneralRegister(RegisterID::OuPutRegister)); break;
		case InstructionSet::Add16:_Gen.Push_Ins_Add16(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister), GetGeneralRegister(RegisterID::OuPutRegister));  break;
		case InstructionSet::Add32:_Gen.Push_Ins_Add32(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister), GetGeneralRegister(RegisterID::OuPutRegister)); break;
		case InstructionSet::Add64:_Gen.Push_Ins_Add64(GetGeneralRegister(Item.Value0.AsRegister), GetGeneralRegister(Item.Value1.AsRegister), GetGeneralRegister(RegisterID::OuPutRegister));  break;
		
		case InstructionSet::Call:
		{
			
			_Gen.Push_Ins_MovImm64(X86_64Gen::GReg::A, (uintptr_t)&OnUAddressPar);
			_Gen.Push_Ins_MovImm64(X86_64Gen::GReg::B, i);

			_Gen.Push_Ins_MovReg64ToPtrdereference(X86_64Gen::GReg::A, X86_64Gen::GReg::B);
			//pass to 
			//static UInt64 OnUAddressCall(UAddress TryingToCall);
			NullCalls.push_back({ i, _Gen.GetIndex() }); _Gen.Push_Ins_Callptr(0);



		}break;
		//case InstructionSet::SysCall: BuildSysCallIns(*(InstructionSysCall*)&Item.Value0, Item.Value1.AsRegister);
		case InstructionSet::Return:_Gen.Push_Ins_ret(); goto EndLoop;
		default:return false;
		}
	}
	EndLoop:
	//
	_Ins = nullptr;
	Output = nullptr;

	X64Output = std::move(_Gen._Base._Output.ByteOutput);
	



	
	return true;
}





void BuildSysCallIns(InstructionSysCall Ins, RegisterID Reg)
{
	switch (Ins)
	{

	case UCodeLang::InstructionSysCall::Cout_CString:
		break;
	case UCodeLang::InstructionSysCall::Cout_Char:
		break;
	case UCodeLang::InstructionSysCall::Cout_Buffer:
		break;
	case UCodeLang::InstructionSysCall::Cout_ReadChar:
		
		break;
	case UCodeLang::InstructionSysCall::File_Open:
		break;
	case UCodeLang::InstructionSysCall::File_Close:
		break;
	case UCodeLang::InstructionSysCall::File_Read:
		break;
	default:
		break;
	}
}
void X86_64JitCompiler::SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, Vector<UInt8>& X64Output)
{
	_Gen.Sub_Ins_Callptr(&X64Output[CPPOffset], *(uint64_t*)&Value);
}

UCodeLangEnd

