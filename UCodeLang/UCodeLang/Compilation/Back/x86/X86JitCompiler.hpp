#pragma once
#include "../Jit/Jit.hpp"
#include "X86Builder.hpp"
UCodeLangStart
class X86JitCompiler :public JitCompiler
{
public:


	X86JitCompiler();
	~X86JitCompiler();
	void Reset() JitCompilerOverride;


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X86Output) JitCompilerOverride;
	void SubCall(FuncType Value, uintptr_t CPPOffset, void* Output) JitCompilerOverride;
private:
	X86Builder _Gen;
};
UCodeLangEnd

