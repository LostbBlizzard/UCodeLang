#pragma once


#include "../BackEndInterface.hpp"
UCodeLangStart
class WebAssemblyBackEnd : BackEndObject
{
public:
	WebAssemblyBackEnd();
	~WebAssemblyBackEnd() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;


	String GetBackEndName() override { return "WebAssembly"; }
	String GetOutputExtWithDot() override { return ".wast"; }
	static BackEndObject* MakeObject();
private:
	const IRBuilder* _Input = nullptr;
	String _Output;


	void OnFunc(const IRFunc* Func);
	void OnFuncSyb(const IRFunc* Func);
	
	String ToWebName(const IRidentifierID Name);


	String ToString(const IRType& Type);
};
UCodeLangEnd
