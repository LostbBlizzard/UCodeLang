#pragma once
#include "../../LangCore.hpp"
#include "Parser.hpp"
#include "../CompliationSettings.hpp"
#include "../../LangCore/ReflectionData.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

using symbolType_t = UInt8;
enum class symbolType : symbolType_t
{
	Null,
	Class,
	Func,
	Var,
	
	Parameter,
	Parameter_Input,//A parameter but its not on the stack its in the input reg
	EnumValue,
	ThisVar,
	This,
	StaticVar,
	ExternalValue,
	ClassValue,
	
};
class symbol
{
public:
	String Name;
	String VarType;
	symbolType Type;
	
	UAddress AddressInLib;
	UAddress Address; 
	UAddress Size;
	
	size_t Line;
	size_t Pos;
	symbol(const String& N):Name(N), Type(symbolType::Null)
		, AddressInLib(0)
		, Address(0)
		, Size(0)
		, Line(0),Pos(0)
	{

	}

	static constexpr UAddress Size_cant_be_determined = 0;
};

class SemanticAnalysisData
{
public:
	Vector<symbol> Symbols;
	Vector<String_view> Use_;

	UCodeLangForceinline void AddUseing(const String_view& Name)
	{
		Use_.push_back(Name);
	}

	symbol* FindSymbol(const String_view& Name, const String_view& Scope = ScopeHelper::_globalScope)
	{
		String TepScope = Scope.data();

		while (true)
		{
			String FullName = TepScope + Name.data();
			for (auto& item : Symbols)
			{
				if (item.Name == FullName)
				{
					return &item;
				}
			}

			if (TepScope == ScopeHelper::_globalScope) { break; }
			ScopeHelper::ReMoveScope(TepScope);
		}
		return nullptr;
	}
};
class SemanticAnalysisRet
{
public:
	SemanticAnalysisData Symbols;
	UClib Lib;
};



class SemanticAnalysis
{
public:
	SemanticAnalysis(){}
	~SemanticAnalysis() {}
	void Reset();

	void DoAnalysis(const FileNode& Tree);

	UCodeLangForceinline bool Get_Success()
	{
		return  _Success;
	}
	UCodeLangForceinline SemanticAnalysisRet& Get_SemanticAnalysisRet()
	{
		return Value;
	}
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	SemanticAnalysisRet Value;
	bool _Success = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings * _Settings = nullptr;
	//
	ScopeHelper Scope;
	std::stack<ClassData*> OnClass;
	Vector<String> TepAttributes;
	Instruction _Ins;
	UCodeLangForceinline void ReSetIns()
	{
		_Ins = Instruction();
	}


	symbol& AddSymbol(const String& Name)
	{
		Value.Symbols.Symbols.push_back(symbol(Name));
		return Value.Symbols.Symbols.back();
	};
	symbol& AddSymbol(const String& Name, const Token& token)
	{
		Value.Symbols.Symbols.push_back(symbol(Name));
		auto V = Value.Symbols.Symbols.back();
		V.Line = token.OnLine;
		V.Pos = token.OnPos;
		return V;
	};
	symbol& AddSymbol(const String& Name, const Token* node, symbolType Type)
	{
		Value.Symbols.Symbols.push_back(symbol(Name));
		auto& V = Value.Symbols.Symbols.back();
		if (node)
		{
			V.Line = node->OnLine;
			V.Pos = node->OnPos;
		}
		V.Type = Type;
		return V;
	};
	symbol* FindSymbol(const String_view& Name)
	{
		return Value.Symbols.FindSymbol(Name, Scope.ThisScope);
	}

	unordered_map<String_view, UAddress> _Strings;
	String TepString;
	UCodeLangForceinline void ClearTepString()
	{
		TepString.clear();
	}
	UAddress AddDebug_String(const String_view& String)
	{
		if (_Strings.count(String))
		{
			return _Strings.at(String);
		}
		else
		{
			return _Strings[String] = Value.Lib.AddDebugBytes(String);
		}
	}
	Vector<const Node*> _StaticVariables;

	void BuildStaticVariable(const UCodeLang::Node* node);
	void BuildNameSpace(const UCodeLang::Node* Tree);
	void BuildUseingNode(const UCodeLang::UsingNode& Item);
	void BuildClass(const UCodeLang::ClassNode& Node);
	void BuildFunc(const UCodeLang::FuncNode& Node);
	void BuildStatements(const UCodeLang::StatementsNode& BodyStatements);
	void BuildStatement(const UCodeLang::Node* Statement);
	void BuildExpressionType(const UCodeLang::Node* Statement);
	void BuildExpressionValue(const UCodeLang::Node* Item);
	void BuildUnaryExpression(const UCodeLang::Node* Item);
	void BuildBinaryExpression(const UCodeLang::Node* Item);
	//Statements
	void BuildAsmBlock(const UCodeLang::AsmBlockNode& Asm);
	//Helpers
	void BuildReturnExpression();
	void BuildStoreExpression(const String_view& VarName);
	void BuildDeclareVariable(const String_view& VarName, const String_view& Type);
};
UCodeLangEnd
