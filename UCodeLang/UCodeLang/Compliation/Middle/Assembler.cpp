#include "Assembler.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangStart

#define GenIns(X) ReSetIns(); X;
#define GenInsPush(X)  GenIns(X) PushIns();

void Assembler::Reset()
{
	StaticStackSize = 0;
	Debugoffset = 0;
	Index = 0;
	_RegisterState.ResetRegistersData();
	Symbols.clear();
	ThisSymbolInfo = symbolInfo();
}
void Assembler::Assemble(UClib* Output,UClib* Data)
{
	Reset();
	_OutPut = Output;
	_OutPut->ClearState();
	_Input = Data;

	BuildBuffer();
	
	BuildTypes();
	
	BuildCode();
	
	LinkIns();

	ClassAssembly::Move(Data->Get_Assembly(), Output->Get_Assembly());
}
void Assembler::LinkIns()
{
	auto& In_s = _OutPut->Get_Instructions();
	for (size_t i = 0; i < In_s.size(); i++)
	{
		auto& Ins = In_s[i];
		if (Ins.OpCode_AsInt == (InstructionSet_t)Intermediate_Set::TepFuncCall)
		{
			String_view FuncName = Get_StringFromDebug(Ins.Value0.AsAddress);
			UAddress funcpos = _OutPut->Get_NameToInstruction(FuncName.data());
			InstructionBuilder::Call(funcpos, Ins);
		}
	}
}

void Assembler::BuildBuffer()
{
	GenInsPush(InstructionBuilder::Exit(ExitState::Failure, _Ins));
}
void Assembler::BuildTypes()
{
	Index = 0;

	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::FileEnd:Debugoffset = Inter->Value0.AsUInt64; NextIns(); break;
		default:break;
		}

		NextIns();
	}
}
void Assembler::BuildCode()
{
	Index = 0;

	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::FileStart:
		{
			String_view FilePath = Get_StringFromDebug(Inter->Value0.AsUInt64);
			ThisSymbolInfo.FilePath = FilePath;
			NextIns();
		}
		break;
		case Intermediate_Set::DeclareFunc:
			break;
		case Intermediate_Set::SetFilePos:SetFilePos(Inter); NextIns();break;
		case Intermediate_Set::FileEnd:Debugoffset = Inter->Value0.AsUInt64; NextIns();break;
		default:
			break;
		}
	}
}
void Assembler::SetFilePos(UCodeLang::Assembler::Intermediate_Instruction* Inter)
{
	ThisSymbolInfo.Line = Inter->Value0.AsUInt64;
	ThisSymbolInfo.Pos = Inter->Value1.AsUInt64;
}
void Assembler::BuildAsm(Intermediate_Instruction& Ins)
{
}

void Assembler::GetType(TypeData& Out)
{
}


UCodeLangEnd


