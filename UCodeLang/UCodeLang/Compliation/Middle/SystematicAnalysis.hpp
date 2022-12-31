#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
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

	UCodeLangForceinline UClib& Get_Output()
	{
		return _Lib;
	}
	UCodeLangForceinline void Set_BackEnd(const BackEndInterface* Value)
	{
		BackEnd = Value;
	}
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Lib;
	const BackEndInterface* BackEnd = nullptr;
	BackEndInterface::BackendPtr BackEndObject = nullptr;

	const Vector<FileNode*>* _Files =nullptr;
	const Vector<UClib*>* _Libs = nullptr;

	void TypePass();
	void TypePass_ClassNode(const ClassNode& node);
	void CheckBackEnd();
};
UCodeLangEnd

