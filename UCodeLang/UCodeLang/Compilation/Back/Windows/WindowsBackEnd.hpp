#pragma once
#include "../BackEndInterface.hpp"
UCodeLangStart

//Outputs Windows exe,dll,.lib files
class WindowsBackEnd : BackEndObject
{

public:
	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return "Windows"; }
	String GetOutputExtWithDot() override { return ".exe"; }
	static BackEndObject* MakeObject() { return new WindowsBackEnd(); }
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;
private:
};
UCodeLangEnd
