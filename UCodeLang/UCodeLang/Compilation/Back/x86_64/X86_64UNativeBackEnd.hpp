#pragma once
#include "X86_64IR.hpp"
UCodeLangStart

//Makes an UCLib file that can be run on any OS useing NativeInterpreter for the x86_64 Cpu
class X86_64UNativeBackEnd : BackEndObject
{

public:
	void Reset() override;
	void Build(const IRBuilder* Input) override;

	String GetBackEndName() override { return "UCodeVM-X86_64"; }
	String GetOutputExtWithDot() override { return FileExt::LibWithDot; }
	static BackEndObject* MakeObject() { return new X86_64UNativeBackEnd(); }
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;
private:
	const IRBuilder* _Input = nullptr;
	UClib* _Output = nullptr;


	inline UClib& Get_Output()
	{
		return Getliboutput();
	}
};

UCodeLangEnd