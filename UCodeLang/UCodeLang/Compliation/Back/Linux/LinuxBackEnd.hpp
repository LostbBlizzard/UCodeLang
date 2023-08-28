#pragma once
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
private:
};

UCodeLangEnd