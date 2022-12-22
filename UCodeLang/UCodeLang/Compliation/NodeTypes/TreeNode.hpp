#pragma once
#include "../../LangCore.hpp"
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
	AttributeTypeNode,
	IfNode,
	ElseNode,
	BoolliteralNode,
	ReadVariableNode,
	AssignVariableNode,
};


struct Node
{
	typedef void (*Nodedone)(Node* ptr);
	struct VptrData
	{
		NodeType _Type;
		Nodedone _Destroy;
		constexpr VptrData(NodeType Type, Nodedone Destroy): _Type(Type), _Destroy(Destroy)
		{

		}
	};
	const VptrData* _Vptr;
	Node() : _Vptr(nullptr)
	{

	};
	Node(const VptrData* ptr) : _Vptr(ptr)
	{

	};
	UCodeLangForceinline NodeType Get_Type() const
	{
		return _Vptr->_Type;
	}
	~Node()
	{
		if (_Vptr)
		{
			_Vptr->_Destroy(this);
		}
	}
};

enum class GotNodeType :UInt8
{
	Null,
	Success,
	failed,
	Error,
	EndLoop,
};
template<typename T> struct Optional
{
	bool HasValue = false;
	T Item;
};
template<typename T> struct TryGetNode_
{
	GotNodeType GotNode;
	T* Node;
};
using TryGetNode = TryGetNode_<Node>;


//ALL nodes must have this as there first member. 
#define AddforNode(Type) \
static void _Destroy(Node* Value){auto V = As(Value);Value->_Vptr =nullptr; V->~##Type();};\
static constexpr NodeType Node_t =NodeType::##Type;\
static constexpr Node::VptrData VPtr= Node::VptrData(Node_t,##Type::_Destroy);\
Node _Node = Node(&VPtr); \
\
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
Vector<Node*> _Nodes; \
~##Type() \
{ \
	for (auto Item : _Nodes) { delete Item; } \
	_Nodes.clear(); \
}\


#define AddforNodeAndWithList(Type) \
AddforNode(Type); \
Has_NodesList(Type); \

struct FileNode
{
	AddforNodeAndWithList(FileNode);

	String_view FilePath;
};
UCodeLangEnd