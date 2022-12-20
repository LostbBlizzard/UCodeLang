#pragma once
#include "../../LangCore.hpp"
#include "TreeNodeTypes.hpp"
#include "..//..//LangCore/TypeNames.hpp"
#include "../..//LangCore/ScopeHelper.hpp"
UCodeLangStart

struct StringliteralNode
{
	AddforNode(StringliteralNode);

	const Token* Token = nullptr;
};



struct BoolliteralNode
{
	AddforNode(BoolliteralNode);

	bool Value = false;
};
struct NumberliteralNode
{
	AddforNode(NumberliteralNode);

	const Token* Token = nullptr;
};

struct NameNode
{
	AddforNode(NameNode);

	const Token* Token = nullptr;
};
struct ScopedNameNode
{
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

struct ReadVariableNode
{
	AddforNode(ReadVariableNode);

	ScopedNameNode VariableName;
};
struct NamespaceNode
{
	AddforNodeAndWithList(NamespaceNode);

	ScopedNameNode NamespaceName;
};
struct GenericValueNode 
{
	const Token* Token = nullptr;
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
	
	
	static void Gen_Type(TypeNode& Out,TokenType Type, const Token& ToGetLinesFrom)
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
	static void Gen_Byte(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWorld_UInt8, ToGetLinesFrom);
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
	const Token* BinaryOp =nullptr;
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
struct DeclareThreadVariableNode
{
	AddforNode(DeclareThreadVariableNode);
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
	NameNode Name;
	Node* Expression = nullptr;//Can be null.
};
struct EnumNode
{
	AddforNode(EnumNode);

	NameNode EnumName;
	Vector<EnumValueNode> Values;
	TypeNode BaseType;
};

struct AttributeTypeNode
{
	AddforNodeAndWithList(AttributeTypeNode);

	NameNode AttributeName;
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