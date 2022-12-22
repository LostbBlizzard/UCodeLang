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

	_Assembly.Set_ErrorsOutput(_ErrorsOutput);
	_Assembly.Set_Settings(_Settings);

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
		case Intermediate_Set::FileStart:
		{
			String_view FilePath = Get_StringFromDebug(Inter->Value0.AsUInt64);
			ThisSymbolInfo.FilePath = FilePath;
			NextIns();
		}
		break;
		case Intermediate_Set::FileEnd:Debugoffset = Inter->Value0.AsUInt64; NextIns(); break;
		default:break;
		}

		NextIns();
	}
}
void Assembler::BuildCode()
{
	Index = 0;
	Debugoffset = 0;

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
		case Intermediate_Set::DeclareFunc:BuildDeclareFunc(*Inter); break;
		case Intermediate_Set::SetFilePos:SetFilePos(*Inter); break;
		case Intermediate_Set::FileEnd:Debugoffset = Inter->Value0.AsUInt64; NextIns();break;
		default:
			throw std::exception("");
			break;
		}
	}
}
void Assembler::BuildDeclareFunc(Intermediate_Instruction& Inter)
{
	NextIns();
	
	funcStackSize = 0;
	auto FuncName = Get_StringFromDebug(Inter.Value0.AsUInt64);
	func_symbol& Data =AddFunc(FuncName);
	GetType(Data.RetType);
	
	FuncType = &Data.RetType;

	UAddress FuncIndex = _OutPut->Get_Instructions().size();
	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::FuncEnd:goto EndWhile;
		case Intermediate_Set::DeclareVar:BuildDeclareVar(*Inter); break;
		case Intermediate_Set::Ret:BuildRet(*Inter); break;
		case Intermediate_Set::AsmBlock:BuildAsm(*Inter); break;
		default:
			throw std::exception("");
			break;
		}
	}
EndWhile:
	GenInsPush(InstructionBuilder::Return(ExitState::Success,_Ins));


	FuncType = nullptr;
	_OutPut->Add_NameToInstruction(FuncIndex,FuncName.data());
	NextIns();
}
void Assembler::SetFilePos(UCodeLang::Assembler::Intermediate_Instruction& Inter)
{
	NextIns();
	ThisSymbolInfo.Line = Inter.Value0.AsUInt64;
	ThisSymbolInfo.Pos = Inter.Value1.AsUInt64;
}
void Assembler::BuildAsm(Intermediate_Instruction& Ins)
{
	NextIns();
	String_view AsmBlock = Get_StringFromDebug(Ins.Value0.AsUInt64);
	_Assembly.Assemble(AsmBlock, _OutPut);
}

Assembler::DeclareExpression_ret_t Assembler::DeclareExpression(Intermediate_Instruction& Ins)
{
	Assembler::DeclareExpression_ret_t r;
	NextIns();
	if (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::NumberNumberliteral:
		{	
			NextIns();

			String_view V = Get_StringFromDebug(Inter->Value0.AsUInt64);
			UInt8 Num;
			ParseHelper::ParseStringToUInt8(V, Num);
			auto Register = _RegisterState.GetFreeRegister();
			GenInsPush(InstructionBuilder::Store8(_Ins, Register,Num));
			
			
			TypeData::SetToUInt8(r.ExpressionType);
			r.Value = Register;

			return r;
		}
		case Intermediate_Set::GetVar:
		{
			NextIns();
			Inter = Get_Ins();
			String_view V = Get_StringFromDebug(Inter->Value0.AsUInt64);
			symbol* S = Symbols.FindSymbol(V);
		
			r.ExpressionType = S->Var.Type;
			r.Value = GetSymbolInFreeRegister(&S->Var);

			NextIns();
			return r; 
		}
		break;
		default:
			throw std::exception("");
			break;
		}
	}
}

void Assembler::BuildRet(Intermediate_Instruction& Ins)
{
	NextIns();
	if (Intermediate_Instruction* Inter = Get_Ins())
	{
		if (Inter->OpCode == Intermediate_Set::DeclareExpression)
		{
			auto ExRegister = DeclareExpression(*Inter);
			BuildRegToReg8(ExRegister.Value, RegisterID::OuPutRegister);
		}
	}
}

void Assembler::BuildDeclareVar(Intermediate_Instruction& Ins)
{
	NextIns();
	String_view VarName = Get_StringFromDebug(Ins.Value0.AsUInt64);
	Var_symbol& Symbol = AddVar(VarName);
	GetType(Symbol.Type);
	Exlooking = &Symbol.Type;

	Symbol.Address = funcStackSize;
	funcStackSize += Symbol.Type.Size;
	Symbol.Location = Var_symbol::LocationType::Stack;

	if (Intermediate_Instruction* Inter = Get_Ins())
	{
		if (Inter->OpCode == Intermediate_Set::DeclareExpression)
		{
			auto ExRegister = DeclareExpression(*Inter);
			BuildSetVar(&Symbol, ExRegister.Value);
		}
	}
	Exlooking = nullptr;
}

void Assembler::GetType(TypeData& Out)
{
}

void Assembler::BuildSetVar(Var_symbol* Sym, RegisterID Register)
{
	switch (Sym->Location)
	{
	case Var_symbol::LocationType::Stack:
		GenInsPush(InstructionBuilder::StoreRegOnStack8(_Ins, Register,Sym->Address));
		break;
	default:
		throw std::exception("");
		break;
	}

	


	_RegisterState.SetRegisterUse(Register, Sym);
}

void Assembler::BuildRegToReg8(RegisterID A, RegisterID B)
{
	if (A != B)
	{
		GenInsPush(InstructionBuilder::Push8(_Ins, A));
		GenInsPush(InstructionBuilder::Pop8(_Ins, B));
	}
}

void Assembler::GetSymbolInRegister(Var_symbol* Symbol, RegisterID id)
{
	switch (Symbol->Location)
	{
	case Var_symbol::LocationType::Stack:
		GenInsPush(InstructionBuilder::GetFromStack8(_Ins, Symbol->Address, id));
		break;
	default:
		throw std::exception("");
		break;
	}

	_RegisterState.SetRegisterUse(id, Symbol);
}


UCodeLangEnd


