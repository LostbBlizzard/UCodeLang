#ifndef UCodeLangNoCompiler
#include "MacOSBackEnd.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
UCodeLangStart
void MacOSBackEnd::Reset()
{
	this->~MacOSBackEnd();
	new (this)  MacOSBackEnd;
}

void MacOSBackEnd::Build(const IRBuilder* Input)
{
	/*
	auto x8664_ir = IRToX86_64IR::Into(*Input);
	auto code = x8664_ir.Build();

	ELFIO::elfio file;

	std::stringstream s;
	file.save(s);
	auto f = s.str();
	Set_Output(BytesView::Make((const Byte*)f.data(),f.size()));
	*/
}
void MacOSBackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::MacOs;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::X86;
}
UCodeLangEnd

#endif