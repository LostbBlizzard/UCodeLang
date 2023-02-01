#pragma once

#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart
class SystematicAnalysis;
class IRBuilder;
class SymbolTable;




class BackEndObject
{
public:
	virtual void Reset() {}
	virtual void Build(const IRBuilder* Input) {};
	virtual ~BackEndObject() {}

	inline CompliationErrors& Get_ErrorsOutput()
	{
		return *_ErrorsOutput;
	}
	inline CompliationSettings& Get_Settings()
	{
		return *_Settings;
	}

	UCodeLangForceinline UClib& Getliboutput()
	{
		return _Output;
	}


	inline void Set_ErrorsOutput(CompliationErrors* Errors)
	{
		_ErrorsOutput = Errors;
	}
	inline void Set_Settings(CompliationSettings* setting)
	{
		_Settings = setting;
	}
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Output;
};

using BackEndObject_Ptr = BackEndObject* (*)();
UCodeLangEnd

