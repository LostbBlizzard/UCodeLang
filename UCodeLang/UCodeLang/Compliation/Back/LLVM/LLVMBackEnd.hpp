#pragma once
#include "../BackEndInterface.hpp"


UCodeLangStart


//Just Outputs LLVM assembly string
class LLVMBackEnd : BackEndObject
{
public:
	LLVMBackEnd(){}
	~LLVMBackEnd() {}

	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return " LLVMBackEnd"; }
	String GetOutputExtWithDot() override { return ".ll"; }
	static BackEndObject* MakeObject() { return new LLVMBackEnd(); }
private:
};
UCodeLangEnd