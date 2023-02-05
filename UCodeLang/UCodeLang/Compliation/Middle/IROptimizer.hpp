#pragma once
#include "IRCode.hpp"
#include "../Helpers/CompliationErrors.hpp"
UCodeLangStart
class IROptimizer
{
public:
	IROptimizer(){}
	~IROptimizer(){}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }


	void Optimized(const IRBuilder& IRCode);
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
};

UCodeLangEnd