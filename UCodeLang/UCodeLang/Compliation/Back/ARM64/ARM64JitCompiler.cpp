#include "ARM64JitCompiler.hpp"
UCodeLangStart

void ARM64JitCompiler::Reset()
{

}
bool ARM64JitCompiler::BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& ArmOutput)
{
	return false;
}
void ARM64JitCompiler::SubCall(FuncType Value, uintptr_t CPPOffset, void* ArmOutput)
{
	return false;
}

UCodeLangEnd