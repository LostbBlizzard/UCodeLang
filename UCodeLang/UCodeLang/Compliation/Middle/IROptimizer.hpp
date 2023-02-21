#pragma once
#include "IR.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
#include "IRTypeFixer.hpp"
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
	IRTypeFixer _TypeFixer;
	IRBuilder* Input = nullptr;
	bool _UpdatedCode = false;
	void UpdatedCode() { _UpdatedCode = true; }

	struct OptimizationState
	{
		bool IsActive = false;
	};
	Array<OptimizationState, OptimizationCount> OptimizationListState;

	void UpdateOptimizationList();
	void UpdateCodePass();
	void ConstantFoldOperator(IRInstruction& I,IROperator& Value);

	struct MyStruct
	{
		bool IsReferenced = false;
	};
	Unordered_map<IRInstruction*, MyStruct> IRData;
	auto& Get_IRData(IRInstruction* V)
	{
		return  IRData[V];
	}
};

UCodeLangEnd