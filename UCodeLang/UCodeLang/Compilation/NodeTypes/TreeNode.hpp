#pragma once

#ifndef UCodeLangNoCompiler
#include "../../LangCore.hpp"
#include "../Front/FrontEndObject.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

enum class NodeType :UInt8
{
	Null,
	FileNode,
	NamespaceNode,
	ClassNode,
	UsingNode,
	ScopedName,
	DeclareStaticVariableNode,
	DeclareThreadVariableNode,
	DeclareVariableNode,
	FuncNode,
	NamedParameters, 
	NameNode,
	NamedParametersNode,
	StatementsNode,
	ScopedNameNode,
	AsmBlockNode,
	AttributeNode,
	ValueParametersNode,
	StringliteralNode,
	ExpressionNodeType,
	ValueExpressionNode,
	BinaryExpressionNode,
	RetStatementNode,
	NumberliteralNode,
	AliasNode,
	EnumNode,
	TagTypeNode,
	IfNode,
	ElseNode,
	WhileNode,
	DoNode,
	BoolliteralNode,
	ReadVariableNode,
	AssignVariableNode,
	PostfixVariableNode,
	CompoundStatementNode,
	FuncCallNode, 
	FuncCallStatementNode,
	AnonymousTypeNode,
	AnonymousObjectConstructorNode,
	CastNode,
	ParenthesesExpresionNode,
	SizeofExpresionNode,
	NewExpresionNode,
	DropStatementNode,
	LibImportNode,
	FloatliteralNode,
	CharliteralNode,
	IndexedExpresionNode,
	ForNode,
	ContinueNode,
	BreakNode,
	MoveNode,
	LambdaNode,
	TraitNode,
	OutExpression,
	BitCastExpression,
	PanicNode,
	AssignExpressionNode,
	InvalidNode,
	ValidNode,
	CMPTypesNode,
	TypeToValueNode,
	ExpressionToTypeValueNode,
	DeclareEvalVariableNode,
	CompileTimeIfNode,	
	CompileTimeForNode,
	ExtendedScopeExpression,
	ExtendedFuncExpression,
	MatchStatement,
	MatchExpression,
	MatchArm,
	ImportStatement,
	AwaitExpression,
	AwaitStatement,
	YieldExpression,
	YieldStatement,
	UnsafeExpression,
	UnsafeStatementsNode,
	DeferStatementNode,
	UnaryExpressionNode,
	ForTypeNode,
	ExtendedFuncStatement,

	//Inturnal used in SystematicAnalyis
	InturnalEvalTypeCheck,
};


struct Node 
{
	Node() { _type = NodeType::Null; };
	Node(NodeType T) { _type = T; };

	virtual ~Node(){} ;

	Node(const Node& source) = delete;
	Node& operator=(const Node& source) = delete;

	Node(Node&& source) noexcept = default;
	Node& operator=(Node&& source) noexcept = default;

	UCodeLangForceinline NodeType Get_Type() const
	{
		return _type;
	}
private:
	NodeType _type;
};

enum class GotNodeType :UInt8
{
	Null,
	Success,
	failed,
	Error,
	EndLoop,
};

template<typename T> struct TryGetNode_
{
	GotNodeType GotNode = GotNodeType::Null;
	Unique_ptr<T> Node =nullptr;
};
using TryGetNode = TryGetNode_<Node>;


//ALL nodes must have this as there first member. 
#define AddforNode(Type) \
static constexpr NodeType Node_t = NodeType:: Type;\
Node* As(){return (Node*)this;}\
const Node* As() const {return (const Node*)this;}\
static Unique_ptr<Type> Gen(){return Unique_ptr<Type>(new Type());} \
static UCodeLangForceinline Node* As(Type* Value) {return (Node*)Value;} \
static UCodeLangForceinline const Node* As(const Type* Value) {return (const Node*)Value; } \
\
static UCodeLangForceinline Type* As(Node* Value) \
{ \
UCodeLangAssert(Value->Get_Type() == Node_t);\
return (Type*)Value; \
} \
static UCodeLangForceinline const Type* As(const Node* Value) \
{ \
UCodeLangAssert(Value->Get_Type() == Node_t); \
return (const Type*)Value; \
} \


#define Has_NodesList(Type) \
Vector<Unique_ptr<Node>> _Nodes; \



#define AddforNodeAndWithList(Type) \
AddforNode(Type); \
Has_NodesList(Type); \

struct FileNode : FileNode_t,Node
{
	FileNode() :Node(NodeType::FileNode)
	{
	
	}
	FileNode(FileNode&& Source) = default;
	FileNode& operator=(FileNode&& Source) = default;
	AddforNodeAndWithList(FileNode);


	Vector<Token> Tokens;
	Vector<Unique_ptr<Token>> TemporaryTokens;//int?,int[],int[10],int[:] etc.
	Vector<Unique_ptr<String>> TemporaryStrings;
	String_view FileText;//needed to save generics in .ulib
	void Reset()
	{
		_Nodes.clear();
		TemporaryTokens.clear();
		TemporaryStrings.clear();
	}
};

enum class ImportMode
{
	IntermediateFile,
	ExternalFile,
};

struct LibImportNode :FileNode_t, Node
{
	LibImportNode() : Node(NodeType::LibImportNode)
	{

	}
	LibImportNode(LibImportNode&& source);
	AddforNode(LibImportNode);

	UClib LIb;
	Vector<Unique_ptr<Node>> _Nodes;
	ImportMode Mode = ImportMode::IntermediateFile;
};
UCodeLangEnd

#endif