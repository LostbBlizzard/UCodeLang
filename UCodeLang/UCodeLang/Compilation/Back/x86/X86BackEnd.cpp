
#ifndef UCodeLangNoCompiler
#include "X86BackEnd.hpp"
#include "../../Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
UCodeLangStart

void X86BackEnd::Reset()
{
	/*
	_CodeGen.Reset();
	*/

}

void X86BackEnd::Build(const IRBuilder* Input)
{
	/*
		_CodeGen.Build(Input);
		Vector<Byte>& Output = _CodeGen._Output._Output.ByteOutput;

		Set_Output(BytesView(Output.data(), Output.size()));//output bytes

	*/
}

void X86BackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::Other;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::X86;
}

void X86BackEnd_UCodeLib::Reset()
{
	/*
	_CodeGen.Reset();
	*/
}

void X86BackEnd_UCodeLib::Build(const IRBuilder* Input)
{
	/*
	_CodeGen.Build(Input);
	Vector<Byte>& Output = _CodeGen._Output._Output.ByteOutput;

	UClib& Lib = Getliboutput();

	auto _OutLayer = Getliboutput().AddLayer(UCode_CodeLayer_X86_UCodeVM_Name);
	CodeLayer::MachineCode V;
	V._Code = Output;
	_OutLayer->_Data = std::move(V);
	*/

}

//CodeLayer* _OutLayer = nullptr;

void X86BackEnd_UCodeLib::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::UCodeVm;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::X86;
}
UCodeLangEnd

#endif