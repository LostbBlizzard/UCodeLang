#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "UCodeLang/LangCore/ScopeHelper.hpp"
UCodeLangStart

struct ScopeHelper2
{
public:
	Vector<String> Useings;
	ScopeHelper _Scope;


	void AddUseing(const String_view& Name){Useings.push_back(Name.data());}
	void AddUseing(const String& Name) { Useings.push_back(Name); }
	void ClearUseings(){Useings.clear();}

	size_t GetUseingIndex() { return Useings.size() - 1; }
	void RemovePopUseing(size_t Index)
	{
	
	}

	void AddScope(const String_view& Name) { _Scope.AddScope(Name); }
	void RemoveScope() { _Scope.ReMoveScope(); }
};
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
	ScopeHelper2 _Sc;
	std::stack<ClassData*> _ClassStack;

	void Pass();
	void OnFileNode(UCodeLang::FileNode* const& File);
	void OnClassNode(const ClassNode& node);
	void OnUseingNode(const UsingNode& node);
	void OnFuncNode(const FuncNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node);
	void CheckBackEnd();
};
UCodeLangEnd

