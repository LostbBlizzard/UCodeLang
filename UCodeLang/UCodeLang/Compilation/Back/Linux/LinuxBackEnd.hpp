#pragma once
#ifndef UCodeLangNoCompiler
#include "../BackEndInterface.hpp"
UCodeLangStart
class LinuxBackEnd : BackEndObject
{

public:
	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return "Linux"; }
	String GetOutputExtWithDot() override { return ""; }
	static BackEndObject* MakeObject() { return new LinuxBackEnd(); }
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;
private:
};

UCodeLangEnd

#endif