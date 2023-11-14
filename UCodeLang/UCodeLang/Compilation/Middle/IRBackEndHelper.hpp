#pragma once
#ifndef UCodeLangNoCompiler
#include "IR.hpp"

UCodeLangStart

struct IRAndOperator
{
	const IRInstruction* Ins = nullptr;
	const IROperator* Op = nullptr;
	inline const IROperator& GetOp() const
	{
		return *Op;
	}
	IRAndOperator()
	{

	}
	IRAndOperator(
		const IRInstruction* Ins,
		const IROperator* Op)
	{
		this->Ins = Ins;
		this->Op = Op;
	}
	bool operator==(const IRAndOperator& Other)
	{
		return this->Ins == Other.Ins
			&& this->Op == Other.Op;
	}
	bool operator!=(const IRAndOperator& Other)
	{
		return !this->operator==(Other);
	}
};
UCodeLangEnd

#endif