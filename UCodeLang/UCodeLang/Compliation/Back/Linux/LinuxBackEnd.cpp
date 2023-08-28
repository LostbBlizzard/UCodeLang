#include "LinuxBackEnd.hpp"
#include "../x86_64/IRToX86_64IR.hpp"
#include "elfio/elfio.hpp"
#include <sstream>
UCodeLangStart
void LinuxBackEnd::Reset()
{
	this->~LinuxBackEnd();
	new (this)  LinuxBackEnd;
}

void LinuxBackEnd::Build(const IRBuilder* Input)
{
	auto x8664_ir = IRToX86_64IR::Into(*Input);
	auto code = x8664_ir.Build();

	ELFIO::elfio file;

	std::stringstream s;
	file.save(s);
	auto f = s.str();
	Set_Output(BytesView::Make((const Byte*)f.data(),f.size()));
}
UCodeLangEnd