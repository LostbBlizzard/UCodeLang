#include "LinuxBackEnd.hpp"
#include "../x86_64/IRToX86_64IR.hpp"
#include <sstream>

#include "UCodeLang/Compliation/CompliationSettings.hpp"
UCodeLangStart
void LinuxBackEnd::Reset()
{
	this->~LinuxBackEnd();
	new (this)  LinuxBackEnd;
}

void LinuxBackEnd::Build(const IRBuilder* Input)
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
void LinuxBackEnd::UpdateBackInfo(CompliationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompliationBackEndInfo::BackEnd::Linux;
	BackInfo.OutputSet = CompliationBackEndInfo::InsSet::X86;
}
UCodeLangEnd