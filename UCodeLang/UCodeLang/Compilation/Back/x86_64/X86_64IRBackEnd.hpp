#pragma once
#include "../BackEndInterface.hpp"

UCodeLangStart

//Just Outputs a string
class X86_64IRBackEnd : BackEndObject
{
public:
	X86_64IRBackEnd(){}
	~X86_64IRBackEnd(){}

	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return " X86_64IR"; }
	String GetOutputExtWithDot() override { return ".txt"; }
	static BackEndObject* MakeObject() { return new X86_64IRBackEnd(); }
	void  UpdateBackInfo(CompilationBackEndInfo& BackInfo)  override {}

};

UCodeLangEnd
