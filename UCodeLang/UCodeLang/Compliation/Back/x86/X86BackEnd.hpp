#pragma once
#include "X86Builder.hpp"
#include "IRToX86IR.hpp"
UCodeLangStart

//outputs a flat binary of x86 code
class X86BackEnd:BackEndObject
{
public:
	X86BackEnd(){}
	~X86BackEnd() override {}


	String GetBackEndName() override  { return "flat_x86"; };
	String GetOutputExtWithDot() override { return ".data"; }
	static BackEndObject* MakeObject() { return new X86BackEnd(); }
	void Reset() override;
	void Build(const IRBuilder* Input) override;

};

//uses UCodeLib
class X86BackEnd_UCodeLib :BackEndObject
{
public:
	X86BackEnd_UCodeLib() {}
	~X86BackEnd_UCodeLib() override {}


	String GetBackEndName() override { return "UCodeVM_x86"; };
	String GetOutputExtWithDot() override { return FileExt::LibWithDot; }
	static BackEndObject* MakeObject() { return new X86BackEnd_UCodeLib(); }
	void Reset() override;
	void Build(const IRBuilder* Input) override;
	CodeLayer* _OutLayer = nullptr;
};

UCodeLangEnd
