#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Middle/SystematicAnalysis.hpp"
UCodeLangStart
void UCodeBackEnd::Bind(BackEndInterface& Obj)
{
	Obj
	.Set_GenBackEnd(Gen)
	.Set_DeleteBackEnd(Delete)
	.Set_ErrorsOutput(Set_ErrorsOutput)
	.Set_BuildStatements(BuildStatements)
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

void UCodeBackEndObject::BuildStatements(const Vector<Node*>& nodes)
{
	for (const auto& node : nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::AsmBlockNode:BuildAsmNode(*AsmBlockNode::As(node)); break;
		default:break;
		}
	}
}

void UCodeBackEndObject::BuildAsmNode(const AsmBlockNode& node)
{
	UAssembly.Set_ErrorsOutput(ErrorOutput);
	UAssembly.Assemble(node.AsmText, &Analysis->Get_Output());
}

UCodeLangEnd

