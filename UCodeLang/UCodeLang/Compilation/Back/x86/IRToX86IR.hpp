#pragma once
#ifndef UCodeLangNoCompiler
#include "X86IR.hpp"
#include "X86Builder.hpp"

UCodeLangStart

class IRToX86IR
{
public:
	IRToX86IR() {}
	~IRToX86IR() {};
	static X86IR Into(const IRBuilder& builder, X86IR::CleanUpMode mode = X86IR::CleanUpMode::None)
	{
		X86IR r;

		IRToX86IR c;
		c.Build(&builder, mode);

		r = std::move(c.Get_Output());
		return r;
	}
	void Reset();
	void Build(const IRBuilder* builder, X86IR::CleanUpMode mode = X86IR::CleanUpMode::None);
	X86IR& Get_Output()
	{
		return _Out;
	}
private:
	X86IR _Out;
	const IRBuilder* _Input = nullptr;
};

UCodeLangEnd

#endif