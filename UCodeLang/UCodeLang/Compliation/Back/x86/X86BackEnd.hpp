#pragma once
#include "X86Gen.hpp"
UCodeLangStart

//outputs a flat binary of x86 code
class X86BackEnd:BackEndObject
{
	X86BackEnd(){}
	~X86BackEnd() override {}



	static BackEndObject* MakeObject() { return new X86BackEnd(); }
	void Reset() override
	{
		_CodeGen.Reset();
	}
	void Build(const IRBuilder* Input) override
	{
		_CodeGen.Build(Input);
		Vector<Byte>& Output = _CodeGen._Output.ByteOutput;

		Set_Output(BytesView(Output.data(), Output.size()));//output bytes
	}
	X86Gen _CodeGen;
};

//uses UCodeLib
class X86BackEnd_UCodeLib :BackEndObject
{
	X86BackEnd_UCodeLib() {}
	~X86BackEnd_UCodeLib() override {}


	static BackEndObject* MakeObject() { return new X86BackEnd_UCodeLib(); }
	void Reset() override
	{
		_CodeGen.Reset();
	}
	void Build(const IRBuilder* Input) override
	{
		_CodeGen.Build(Input);
		Vector<Byte>& Output = _CodeGen._Output.ByteOutput;
		
		UClib& Lib = Getliboutput();
		Set_Output(BytesView(Output.data(), Output.size()));//output bytes
	}
	X86Gen _CodeGen;
};

UCodeLangEnd
