#pragma once
#include "../../LangCore.hpp"
#include "TreeNodeTypes.hpp"
#include "..//..//LangCore/TypeNames.hpp"
#include "../..//LangCore/ScopeHelper.hpp"
UCodeLangStart

struct StringliteralNode
{
	AddforNode(StringliteralNode);

	Token* Token = nullptr;
};

struct BoolliteralNode
{
	AddforNode(BoolliteralNode);

	bool Value;
};
struct NumberliteralNode
{
	AddforNode(NumberliteralNode);

	Token* Token = nullptr;
};

struct NameNode
{
	AddforNode(NameNode);

	Token* Token = nullptr;
};
struct ScopedNameNode
{
	AddforNode(ScopedNameNode);

	Vector<Token*> ScopedName;

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
struct NamespaceNode
{
	AddforNodeAndWithList(NamespaceNode);

	ScopedNameNode NamespaceName;
};
struct GenericValueNode 
{
	Token* Token = nullptr;
};
struct GenericValuesNode
{
	Vector<GenericValueNode> Values;

	UCodeLangForceinline bool HasGeneric()
    {
		return Values.size();
	}
};
struct ClassNode
{
	AddforNodeAndWithList(ClassNode);

	NameNode ClassName;
	GenericValuesNode Generic;
};


struct UsingNode
{
	AddforNodeAndWithList(UsingNode);

	ScopedNameNode ScopedName;
};


struct TypeNode
{
	NameNode Name;
	GenericValuesNode Generic;


	static constexpr bool IsType(TokenType Type)
	{
		if (IsPrimitive(Type)){return true;}
		else if (Type == TokenType::Name) { return true; }
		else if (Type == TokenType::KeyWorld_This) { return true; }

		return false;
	}

	static constexpr bool IsPrimitive(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::KeyWorld_UInt8:return true;
		case TokenType::KeyWorld_SInt8:return true;
		default:return false;
		}

	}
	
	static void Gen_void(TypeNode& Value,Token& ToGetLinesFrom)
	{
		auto T = new Token();
		T->OnLine = ToGetLinesFrom.OnLine;
		T->OnPos = ToGetLinesFrom.OnPos;
		Value.HasMadeToken = true;
		Value.Name.Token = T;
	}
	~TypeNode()
	{
		if (HasMadeToken)
		{
			delete Name.Token;
		}
	}
private:
	bool HasMadeToken = false;
};
struct NamedParameterNode
{
	TypeNode Type;
	NameNode Name;
};
struct NamedParametersNode
{
	AddforNode(NamedParametersNode);
	Vector<NamedParameterNode> Parameters;
};
struct ValueParametersNode
{
	AddforNodeAndWithList(ValueParametersNode);
};
struct AttributeNode
{
	AddforNode(AttributeNode);

	ScopedNameNode ScopedName;
	ValueParametersNode Parameters;
};
struct StatementsNode
{
	AddforNodeAndWithList(StatementsNode);
};
struct FuncSignatureNode
{
	NameNode Name;
	GenericValuesNode Generic;
	NamedParametersNode Parameters;
	TypeNode ReturnType;
};
struct FuncBodyNode
{
	StatementsNode Statements;
};
struct FuncNode
{
	AddforNode(FuncNode);
	FuncSignatureNode Signature;
	Optional<FuncBodyNode> Body;
};

struct AsmBlockNode
{
	AddforNode(AsmBlockNode);
	String_view AsmText;
};
struct BinaryExpressionNode
{
	AddforNode(BinaryExpressionNode);

	Node* Value0 = nullptr;
	Token* BinaryOp =nullptr;
	Node* Value1 = nullptr;
	~BinaryExpressionNode()
	{
		delete Value0;
		delete Value1;
	}
};
struct RetStatementNode
{
	AddforNode(RetStatementNode);
	Node* Expression = nullptr;//Can be null.
};

struct DeclareVariableNode
{
	AddforNode(DeclareVariableNode);
	TypeNode Type;
	NameNode Name;
	Node* Expression = nullptr;//Can be null.
};
struct DeclareStaticVariableNode
{
	AddforNode(DeclareStaticVariableNode);
	DeclareVariableNode Variable;
};
struct DeclareStaticforthreadVariableNode
{
	AddforNode(DeclareStaticforthreadVariableNode);
	DeclareVariableNode Variable;
};

struct AliasNode
{
	AddforNode(AliasNode);

	NameNode AliasName;
	GenericValuesNode Generic;
	TypeNode Type;
};
struct EnumValueNode
{
	NameNode AliasName;
	Node* Expression = nullptr;//Can be null.
	TypeNode BaseType;
};
struct EnumNode
{
	AddforNode(EnumNode);

	NameNode AliasName;
	Vector<EnumValueNode> Values;
};

struct AttributeTypeNode
{
	AddforNode(AttributeTypeNode);

	NameNode AttributeName;
	Vector<EnumValueNode> Values;
};

struct IfNode
{
	AddforNode(IfNode);

	Node* Expression = nullptr;
	StatementsNode Body;
};
struct ElseNode
{
	AddforNode(ElseNode);

	Node* Expression = nullptr;
	StatementsNode Body;
};

UCodeLangEnd