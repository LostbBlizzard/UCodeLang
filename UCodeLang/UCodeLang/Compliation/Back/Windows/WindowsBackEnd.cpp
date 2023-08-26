#include "WindowsBackEnd.hpp"
UCodeLangStart
void WindowsBackEnd::Reset()
{
	this->~WindowsBackEnd();
	new (this)  WindowsBackEnd;
}

void WindowsBackEnd::Build(const IRBuilder* Input)
{

	PE_File Pefile;
	Set_Output(Pefile.ToRawBytes(&Pefile).AsSpan());
}
UCodeLangEnd