#include "X86_64PlatformWindows.hpp"
UCodeLangStart
void X86_64PlatformWindows::Reset()
{
	this->~X86_64PlatformWindows();
	new (this)  X86_64PlatformWindows;
}

void X86_64PlatformWindows::Build(const IRBuilder* Input)
{

	PE_File Pefile;
	Set_Output(Pefile.ToRawBytes(&Pefile).AsSpan());
}
UCodeLangEnd