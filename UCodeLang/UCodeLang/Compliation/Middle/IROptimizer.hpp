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
	Optional<size_t> FuncBodyHash;
	InlineState Inlinestate= InlineState::Null;
	bool BodyWasRemoved = false;
	bool FuncBodyWasUpdated = false;
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
	
	bool IsIsolatedandIndependent = false;
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	IRTypeFixer _TypeFixer;
	IRBuilder* Input = nullptr;
	IRFunc* LookAtfunc = nullptr;
	bool _UpdatedCode = false;
	void UpdatedCode() { _UpdatedCode = true; }
	void UpdatedCodeFor(IRFunc* Func) {
		_UpdatedCode = true;
		auto& FuncData = Funcs[Func];
		FuncData.FuncBodyWasUpdated = true;
	}
	//
	bool Optimization_ShortFuncInline = false;
	bool Optimization_RemoveUnsedVarables = false;
	bool Optimization_ConstantFoldVarables = false;
	bool Optimization_RemoveUnusePars = false;
	bool Optimization_RemoveFuncsWithSameBody = false;
	bool Optimization_IndirectMemeberToDirectMemeber = false;
	bool Optimization_DestructureStructMembers = false;
	bool Optimization_ConstantFuncPtrToDirectCall = false;
	bool Optimization_LowerToBodysToCFuncions = false;//Working On
	bool Optimization_InlineConditionalJump = false;
	bool Optimization_RemoveUnreachable = false;
	bool Optimization_ReorderFuncionsInToHotSpots = false;
	bool Optimization_ReorderInstructionsBaseOnMemoryAndHotSpots = false;
	bool Optimization_LoopUnrolling = false;
	bool Optimization_LoopFrequencyReduction = false;
	bool Optimization_LoopFrequencyReduction = false;
	bool Optimization_StaticAndThreadRemovealAndReadOnlyToValues = false;
	bool Optimization_RemoveUnneedMeallocAndFree = false;
	void ResetOptimizations()
	{
		Optimization_ShortFuncInline = false;
		Optimization_RemoveUnsedVarables = false;
		Optimization_ConstantFoldVarables = false;
		Optimization_RemoveUnusePars = false;
		Optimization_RemoveFuncsWithSameBody = false;
		Optimization_IndirectMemeberToDirectMemeber = false;
		Optimization_DestructureStructMembers = false;
		Optimization_ConstantFuncPtrToDirectCall = false;
		Optimization_LowerToBodysToCFuncions = false;
		Optimization_InlineConditionalJump = false;
		Optimization_RemoveUnreachable = false;
		Optimization_ReorderFuncionsInToHotSpots = false;
		Optimization_ReorderInstructionsBaseOnMemoryAndHotSpots = false;
		Optimization_LoopFrequencyReduction = false;
		Optimization_StaticAndThreadRemovealAndReadOnlyToValues = false;
		Optimization_RemoveUnneedMeallocAndFree = false;
	}
	//
	

	

	void UpdateOptimizationList();
	void UpdateCodePass();
	void UpdateCodePassFunc(IRFunc* Func);
	void DoDestructurStructMembers(UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins, UCodeLang::IRStruct* structtype, UCodeLang::IRFunc* Func, UCodeLang::Unique_ptr<UCodeLang::IRBlock>& Block, const size_t& i);
	void CopyFuncionWithoutUnusedParAndUpdateOtherCalls(UCodeLang::IRPar& Par, UCodeLang::IRFunc* Func, const size_t& i);
	void UpdateCallWhenParWasRemoved(IRFunc* Item, const IRFunc* Func, const IRFunc& NewFunc, size_t i);
	void DoInlines(IRFunc* Func);
	void DoInlines(IRFunc* Func, IRBlock* Block);
	void ConstantFoldOperator(IRInstruction& I,IROperator& Value,ReadOrWrite OpType);

	struct SSAState
	{
		VectorMap<IROperator*, IROperator> Updated;
		VectorMap<IROperator, IRInstruction*> Map;
	};

	void ToSSA(const IRFunc* Func, SSAState& state);
	void UndoSSA(const IRFunc* Func, const SSAState& state);

	struct InLineData
	{
		IRFunc* Func =nullptr;
		IRBlock* Block = nullptr;
		size_t CallIndex = 0;
		Vector<IRInstruction*> InlinedPushedPars;
		Vector<IRInstruction*>* AddIns = nullptr;
	};

	void InLineFunc(InLineData& Data);
	void InLineSubOperator(InLineData& Data, IROperator& Op,size_t Offset);

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