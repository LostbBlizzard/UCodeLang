#include "IRBackEnd.hpp"
UCodeLangStart
void IRBackEnd::Reset()
{
	this->~IRBackEnd();
	new (this) IRBackEnd;
}
void IRBackEnd::Build(const IRBuilder* Input)
{
	auto Data = Input->ToBytes();
	Set_Output(Data.AsView());
}
UCodeLangEnd