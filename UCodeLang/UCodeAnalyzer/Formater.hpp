#pragma once
#include "Typedef.hpp"
#include "UCodeLang/Compliation/Front/Lexer.hpp"
#include "UCodeLang/Compliation/Front/Parser.hpp"
UCodeAnalyzerStart

namespace UF = UCodeLang::FrontEnd;

class Formater
{
public:
	enum class StrScope
	{
		FileScope,
		NameSpace,
		ClassSpace,
		FuncSpace,
		StatementsSpace,
	};

	Formater();
	~Formater();
	void Reset();

	Optional<String> Format(StrScope scope, StringView Text);

	//Boring settings
	size_t IndentSpaces = 2;


	const String& Get_Output()
	{
		return _Output;
	}
	
	//if need to more specific.
	void ReFormat(const UCodeLang::FileNode& Node);
	void ReFormat(const UF::ImportStatement& Node);
	void ReFormat(const UF::ClassNode& Node);
	void ReFormat(const UF::ScopedNameNode& Node);

	void ReFormat(const UF::GenericValuesNode& Node);
	void ReFormat(const UF::GenericValueNode& Node);
	void ReFormat(const UF::InheritedTypeData& Node);


	void ReFormat(const UF::DeclareVariableNode& Node);
	void ReFormat(const UF::DeclareStaticVariableNode& Node);
	void ReFormat(const UF::DeclareThreadVariableNode& Node);
	void ReFormat(const UF::DeclareEvalVariableNode& Node);
	void ReFormat(const UF::TypeNode& Node);
	void ReFormat(const UF::UseGenericsNode& Node);
	void ReFormat(const UF::ExpressionNodeType& Node);
	void ReFormat(const UF::ValueExpressionNode& Node);
	void ReFormat(const UF::NumberliteralNode& Node);
private:
	
	String _Output;
	void CantReFormat()
	{
		ReFormatFail = true;
	}
	size_t IndentCount = 0;
	Stack<UCodeLang::AccessModifierType> _CurintAccess;

	UCodeLang::AccessModifierType Get_CurintAccess()
	{
		return _CurintAccess.size() ? _CurintAccess.top() : UCodeLang::AccessModifierType::Default;
	}


	void ReFormat(UCodeLang::AccessModifierType Type);

	Stack<size_t> _AccessIndent;
	void SetAccessIndent()
	{
		_AccessIndent.push(IndentCount);
		_CurintAccess.push(UCodeLang::AccessModifierType::Default);
	}
	void RemoveAccessIndent()
	{
		_AccessIndent.pop();
		_CurintAccess.pop();
	}
	size_t GetAccessIndent()
	{
		return  _AccessIndent.size() ? _AccessIndent.top() : 0;
	}

	void AddIndentCount()
	{
		IndentCount++;
	}
	void RemoveIndentCount()
	{
		IndentCount++;
	}


	void AddIndentsFromCount()
	{
		AddIndents(IndentCount);
	}
	void AddIndents(size_t Count)
	{
		for (size_t i = 0; i < Count; i++)
		{
			AddIndent();
		}
	}
	void AddIndent()
	{
		for (size_t i = 0; i < IndentSpaces; i++)
		{
			_Output += " ";
		}
	}
	void AddNextLine()
	{
		_Output += "\n";
	}

	void AddToOutput(const StringView Text)
	{
		_Output += (String)Text;
	}
	void AddToOutput(const String& Text)
	{
		_Output += Text;
	}
	void AddToOutput(const char* Text)
	{
		_Output += Text;
	}
	void NextLineAndIndentsFromCount()
	{
		AddNextLine();
		AddIndentsFromCount();
	}

	void RemoveIndent()
	{
		RemoveLastChar(IndentSpaces);
	}


	void RemoveLastChar(size_t Count)
	{
		for (size_t i = 0; i < Count; i++)
		{
			_Output.pop_back();
		}
	}

	void RemoveLastChar()
	{
		_Output.pop_back();
	}
	
	bool ReFormatFail = false;
};
UCodeAnalyzerEnd