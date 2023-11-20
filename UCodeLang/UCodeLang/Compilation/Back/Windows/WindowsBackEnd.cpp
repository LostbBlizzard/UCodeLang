
#ifndef UCodeLangNoCompiler
#include "WindowsBackEnd.hpp"
#include "../x86_64/IRToX86_64IR.hpp"
#include "PE_File.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
UCodeLangStart
void WindowsBackEnd::Reset()
{
	this->~WindowsBackEnd();
	new (this)  WindowsBackEnd;
}

void WindowsBackEnd::Build(const IRBuilder* Input)
{
	
}
void WindowsBackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::Windows;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::X86;
}
UCodeLangEnd

#endif