#pragma once
#include "../../LangCore.hpp"
#include "TreeNodeTypes.hpp"
#include "..//..//LangCore/TypeNames.hpp"
#include "../..//LangCore/ScopeHelper.hpp"
UCodeLangStart

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
struct ClassNode
{
	AddforNodeAndWithList(ClassNode);

	NameNode ClassName;
};


struct UsingNode
{
	AddforNodeAndWithList(UsingNode);

	ScopedNameNode ScopedName;
};

struct ParameterValueNode
{
	NameNode Type;
	String_view Name;
};
struct ParametersNode
{
	Vector<ParameterValueNode> Parameters;
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
struct TypeNode
{
	NameNode Name;
	GenericValuesNode Generic;

	
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
	NameNode Name;
	TypeNode Type;
};
struct NamedParametersNode
{
	AddforNode(NamedParametersNode);
	Vector<NamedParameterNode> Statements;
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
struct DeclareVariableNode
{
	AddforNode(DeclareVariableNode);
};
struct DeclareStaticVariableNode
{
	AddforNode(DeclareStaticVariableNode);
	DeclareVariableNode Variable;
};
struct AsmBlockNode
{
	AddforNode(AsmBlockNode);
	String_view AsmText;
};

UCodeLangEnd