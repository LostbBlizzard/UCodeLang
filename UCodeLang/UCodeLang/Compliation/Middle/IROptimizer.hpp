#pragma once
#include "IR.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
#include "IRTypeFixer.hpp"
UCodeLangStart




enum class InlineState
{
	Null,
	FuncionCantBeInline,
	MayBeInlined,
	AutoInline,
};
enum class FuncionType
{
	Null,
	SimpleFuncion,
	Recursive,
};

struct IROptimizationFuncData
{
public:
	size_t FuncBodyHash = 0;
	InlineState Inlinestate= InlineState::Null;
};



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
	void UpdateCodePassFunc(const IRFunc* Func);
	void ConstantFoldOperator(IRInstruction& I,IROperator& Value);


	struct InLineData
	{
		IRFunc* Func =nullptr;
		IRBlock* Block = nullptr;
		size_t CallIns = 0;
		Vector<IRInstruction*> InlinedPushedPars;
	};

	void InLineFunc(InLineData& Data);
	void InLineSubOperator(InLineData& Data, IROperator& Op);

	struct MyStruct
	{
		bool IsReferenced = false;
	};
	BinaryVectorMap<IRInstruction*, MyStruct> IRData;
	auto& Get_IRData(IRInstruction* V)
	{
		return  IRData[V];
	}
	BinaryVectorMap<IRFunc*, IROptimizationFuncData> Funcs;
};

UCodeLangEnd