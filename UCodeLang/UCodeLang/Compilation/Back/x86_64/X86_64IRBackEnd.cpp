#ifndef UCodeLangNoCompiler

#include "X86_64IRBackEnd.hpp"
#include "IRToX86_64IR.hpp"
UCodeLangStart
void X86_64IRBackEnd::Reset()
{
	this->~X86_64IRBackEnd();
	new (this) X86_64IRBackEnd;
}

void X86_64IRBackEnd::Build(const IRBuilder* Input)
{
	String v = IRToX86_64IR::Into(*Input).ToString();
	Set_Output(v);
}
UCodeLangEnd

#endif