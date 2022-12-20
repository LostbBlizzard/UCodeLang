#pragma once
#include "../../LangCore.hpp"
#include "Parser.hpp"
#include "../CompliationSettings.hpp"
#include "../../LangCore/ReflectionData.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart



class SemanticAnalysisRet
{
public:
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
	void BuildAttributeNode(const UCodeLang::AttributeNode& Node);
	void BuildFunc(const UCodeLang::FuncNode& Node);
	void BuildParameter(const UCodeLang::NamedParameterNode& BodyStatements);
	void BuildStatements(const UCodeLang::StatementsNode& BodyStatements);
	void BuildStatement(const UCodeLang::Node* Statement);
	void BuildExpressionType(const UCodeLang::Node* Statement);
	void BuildExpressionValue(const UCodeLang::Node* Item);
	void BuildUnaryExpression(const UCodeLang::Node* Item);
	void BuildBinaryExpression(const UCodeLang::Node* Item);
	//Statements
	void BuildAsmBlock(const UCodeLang::AsmBlockNode& Asm);
	//Helpers
	void BuildReturnExpression(const RetStatementNode& Item);
	void BuildStoreExpression(const String_view& VarName);

	void BuildStaticDeclareVariable(const DeclareStaticVariableNode& Item);
	void BuildDeclareThreadVariable(const DeclareThreadVariableNode& Item);
	void BuildDeclareVariable(const DeclareVariableNode& Item, Intermediate_Set VarType = Intermediate_Set::DeclareVar);
	void BuildType(const TypeNode& Item);
};
UCodeLangEnd
