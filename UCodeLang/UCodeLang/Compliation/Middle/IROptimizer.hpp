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
	bool Optimization_RemoveUnseddVarables = false;
	bool Optimization_ConstantFoldVarables = false;
	bool Optimization_RemoveUnusePars = false;
	bool Optimization_RemoveFuncsWithSameBody = false;
	void ResetOptimizations()
	{
		Optimization_ShortFuncInline = false;
		Optimization_RemoveUnseddVarables = false;
		Optimization_ConstantFoldVarables = false;
		Optimization_RemoveUnusePars = false;
		Optimization_RemoveFuncsWithSameBody = false;
	}
	//
	

	

	void UpdateOptimizationList();
	void UpdateCodePass();
	void UpdateCodePassFunc(IRFunc* Func);
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