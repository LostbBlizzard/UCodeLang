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
private:
	IRBuilder* _Input;
	CompliationErrors* _Errs;
	void OnOp(IRInstruction& Ins, IROperator& Op);
};
UCodeLangEnd