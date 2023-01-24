#pragma once
#include "../Jit/Jit.hpp"

UCodeLangStart
class X86JitCompiler
{
public:
	X86JitCompiler();
	~X86JitCompiler();
	void Reset();

	
	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X86Output);


private:
	
};
UCodeLangEnd

