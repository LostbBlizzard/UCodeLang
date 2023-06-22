#pragma once
#include "../Jit/Jit.hpp"
#include "X86Gen.hpp"
UCodeLangStart
class X86JitCompiler
{
public:
	X86JitCompiler();
	~X86JitCompiler();
	void Reset();

	
	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X86Output);

	Vector<NullJitCalls> NullCalls;
	UAddress OnUAddressPar = 0;
private:
	X86Gen _Gen;
};
UCodeLangEnd

