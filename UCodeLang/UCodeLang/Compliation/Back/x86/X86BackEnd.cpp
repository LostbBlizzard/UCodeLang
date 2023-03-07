#include "X86BackEnd.hpp"
#include "../../Helpers/InstructionBuilder.hpp"
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

	Lib.Get_Code() = Output;

	Instruction Ins;
	for (const auto& Item : _CodeGen._Funcs)
	{
		const String FuncName =Input->FromID(Item.Func->identifier);

		InstructionBuilder::CallCode(Item.location, Ins);
		Lib.Add_NameToInstruction(Lib.Add_Instruction(Ins), FuncName);


	}

}
UCodeLangEnd

