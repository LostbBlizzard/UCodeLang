#pragma once

#ifndef UCodeLangNoCompiler
#include "IR.hpp"
#include "../Helpers/CompilationErrors.hpp"


UCodeLangStart

struct CompilationSettings;

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
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _Errs = V; }
	UCodeLangForceinline void Set_CompilerSetting(CompilationSettings* V) { _Set = V; }
private:
	IRBuilder* _Input=nullptr;
	CompilationErrors* _Errs=nullptr;
	CompilationSettings* _Set = nullptr;
	IRFunc* _Func = nullptr;
	IRBlock* _Block = nullptr;
	void OnFunc(IRFunc* Func);
	void GetMemberAccessTypeForIns(UCodeLang::IRType& ClassType, UCodeLang::IRBuilder* Input, UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins);
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

	void OnOp(IRInstruction& Ins, IROperator& Op,bool UpdateInsType =true);
	void LogCantFindInsInBlock(IRInstruction* Ins);
	void LogErrorCantFindPar(UCodeLang::IROperator& Op);
};
UCodeLangEnd

#endif