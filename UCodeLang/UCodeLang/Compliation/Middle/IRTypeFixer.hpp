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
	void GetMemberAccessTypeForIns(UCodeLang::IRType& ClassType, UCodeLang::IRBuilder* Input, UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins);
private:
	IRBuilder* _Input=nullptr;
	CompliationErrors* _Errs=nullptr;
	void OnOp(IRInstruction& Ins, IROperator& Op);
};
UCodeLangEnd