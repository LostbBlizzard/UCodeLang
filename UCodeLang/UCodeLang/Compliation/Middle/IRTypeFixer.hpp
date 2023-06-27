#pragma once
#include "IR.hpp"
#include "../Helpers/CompliationErrors.hpp"

UCodeLangStart

class IRTypeFixer
{
public:
	IRTypeFixer()
	{

	}
	~IRTypeFixer()
	{

	}
	void Reset();
	void FixTypes(IRBuilder* Input);
	void OnFunc(IRFunc* Func);
	void GetMemberAccessTypeForIns(UCodeLang::IRType& ClassType, UCodeLang::IRBuilder* Input, UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins);

	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _Errs = V; }
private:
	IRBuilder* _Input=nullptr;
	CompliationErrors* _Errs=nullptr;
	IRFunc* _Func = nullptr;
	IRBlock* _Block = nullptr;
	bool InList(const IRPar* Par,const Vector<IRPar>& Pars)
	{
		for (auto& Item : _Func->Pars)
		{
			if (Par == &Item)
			{
				return true;
			}
		}
		return false;
	}
	bool InBlock(const IRInstruction* IR)
	{
		bool Has = false;
		for (auto& Item : _Block->Instructions)
		{
			if (Item.get() == IR)
			{
				return true;
			}
		}
		return false;
	}

	void OnOp(IRInstruction& Ins, IROperator& Op);
	void LogCantFindInsInBlock();
	void LogErrorCantFindPar(UCodeLang::IROperator& Op);
};
UCodeLangEnd