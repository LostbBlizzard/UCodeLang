#include "SemanticAnalysis.hpp"
#include "../Helpers/KeyWords.hpp"
#include "../Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(X) ReSetIns(); X

void SemanticAnalysis::Reset()
{
}

void SemanticAnalysis::DoAnalysis(const FileNode& Tree)
{
	Value.Lib.ClearState();
	_StaticVariables.clear();

	
	Scope.ThisScope = ScopeHelper::_globalScope;
	_Ins = UCodeLang::Instruction();

	BuildNameSpace(&Tree._Node);


	if (_StaticVariables.size() != 0)
	{

	}
}


void SemanticAnalysis::BuildNameSpace(const Node* Tree)
{
	bool IsNameSpace = Tree->Get_Type() == NodeType::NamespaceNode;

	const Vector<Node*>& Nodes = IsNameSpace
		? NamespaceNode::As(Tree)->_Nodes : FileNode::As(Tree)->_Nodes;

	if (IsNameSpace)
	{
		auto NameSpace = NamespaceNode::As(Tree);
		NameSpace->NamespaceName.GetScopedName(TepString);

		Scope.AddScope(TepString);
	}

	for (const auto& Item : Nodes)
	{

		switch (Item->Get_Type())
		{
		case NodeType::AttributeNode:BuildAttributeNode(*AttributeNode::As(Item)); break;
		case NodeType::NamespaceNode:BuildNameSpace(Item);break;
		case NodeType::NameNode:BuildClass(*ClassNode::As(Item));break;
		case NodeType::FuncNode:BuildFunc(*FuncNode::As(Item));break;
		case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Item));break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

	}

	if (IsNameSpace) {
		Scope.ReMoveScope();
	}
}
void SemanticAnalysis::BuildUseingNode(const UCodeLang::UsingNode& Item)
{
}
void SemanticAnalysis::BuildStaticVariable(const UCodeLang::Node* node)
{
	_StaticVariables.push_back(node);
}

void SemanticAnalysis::BuildClass(const UCodeLang::ClassNode& node)
{
	for (const auto& Item : node._Nodes)
	{

		switch (Item->Get_Type())
		{
		case NodeType::NameNode:BuildClass(*ClassNode::As(Item)); break;
		case NodeType::FuncNode:BuildFunc(*FuncNode::As(Item)); break;
		case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Item)); break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

	}
}

void SemanticAnalysis::BuildAttributeNode(const UCodeLang::AttributeNode& Node)
{
}

void SemanticAnalysis::BuildFunc(const UCodeLang::FuncNode& Node)
{
	auto _Block = AddDebug_String(Node.Signature.Name.Token->Value._String);
	GenIns(InstructionBuilder::GenInst(Intermediate_Set::DeclareFunc, _Block, _Ins));
	Value.Lib.Add_Instruction(_Ins);

	if (Node.Body.HasValue)
	{
		BuildStatements(Node.Body.Item.Statements);
	}


	GenIns(InstructionBuilder::GenInst(Intermediate_Set::FuncEnd, _Block, _Ins));
	Value.Lib.Add_Instruction(_Ins);
}

void SemanticAnalysis::BuildStatements(const UCodeLang::StatementsNode& BodyStatements)
{
	for (const auto& Item : BodyStatements._Nodes)
	{
		BuildStatement(Item);
	}
}

void SemanticAnalysis::BuildStatement(const UCodeLang::Node* Statement)
{
	switch (Statement->Get_Type())
	{
	case NodeType::NameNode:BuildClass(*ClassNode::As(Statement)); break;
	case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Statement)); break;
	case NodeType::AsmBlockNode:BuildAsmBlock(*AsmBlockNode::As(Statement)); break;
	default:
		#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
		#endif
		break;
	}
}
void SemanticAnalysis::BuildExpressionType(const UCodeLang::Node* Statement)
{

}
void SemanticAnalysis::BuildExpressionValue(const UCodeLang::Node* Item)
{

}
void SemanticAnalysis::BuildUnaryExpression(const UCodeLang::Node* Item)
{
}
void SemanticAnalysis::BuildBinaryExpression(const UCodeLang::Node* Item)
{

}
void SemanticAnalysis::BuildAsmBlock(const UCodeLang::AsmBlockNode& Asm)
{
	auto _Block = AddDebug_String(Asm.AsmText);
	GenIns(InstructionBuilder::GenInst(Intermediate_Set::AsmBlock,_Block, _Ins));
	Value.Lib.Add_Instruction(_Ins);
}
void SemanticAnalysis::BuildReturnExpression()
{
	GenIns(InstructionBuilder::GenInst(Intermediate_Set::Ret, _Ins));
	Value.Lib.Add_Instruction(_Ins);
}
void SemanticAnalysis::BuildStoreExpression(const String_view& VarName)
{
	auto _VarPos = AddDebug_String(VarName);
	GenIns(InstructionBuilder::GenInst(Intermediate_Set::StoreVar, _VarPos, _Ins));
	Value.Lib.Add_Instruction(_Ins);
}
void SemanticAnalysis::BuildDeclareVariable(const String_view& VarName, const String_view& Type)
{
	auto _VarPos = AddDebug_String(VarName);
	auto _TypePos = AddDebug_String(Type);
	
	GenIns(InstructionBuilder::GenInst(Intermediate_Set::DeclareVar, _TypePos, _VarPos, _Ins));
	Value.Lib.Add_Instruction(_Ins);
}
UCodeLangEnd