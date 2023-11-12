#pragma once
#include "X86_64IR.hpp"
UCodeLangStart

//Makes an UCLib file that can be run on any OS using NativeInterpreter for the x86_64 Cpu
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
	X86_64IR x8664_ir;

	inline UClib& Get_Output()
	{
		return Getliboutput();
	}


	struct NewID
	{
		uintptr_t oldid = 0;
		uintptr_t newid = 0;
	};
	UnorderedMap<uintptr_t, String> newidnames;
	Vector< NewID> newids;

	void MakeWapperFuncion(const IRFunc* func);


	struct ABIInfo
	{
		X86_64IR::CallConventionID id;
		String OS;
	};
	Vector<ABIInfo> ABIList;
};

UCodeLangEnd