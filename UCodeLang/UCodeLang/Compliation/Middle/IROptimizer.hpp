#pragma once
#include "IR.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
UCodeLangStart
class IROptimizer
{
public:
	IROptimizer() {}
	~IROptimizer() {}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }


	void Optimized(IRBuilder& IRcode);
	
	
	static constexpr size_t OptimizationCount = 0;
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	IRBuilder* Input = nullptr;

	

	struct OptimizationState
	{
		bool IsActive = false;
	};
	Array<OptimizationState, OptimizationCount> OptimizationListState;

	void UpdateOptimizationList();
};

UCodeLangEnd