#include "WindowsBackEnd.hpp"
#include "../x86_64/IRToX86_64IR.hpp"
#include "PE_File.hpp"
#include "UCodeLang/Compliation/CompliationSettings.hpp"
UCodeLangStart
void WindowsBackEnd::Reset()
{
	this->~WindowsBackEnd();
	new (this)  WindowsBackEnd;
}

void WindowsBackEnd::Build(const IRBuilder* Input)
{
	
}
void WindowsBackEnd::UpdateBackInfo(CompliationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompliationBackEndInfo::BackEnd::Windows;
	BackInfo.OutputSet = CompliationBackEndInfo::InsSet::X86;
}
UCodeLangEnd