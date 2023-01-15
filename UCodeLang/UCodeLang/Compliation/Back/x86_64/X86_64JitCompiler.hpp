#pragma once
#include "../Jit/Jit.hpp"

UCodeLangStart
class X86_64JitCompiler
{
public:
	X86_64JitCompiler();
	~X86_64JitCompiler();
	void Reset();


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output);


private:

};
UCodeLangEnd