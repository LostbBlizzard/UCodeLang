#pragma once
#include "X86Gen.hpp"
UCodeLangStart
class X86BackEnd:BackEndObject
{
	X86BackEnd(){}
	~X86BackEnd(){}

	void Reset() override
	{

	}
	void Build(const IRBuilder* Input) override
	{

	}
	X86Gen _CodeGen;
};
UCodeLangEnd
