#pragma once
#include "../../LangCore.hpp"
#include "../Front/FrontEndObject.hpp"
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
};


struct Node 
{
	Node() { _type = NodeType::Null; };
	Node(NodeType T) { _type = T; };

	virtual ~Node(){} ;


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
	T* Node =nullptr;
};
using TryGetNode = TryGetNode_<Node>;


//ALL nodes must have this as there first member. 
#define AddforNode(Type) \
static constexpr NodeType Node_t = NodeType::##Type;\
Node* As(){return (Node*)this;}\
static Type* Gen(){return new Type();} \
static UCodeLangForceinline Node* As(Type* Value) {return (Node*)Value;} \
static UCodeLangForceinline const Node* As(const Type* Value) {return (const Node*)Value; } \
\
static UCodeLangForceinline Type* As(Node* Value) \
{ \
if (Value->Get_Type() != Node_t) \
{ \
	throw std::exception("invalid cast"); \
} \
return (Type*)Value; \
} \
static UCodeLangForceinline const Type* As(const Node* Value) \
{ \
 \
if (Value->Get_Type() != Node_t) \
{ \
	throw std::exception("invalid cast"); \
} \
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
	AddforNodeAndWithList(FileNode);


	String_view FilePath;
	void Reset()
	{
		FilePath = "";
		_Nodes.clear();
	}
};
UCodeLangEnd