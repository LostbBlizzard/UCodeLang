#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "Symbol.hpp"
UCodeLangStart

class SystematicAnalysis
{

public:
	SystematicAnalysis() {}
	~SystematicAnalysis() {}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }


	bool Analyze(const FileNode& File);
	bool Analyze(const Vector<FileNode*>& Files, const Vector<UClib*>& Libs);

	void BuildCode();

	UCodeLangForceinline UClib& Get_Output()
	{
		return _Lib;
	}
	UCodeLangForceinline void Set_BackEnd(const BackEndInterface* Value)
	{
		BackEnd = Value;
	}

	enum class PassType : UInt8
	{
		Null,
		GetTypes,
		FixedTypes,
		BuidCode,
	};

private:
	
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Lib;
	const BackEndInterface* BackEnd = nullptr;
	BackEndInterface::BackendPtr BackEndObject = nullptr;


	PassType passtype = PassType::Null;

	const Vector<FileNode*>* _Files =nullptr;
	const Vector<UClib*>* _Libs = nullptr;
	SymbolTable _Table;
	std::stack<ClassData*> _ClassStack;
	Vector<const AttributeNode*> _TepAttributes;
	bool _InStatements = false;
	//
	IRBuilder _Builder;
	IRSeg _LastExpression;
	IRField _LastExpressionField;
	//

	void Pass();
	void OnFileNode(UCodeLang::FileNode* const& File);
	void OnClassNode(const ClassNode& node);
	void OnUseingNode(const UsingNode& node);
	void OnFuncNode(const FuncNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	void OnAttributeNode(const AttributeNode& node);
	void OnNonAttributeable(size_t Line, size_t Pos);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node);
	void OnAssignVariableNode(const AssignVariableNode& node);
	void OnExpressionTypeNode(const Node* node);
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnExpressionNode(const BinaryExpressionNode& node);
	void CheckBackEnd();
	void PushTepAttributesInTo(Vector<AttributeData>& Input);
	void LoadLibSymbols();
	void LoadLibSymbols(const UClib& lib);
	UCodeLangForceinline auto OutputType()
	{
		return  _Settings->_Type;
	}
	UCodeLangForceinline auto OutputTypeAsLibType()
	{
		return   OutputType() == OutPutType::Lib ? LibType::Lib : LibType::Dll;
	}
	Symbol* GetSymbol(String_view Name, SymbolType Type);
};
UCodeLangEnd

