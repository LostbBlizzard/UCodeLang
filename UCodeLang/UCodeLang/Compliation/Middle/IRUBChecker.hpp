#pragma once

#include "IR.hpp"
#include "../Helpers/CompliationErrors.hpp"

UCodeLangStart
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

	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _Errs = V; }
private:
	const IRBuilder* _Input = nullptr;
	CompliationErrors* _Errs = nullptr;
};

UCodeLangEnd