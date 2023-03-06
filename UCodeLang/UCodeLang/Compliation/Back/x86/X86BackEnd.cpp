#include "X86BackEnd.hpp"
UCodeLangStart

void X86BackEnd::Reset()
{
	_CodeGen.Reset();
}

void X86BackEnd::Build(const IRBuilder* Input)
{
	_CodeGen.Build(Input);
	Vector<Byte>& Output = _CodeGen._Output._Output.ByteOutput;

	Set_Output(BytesView(Output.data(), Output.size()));//output bytes
}

void X86BackEnd_UCodeLib::Reset()
{
	_CodeGen.Reset();
}

void X86BackEnd_UCodeLib::Build(const IRBuilder* Input)
{
	_CodeGen.Build(Input);
	Vector<Byte>& Output = _CodeGen._Output._Output.ByteOutput;

	UClib& Lib = Getliboutput();
	Set_Output(BytesView(Output.data(), Output.size()));//output bytes
}
UCodeLangEnd

