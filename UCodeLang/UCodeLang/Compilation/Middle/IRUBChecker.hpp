#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"
UCodeLangStart

class IRBuilder;
class CompilationErrors;

class IRUBChecker
{
public:
	IRUBChecker()
	{

	}
	~IRUBChecker()
	{

	}
	void Reset();
	void CheckForUB(const IRBuilder* Input);

	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _Errs = V; }
private:
	const IRBuilder* _Input = nullptr;
	CompilationErrors* _Errs = nullptr;
};

UCodeLangEnd

#endif