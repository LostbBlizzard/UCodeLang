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
	auto& ClassName = node.ClassName.Token->Value._String;
	auto _ClassName = AddDebug_String(ClassName);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::Class, _ClassName, _Ins));
	
	ClassData ClassData(ClassType::Class);
	OnClass.push(&ClassData);
	ClassData.Name = ClassName;
	ClassData.FullName = (String)Scope.ThisScope + ScopeHelper::_ScopeSep + (String)ClassName;


	Scope.AddScope((String)ClassName);
	Vector<DeclareVariableNode*> MemberVariables;
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
		case NodeType::DeclareVariableNode:MemberVariables.push_back(DeclareVariableNode::As(Item));break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}
	}

	{
		
		String V = ClassInitializefuncNameC((String)ClassName);
		auto _Block = AddDebug_String(V);
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareFunc, _Block, _Ins));
		
		TypeNode ret;
		TypeNode::Gen_void(ret, *node.ClassName.Token);
		BuildType(ret);

		for (auto& Item : MemberVariables)
		{
			BuildDeclareVariable(*Item, Intermediate_Set::DeclareThisVar);
		}
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::FuncEnd, _Block, _Ins));
	}

	Scope.ReMoveScope();
	OnClass.pop();
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

	if (par.Type.IsThisMemberFunc())
	{
		if (OnClass.size() == 0)
		{
			auto Token = par.Type.Name.Token;
			_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "this cant not be use here");
		}
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareThisParameter, _Ins));
	}
	else
	{
		auto _Name = AddDebug_String(par.Name.Token->Value._String);
		GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareParameter, _Name, _Ins));
		BuildType(par.Type);
	}
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
	case NodeType::AssignVariableNode:BuildAssignVariableNode(*AssignVariableNode::As(Statement)); break;
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
	Intermediate_Set V = Get_AsIntermediate(Item.BinaryOp->Type);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::DeclareBinaryExpression,(UInt64)V, _Ins));
	BuildExpressionType(Item.Value0);
	BuildExpressionType(Item.Value1);
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
void SemanticAnalysis::BuildAssignVariableNode(const AssignVariableNode& Item)
{
	auto _VarPos = AddDebug_String(Item.Name.Token->Value._String);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::AssignVariable, _VarPos, _Ins));

	BuildExpressionType(Item.Expression);
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
	bool IsThis = Item.Name.Token->Type == TokenType::KeyWorld_This;
	if (IsThis)
	{
		if (OnClass.size() == 0) 
		{
			auto Token = Item.Name.Token;
			_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "this cant not be use here");
		}
		return;
	}

	String_view TypeName;
	auto T = Item.Name.Token->Type;
	switch (T)
	{
	case TokenType::Name:
		TypeName = Item.Name.Token->Value._String;
		break;
	case TokenType::KeyWorld_This:
		TypeName = OnClass.top()->FullName;
		break;
	case TokenType::KeyWorld_var:
	case TokenType::Void:
		TypeName = StringHelper::ToString(T);
		break;
	default:
	{
		if (TypeNode::IsPrimitive(T))
		{
			TypeName = StringHelper::ToString(T);
		}
		else
		{
			throw std::exception();
		}
	}
		break;
	}

	auto _VarPos = AddDebug_String(TypeName);
	GenInsPush(InstructionBuilder::GenInst(Intermediate_Set::Type, _VarPos, _Ins));
}
UCodeLangEnd