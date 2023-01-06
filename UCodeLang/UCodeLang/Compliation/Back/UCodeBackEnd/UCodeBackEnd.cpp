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

UCodeLangEnd

