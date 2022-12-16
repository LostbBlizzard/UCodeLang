#pragma once
#include "../../LangCore.hpp"
#include "../UClib.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "SemanticAnalysis.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../Helpers/KeyWords.hpp"
UCodeLangStart


class IntermediateAssembler
{
public:
	void Assemble(SemanticAnalysisRet& Data);
    


	inline UClib& Get_Output() { return _OutPut; }
	inline bool Get_Success() { return _LinkSuccess; }
	IntermediateAssembler() : _LinkSuccess(false), _ErrorsOutput(nullptr), _Settings(nullptr), _Data(nullptr)
	{
		
	}
	inline void Set_ErrorsOutput(CompliationErrors* V)
	{
		_ErrorsOutput = V;
	}
	inline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	UClib _OutPut;
	bool _LinkSuccess;
	CompliationErrors* _ErrorsOutput;
	CompliationSettings* _Settings;
	SemanticAnalysisRet* _Data;
	
	ScopeHelper ThisScope;
	UCodeLang::Instruction _Ins;
	
	unordered_map<String_view, UAddress> _Strings;
	String TepString;
	inline void ClearTepString()
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
			return _Strings[String] = _OutPut.AddDebugBytes(String);
		}
	}
	Vector<const Node*> _StaticVariables;

	void BuildStaticVariable(const UCodeLang::Node* node);
	void BuildNameSpace(const UCodeLang::Node* Tree);
	void BuildClass(const UCodeLang::ClassNode& Node);
	void BuildFunc(const UCodeLang::FuncNode& Node);
	void BuildStatements(const UCodeLang::StatementsNode& BodyStatements);
	void BuildStatement(const UCodeLang::Node* Statement);
	void BuildExpressionType(const UCodeLang::Node* Statement);
	void BuildExpressionValue(const UCodeLang::Node* Item);
	void BuildUnaryExpression(const UCodeLang::Node* Item);
	void BuildBinaryExpression(const UCodeLang::Node* Item);
	//Helpers
	void BuildReturnExpression();
	void BuildStoreExpression(const String_view& VarName);
	void BuildDeclareVariable(const String_view& VarName, const String_view& Type);

};
UCodeLangEnd
