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
	DeclareVariableNode,
	FuncNode,
	NamedParameters, 
	NameNode,
	NamedParametersNode,
	StatementsNode,
	ScopedNameNode,
};


struct Node
{
	NodeType _Type;
	Node() : _Type(NodeType::Null)
	{

	};
	Node(NodeType type) : _Type(type)
	{

	};
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
static constexpr NodeType Node_t =NodeType::##Type;\
Node _Node = Node(Node_t); \
\
Node* As()\
{\
	return (Node*)this;\
}\
static Type* Gen() \
{ \
	return new Type();\
} \
static UCodeLangForceinline Node* As(Type* Value) \
{ \
	return (Node*)Value; \
} \
static UCodeLangForceinline const Node* As(const Type* Value) \
{ \
	return (const Node*)Value; \
} \
\
static UCodeLangForceinline Type* As(Node* Value) \
{ \
 \
if (Value->_Type != Node_t) \
{ \
	throw std::exception("invalid cast"); \
} \
return (Type*)Value; \
} \
static UCodeLangForceinline const Type* As(const Node* Value) \
{ \
 \
if (Value->_Type != Node_t) \
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
};
UCodeLangEnd