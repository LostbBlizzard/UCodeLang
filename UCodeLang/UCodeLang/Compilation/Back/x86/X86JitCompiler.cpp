#include "X86JitCompiler.hpp"
UCodeLangStart
X86JitCompiler::X86JitCompiler()
{
}
X86JitCompiler::~X86JitCompiler()
{
}
void X86JitCompiler::Reset()
{
}
bool X86JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X86Output)
{
	return false;
}
void  X86JitCompiler::SubCall(FuncType Value, uintptr_t CPPOffset, void* X64Output)
{
	UCodeLangUnreachable();
}
UCodeLangEnd