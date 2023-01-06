#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
void UCodeBackEnd::Bind(BackEndInterface& Obj)
{
	Obj
	.Set_GenBackEnd(Gen)
	.Set_DeleteBackEnd(Delete)
	.Set_ErrorsOutput(Set_ErrorsOutput)
	.Set_Analysis(Set_Analysis)
	.Set_BuildInput(Build)
		;
}
const BackEndInterface* UCodeBackEnd::Get()
{
	static bool IsMade = false;
	static BackEndInterface BackEnd;
	if (!IsMade)
	{
		Bind(BackEnd);
		IsMade = true;
	}

	return &BackEnd;
}

UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Build(BackEndInput& Input)
{
	_BackInput = &Input;

	auto& Code = Input._Builder->Get_Code();
	
	for (_Index = 0; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];
		if (IR.Operator == IROperator::Func)
		{
			BuildFunc();
		}
	}
}

void UCodeBackEndObject::BuildAsmNode(const AsmBlockNode& node)
{
	UAssembly.Set_ErrorsOutput(ErrorOutput);
	UAssembly.Assemble(node.AsmText, &Getliboutput());
}

void UCodeBackEndObject::BuildRetNode(const RetStatementNode& node)
{
	auto& ULib = Getliboutput();

	
	//BuildExpression(node.Expression);

	GenIns(InstructionBuilder::Return(ExitState::Success, _Ins));
	ULib.Add_Instruction(_Ins);

}

void UCodeBackEndObject::BuildExpression(const ExpressionNodeType& node)
{
}

void UCodeBackEndObject::BuildFunc()
{
	auto& Code = _BackInput->_Builder->Get_Code();
	auto& Func = Code[_Index];
	auto SybID = Func.Operand0.SymbolId;
	auto Sym = _BackInput->_Table->GetSymbol(SybID);

	auto& ULib = Getliboutput();
	
	size_t FuncStart = _Index;


	for (_Index = _Index + 1; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];

		switch (IR.Operator)
		{
			case IROperator::Ret:
				goto EndLoop;
			break;
		}
	}
EndLoop:

	GenIns(InstructionBuilder::Return(ExitState::Success, _Ins));
	ULib.Add_Instruction(_Ins);


	ULib.Add_NameToInstruction(FuncStart, Sym.FullName);
}

UCodeLangEnd

