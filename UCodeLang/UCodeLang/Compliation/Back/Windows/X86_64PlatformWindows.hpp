#pragma once

#include "../Windows/PE_File.hpp"
#include "../x86_64/X86_64IR.hpp"
UCodeLangStart

//Outputs Windows exe,dll,.lib files
class X86_64PlatformWindows : BackEndObject
{

public:
	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return "Windows-X86_64"; }
	String GetOutputExtWithDot() override { return ".exe"; }
	static BackEndObject* MakeObject() { return new X86_64PlatformWindows(); }
private:
};
UCodeLangEnd
