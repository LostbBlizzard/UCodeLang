#pragma once

#include "../../LangCore.hpp"
#include "SemanticAnalysis.hpp"
UCodeLangStart
class Optimizer
{
public:
	Optimizer();
	~Optimizer();

	void Optimiz(SemanticAnalysisRet& ToUpdate);
	inline void Set_ErrorsOutput(CompliationErrors* V){_ErrorsOutput = V;}
	inline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	CompliationErrors* _ErrorsOutput;
	CompliationSettings* _Settings;
};
UCodeLangEnd
