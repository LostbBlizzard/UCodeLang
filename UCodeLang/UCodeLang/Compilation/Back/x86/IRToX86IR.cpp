#ifndef UCodeLangNoCompiler
#include "IRToX86IR.hpp"
UCodeLangStart
void IRToX86IR::Reset()
{
	this->~IRToX86IR();
	new (this) IRToX86IR();
}

void IRToX86IR::Build(const IRBuilder* builder, X86IR::CleanUpMode mode)
{
}
UCodeLangEnd

#endif