#pragma once
#include "X86Gen.hpp"
UCodeLangStart

//outputs a flat binary of x86 code
class X86BackEnd:BackEndObject
{
public:
	X86BackEnd(){}
	~X86BackEnd() override {}



	static BackEndObject* MakeObject() { return new X86BackEnd(); }
	void Reset() override;
	void Build(const IRBuilder* Input) override;
	IRToX86 _CodeGen;
};

//uses UCodeLib
class X86BackEnd_UCodeLib :BackEndObject
{
public:
	X86BackEnd_UCodeLib() {}
	~X86BackEnd_UCodeLib() override {}


	static BackEndObject* MakeObject() { return new X86BackEnd_UCodeLib(); }
	void Reset() override;
	void Build(const IRBuilder* Input) override;
	IRToX86 _CodeGen;
};

UCodeLangEnd
