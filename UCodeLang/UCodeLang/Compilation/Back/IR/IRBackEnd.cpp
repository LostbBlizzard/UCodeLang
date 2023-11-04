#include "IRBackEnd.hpp"
#include "../../Middle/IR.hpp"
UCodeLangStart
IRBackEnd::IRBackEnd()
{
}
IRBackEnd::~IRBackEnd()
{
}
void IRBackEnd::Reset()
{
	this->~IRBackEnd();
	new (this) IRBackEnd;
}
void IRBackEnd::Build(const IRBuilder* Input)
{
	auto Data = Input->ToBytes();
	Set_Output(Data.AsSpan());
}
BackEndObject* IRBackEnd::MakeObject() { return new IRBackEnd(); }
UCodeLangEnd