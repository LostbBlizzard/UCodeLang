#pragma once

#ifndef UCodeLangNoCompiler
#include "../BackEndInterface.hpp"
UCodeLangStart
class MacOSBackEnd : BackEndObject
{
public:
	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return "MacOS"; }
	String GetOutputExtWithDot() override { return ".app"; }
	static BackEndObject* MakeObject() { return new MacOSBackEnd(); }
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;
private:
};
UCodeLangEnd

#endif