#include "SemanticAnalysis.hpp"
#include "../Helpers/KeyWords.hpp"
#include "../Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(X) ReSetIns(); X;
#define GenInsPush(X)  GenIns(X) PushIns();
void SemanticAnalysis::Reset()
{
	Scope.ThisScope = ScopeHelper::_globalScope;
	_Ins = UCodeLang::Instruction();
}

void SemanticAnalysis::DoAnalysis(const FileNode& Tree)
{
	Reset();
	Value.Lib.ClearState();
	_StaticVariables.clear();


	
	auto FileNamePos = AddDebug_String(Tree.FilePath);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::FileStart, FileNamePos, _Ins));

	BuildNameSpace(&Tree._Node);
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
		case NodeType::ClassNode:BuildClass(*ClassNode::As(Item));break;
		case NodeType::EnumNode:BuildEnum(*EnumNode::As(Item)); break;
		case NodeType::FuncNode:BuildFunc(*FuncNode::As(Item));break;
		case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Item));break;
		case NodeType::DeclareThreadVariableNode:BuildDeclareThreadVariable(*DeclareThreadVariableNode::As(Item)); break;
		case NodeType::DeclareStaticVariableNode:BuildStaticDeclareVariable(*DeclareStaticVariableNode::As(Item)); break;
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
	auto _ClassName = AddDebug_String(node.ClassName.Token->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::Class, _ClassName, _Ins));

	for (const auto& Item : node._Nodes)
	{
		switch (Item->Get_Type())
		{
		case NodeType::AttributeNode:BuildAttributeNode(*AttributeNode::As(Item)); break;
		case NodeType::ClassNode:BuildClass(*ClassNode::As(Item)); break;
		case NodeType::EnumNode:BuildEnum(*EnumNode::As(Item)); break;
		case NodeType::FuncNode:BuildFunc(*FuncNode::As(Item)); break;
		case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Item)); break;
		case NodeType::DeclareThreadVariableNode:BuildDeclareThreadVariable(*DeclareThreadVariableNode::As(Item)); break;
		case NodeType::DeclareStaticVariableNode:BuildStaticDeclareVariable(*DeclareStaticVariableNode::As(Item)); break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}
	}

	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::ClassEnd, _Ins));
}

void SemanticAnalysis::BuildEnum(const UCodeLang::EnumNode& Node)
{
	auto _ClassName = AddDebug_String(Node.EnumName.Token->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::EnumClass, _ClassName, _Ins));
	BuildType(Node.BaseType);

	for (auto& item : Node.Values)
	{
		bool boolExpression = item.Expression.Value;
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::EnumValue, _ClassName, boolExpression, _Ins));
		
		if (boolExpression) 
		{
			BuildExpressionType(item.Expression);
		}
	}


	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::EnumEnd, _Ins));
}

void SemanticAnalysis::BuildAttributeNode(const UCodeLang::AttributeNode& Node)
{
}

void SemanticAnalysis::BuildFunc(const UCodeLang::FuncNode& Node)
{
	auto _Block = AddDebug_String(Node.Signature.Name.Token->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareFunc, _Block, _Ins));
	BuildType(Node.Signature.ReturnType);

	for (const auto& Item : Node.Signature.Parameters.Parameters)
	{
		BuildParameter(Item);
	}


	if (Node.Body.HasValue)
	{
		BuildStatements(Node.Body.Item.Statements);
	}


	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::FuncEnd, _Block, _Ins));
}

void SemanticAnalysis::BuildParameter(const UCodeLang::NamedParameterNode& par)
{
	auto _Name = AddDebug_String(par.Name.Token->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareParameter, _Name, _Ins));
	BuildType(par.Type);
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
	case NodeType::ClassNode:BuildClass(*ClassNode::As(Statement)); break;
	case NodeType::AttributeNode:BuildAttributeNode(*AttributeNode::As(Statement)); break;
	case NodeType::EnumNode:BuildEnum(*EnumNode::As(Statement)); break;
	case NodeType::UsingNode:BuildUseingNode(*UsingNode::As(Statement)); break;
	case NodeType::AsmBlockNode:BuildAsmBlock(*AsmBlockNode::As(Statement)); break;
	case NodeType::DeclareThreadVariableNode:BuildDeclareThreadVariable(*DeclareThreadVariableNode::As(Statement)); break;
	case NodeType::DeclareStaticVariableNode:BuildStaticDeclareVariable(*DeclareStaticVariableNode::As(Statement)); break;
	case NodeType::DeclareVariableNode:BuildDeclareVariable(*DeclareVariableNode::As(Statement)); break;
	case NodeType::RetStatementNode:BuildReturnExpression(*RetStatementNode::As(Statement)); break;
	default:
		#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
		#endif
		break;
	}
}
void SemanticAnalysis::BuildExpressionType(const UCodeLang::ExpressionNodeType& Item)
{
	auto Ex = Item.Value;

	switch (Ex->Get_Type())
	{
	case NodeType::ValueExpressionNode:BuildExpressionValue(*ValueExpressionNode::As(Ex)); break;
	case NodeType::BinaryExpressionNode:BuildBinaryExpression(*BinaryExpressionNode::As(Ex)); break;
	default:
		break;
	}
}
void SemanticAnalysis::BuildExpressionValue(const UCodeLang::ValueExpressionNode& Item)
{
	auto Ex = Item.Value;

	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareExpression, _Ins));
	switch (Ex->Get_Type())
	{
	case NodeType::NumberliteralNode:
	{
		auto node = NumberliteralNode::As(Ex);
		auto _StringPos = AddDebug_String(node->Token->Value._String);
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::NumberNumberliteral, _StringPos, _Ins));
	}	break;
	case NodeType::ReadVariableNode:
	{
		auto node = ReadVariableNode::As(Ex);
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::GetVar, _Ins));
		BuildScopedName(node->VariableName);
	}
	break;
	default:
		throw std::exception("bad Node");
		break;
	}
}
void SemanticAnalysis::BuildUnaryExpression(const UCodeLang::Node* Item)
{
}
void SemanticAnalysis::BuildBinaryExpression(const BinaryExpressionNode& Item)
{
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareBinaryExpression, _Ins));
}
void SemanticAnalysis::BuildScopedName(const ScopedNameNode& Name)
{

	auto& NameV = Name.ScopedName[0];
	auto _Stringoffset = AddDebug_String(NameV->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::Null, _Stringoffset, _Ins));
}
void SemanticAnalysis::BuildAsmBlock(const UCodeLang::AsmBlockNode& Asm)
{
	auto _Block = AddDebug_String(Asm.AsmText);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::AsmBlock,_Block, _Ins));
}
void SemanticAnalysis::BuildReturnExpression(const RetStatementNode& Item)
{
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::Ret, _Ins));
	if (Item.Expression.Value) 
	{
		BuildExpressionType(Item.Expression);
	}
}
void SemanticAnalysis::BuildStoreExpression(const String_view& VarName)
{
	auto _VarPos = AddDebug_String(VarName);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::StoreVar, _VarPos, _Ins));
}
void SemanticAnalysis::BuildStaticDeclareVariable(const DeclareStaticVariableNode& Item)
{
	BuildDeclareVariable(Item.Variable, Intermediate_Set::DeclareStaticVar);
}
void SemanticAnalysis::BuildDeclareThreadVariable(const DeclareThreadVariableNode& Item)
{
	BuildDeclareVariable(Item.Variable,Intermediate_Set::DeclareThreadVar);
}
void SemanticAnalysis::BuildDeclareVariable(const DeclareVariableNode& Item, Intermediate_Set VarType)
{
	auto _VarNamePos = AddDebug_String(Item.Name.Token->Value._String);


	
	GenInsPush(InstructionBuilder::GenInst(VarType,_VarNamePos, _Ins));
	BuildType(Item.Type);
	
	if (Item.Expression.Value)
	{
		BuildExpressionType(Item.Expression);
	}
}
void SemanticAnalysis::BuildType(const TypeNode& Item)
{

}
UCodeLangEnd