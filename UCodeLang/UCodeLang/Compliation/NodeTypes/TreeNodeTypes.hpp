#pragma once
#include "../../LangCore.hpp"
#include "TreeNodeTypes.hpp"
#include "..//..//LangCore/TypeNames.hpp"
#include "../..//LangCore/ScopeHelper.hpp"
UCodeLangStart

struct StringliteralNode :Node
{
	StringliteralNode() :Node(NodeType::StringliteralNode)
	{

	}

		

	AddforNode(StringliteralNode);

	const Token* Token = nullptr;
};



struct BoolliteralNode :Node
{
	BoolliteralNode() :Node(NodeType::BoolliteralNode)
	{

	}
	AddforNode(BoolliteralNode);

	bool Value = false;
};
struct NumberliteralNode :Node
{
	NumberliteralNode() :Node(NodeType::NumberliteralNode)
	{

	}
	AddforNode(NumberliteralNode);

	const Token* Token = nullptr;
};

struct NameNode :Node
{
	NameNode() : Node(NodeType::NumberliteralNode)
	{

	}
	AddforNode(NameNode);

	const Token* Token = nullptr;
	String AsString()const
	{
		return  String(Token->Value._String);
	}
	String_view AsStringView() const
	{
		return Token->Value._String;
	}
};
struct ScopedNameNode :Node
{
	ScopedNameNode() : Node(NodeType::ScopedNameNode)
	{

	}
	AddforNode(ScopedNameNode);

	Vector<const Token*> ScopedName;

	inline void GetScopedName(String& out) const
	{
		for (size_t i = 0; i < ScopedName.size(); i++)
		{
			auto item = ScopedName[i];

			if (i != ScopedName.size() - 1)
			{
				out += ScopeHelper::_ScopeSep;
			}
		}
	}
};

struct ReadVariableNode :Node
{
	ReadVariableNode() : Node(NodeType::ReadVariableNode)
	{

	}
	AddforNode(ReadVariableNode);

	ScopedNameNode VariableName;
};
struct NamespaceNode :Node
{
	NamespaceNode() : Node(NodeType::NamespaceNode)
	{

	}
	NamespaceNode(NamespaceNode&& Source) = default;
	AddforNodeAndWithList(NamespaceNode);

	ScopedNameNode NamespaceName;
};
struct GenericValueNode :Node
{
	GenericValueNode() : Node(NodeType::Null)
	{

	}
const Token* Token = nullptr;
};
struct GenericValuesNode :Node
{
	GenericValuesNode() : Node(NodeType::Null)
	{

	}
	Vector<GenericValueNode> Values;

	UCodeLangForceinline bool HasGeneric()
	{
		return Values.size();
	}
};
struct ClassNode :Node
{
	ClassNode() : Node(NodeType::ClassNode)
	{

	}
	ClassNode(ClassNode&& Source) = default;
	AddforNodeAndWithList(ClassNode);

	NameNode ClassName;
	GenericValuesNode Generic;
};


struct UsingNode :Node
{

	UsingNode() : Node(NodeType::UsingNode)
	{

	}
	AddforNode(UsingNode);

	ScopedNameNode ScopedName;
};


struct TypeNode :Node
{
	TypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	NameNode Name;
	GenericValuesNode Generic;


	static constexpr bool IsType(TokenType Type)
	{
		if (IsPrimitive(Type)) { return true; }
		else if (Type == TokenType::Name) { return true; }
		else if (Type == TokenType::KeyWorld_This) { return true; }

		return false;
	}

	static constexpr bool IsPrimitive(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::KeyWorld_Bool:
		case TokenType::KeyWorld_Char:
		case TokenType::KeyWorld_UInt8:
		case TokenType::KeyWorld_UInt16:
		case TokenType::KeyWorld_UInt32:
		case TokenType::KeyWorld_UInt64:
		case TokenType::KeyWorld_SInt8:
		case TokenType::KeyWorld_SInt16:
		case TokenType::KeyWorld_SInt32:
		case TokenType::KeyWorld_SInt64:
		case TokenType::KeyWorld_uintptr:
		case TokenType::KeyWorld_sintptr:
			return true;
		default:return false;
		}

	}


	static void Gen_Type(TypeNode& Out, TokenType Type, const Token& ToGetLinesFrom)
	{
		auto T = new Token();
		T->Type = Type;
		T->OnLine = ToGetLinesFrom.OnLine;
		T->OnPos = ToGetLinesFrom.OnPos;
		Out.HasMadeToken = true;
		Out.Name.Token = T;
	}
	static void Gen_void(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::Void, ToGetLinesFrom);
	}
	static void Gen_Var(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWorld_var, ToGetLinesFrom);
	}
	static void Gen_ThisMemberFunc(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWorld_ThisMemberFunc, ToGetLinesFrom);
	}
	static void Gen_Byte(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWorld_UInt8, ToGetLinesFrom);
	}
	bool IsThisMemberFunc() const
	{
		return Name.Token->Type == TokenType::KeyWorld_ThisMemberFunc;
	}
	String AsString() const
	{
		auto T = Name.Token->Type;
		if (IsPrimitive(T))
		{
			return StringHelper::ToString(T);
		}
		return String(Name.Token->Value._String);
	}
	TypeNode(const TypeNode& ToCopyFrom) noexcept
	{
		if (ToCopyFrom.HasMadeToken)
		{
			Name.Token =new Token(*ToCopyFrom.Name.Token);
			bool HasMadeToken = true;
		}
		else
		{
			Name.Token = ToCopyFrom.Name.Token;
		}
	}
	TypeNode(TypeNode&& source) noexcept
	{
		if (source.HasMadeToken)
		{
			Name = std::move(source.Name);
			Generic = std::move(source.Generic);

			HasMadeToken = false;
		}
		HasMadeToken = false;
	}
	~TypeNode() noexcept
	{
		if (HasMadeToken)
		{
			delete Name.Token;
		}
	}
private:
	bool HasMadeToken = false;
};
struct NamedParameterNode :Node
{
	NamedParameterNode() : Node(NodeType::Null)
	{

	}
	TypeNode Type;
	NameNode Name;
};
struct NamedParametersNode :Node
{
	NamedParametersNode() : Node(NodeType::NamedParametersNode)
	{

	}
	AddforNode(NamedParametersNode);
	Vector<NamedParameterNode> Parameters;
};
struct ValueParametersNode :Node
{
	ValueParametersNode() : Node(NodeType::ValueParametersNode)
	{

	}
	ValueParametersNode(ValueParametersNode&& Source) = default;
	AddforNodeAndWithList(ValueParametersNode);
};
struct AttributeNode :Node
{
	AttributeNode() : Node(NodeType::AttributeNode)
	{

	}
	AddforNode(AttributeNode);

	ScopedNameNode ScopedName;
	ValueParametersNode Parameters;
};
struct StatementsNode :Node
{
	StatementsNode() : Node(NodeType::StatementsNode)
	{

	}
	AddforNodeAndWithList(StatementsNode);
	StatementsNode(StatementsNode&& source) = default;
	StatementsNode& operator=(StatementsNode&& source) = default;
};
struct FuncSignatureNode :Node
{
	NameNode Name;
	GenericValuesNode Generic;
	NamedParametersNode Parameters;
	TypeNode ReturnType;
};
struct FuncBodyNode :Node
{
	StatementsNode Statements;
};
struct FuncNode :Node
{
	FuncNode() : Node(NodeType::FuncNode)
	{

	}
	~FuncNode()
	{

	}

		
	AddforNode(FuncNode);
	FuncSignatureNode Signature;
	Optional<FuncBodyNode> Body;
};

struct AsmBlockNode :Node
{
	AsmBlockNode() : Node(NodeType::AsmBlockNode)
	{

	}
	AddforNode(AsmBlockNode);
	String_view AsmText;
};

struct ExpressionNodeType :Node
{
	ExpressionNodeType() : Node(NodeType::ExpressionNodeType)
	{

	}
	AddforNode(ExpressionNodeType);
	Node* Value = nullptr;
	ExpressionNodeType(const ExpressionNodeType& ToCopyFrom) = delete;
	ExpressionNodeType(ExpressionNodeType&& source) noexcept
	{
		Value = source.Value;
		source.Value = nullptr;
	}

	~ExpressionNodeType()noexcept
	{
		if (Value) {
			delete Value;
		}
	}
};

struct ValueExpressionNode :Node
{
	ValueExpressionNode() : Node(NodeType::ValueExpressionNode)
	{

	}
	AddforNode(ValueExpressionNode);
	Node* Value = nullptr;
	ValueExpressionNode(const ValueExpressionNode& ToCopyFrom) = delete;
	ValueExpressionNode(ValueExpressionNode&& source)noexcept
	{
		Value = source.Value;
		source.Value = nullptr;
	}

	~ValueExpressionNode()noexcept
	{
		delete Value;
	}
};
struct BinaryExpressionNode :Node
{
	BinaryExpressionNode() : Node(NodeType::BinaryExpressionNode)
	{

	}
	AddforNode(BinaryExpressionNode);

	ExpressionNodeType Value0;
	const Token* BinaryOp =nullptr;
	ExpressionNodeType Value1;
}; 

struct RetStatementNode :Node
{
	RetStatementNode() : Node(NodeType::RetStatementNode)
	{

	}
	AddforNode(RetStatementNode);
	ExpressionNodeType Expression;
};

struct DeclareVariableNode :Node
{
	DeclareVariableNode() : Node(NodeType::DeclareVariableNode)
	{

	}
	AddforNode(DeclareVariableNode);
	TypeNode Type;
	NameNode Name;
	ExpressionNodeType Expression;
	DeclareVariableNode(DeclareVariableNode&& source) = default;
	DeclareVariableNode& operator=(DeclareVariableNode&& source) = default;
};

struct AssignVariableNode :Node
{
	AssignVariableNode() : Node(NodeType::AssignVariableNode)
	{

	}
	AddforNode(AssignVariableNode);
	NameNode Name;
	ExpressionNodeType Expression;
};

struct DeclareStaticVariableNode :Node
{
	DeclareStaticVariableNode() : Node(NodeType::DeclareStaticVariableNode)
	{

	}
	AddforNode(DeclareStaticVariableNode);
	DeclareVariableNode Variable;
};
struct DeclareThreadVariableNode :Node
{
	DeclareThreadVariableNode() : Node(NodeType::DeclareThreadVariableNode)
	{

	}
	AddforNode(DeclareThreadVariableNode);
	DeclareVariableNode Variable;
};

struct AliasNode :Node
{
	AliasNode() : Node(NodeType::AliasNode)
	{

	}
	AddforNode(AliasNode);

	NameNode AliasName;
	GenericValuesNode Generic;
	TypeNode Type;
};
struct EnumValueNode :Node
{
	
	NameNode Name;
	ExpressionNodeType Expression;
};
struct EnumNode :Node
{
	EnumNode() : Node(NodeType::EnumNode)
	{

	}
	AddforNode(EnumNode);

	NameNode EnumName;
	Vector<EnumValueNode> Values;
	TypeNode BaseType;
};

struct TagTypeNode :Node
{
	TagTypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	AddforNodeAndWithList(TagTypeNode);

	NameNode AttributeName;

	TagTypeNode(TagTypeNode&& source) = default;
	TagTypeNode& operator=(TagTypeNode&& source) = default;
};

struct IfNode :Node
{
	IfNode() : Node(NodeType::IfNode)
	{

	}
	AddforNode(IfNode);

	ExpressionNodeType Expression;
	StatementsNode Body;
};
struct ElseNode :Node
{
	ElseNode() : Node(NodeType::ElseNode)
	{

	}
	AddforNode(ElseNode);

	ExpressionNodeType Expression;
	StatementsNode Body;
};

UCodeLangEnd