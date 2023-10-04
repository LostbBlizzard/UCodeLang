#pragma once


#include "../BackEndInterface.hpp"
#include "WasmFile.hpp"
UCodeLangStart
class WebAssemblyBackEnd : BackEndObject
{
public:
	using TypeSection = WasmFile::TypeSection;
	using FuncSection = WasmFile::FuncSection;
	using CodeSection = WasmFile::CodeSection;
	using ValType = WasmFile::ValType;
	using WasmType = WasmFile::WasmType;
	using FuncType = WasmFile::FuncType;

	WebAssemblyBackEnd();
	~WebAssemblyBackEnd() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;


	String GetBackEndName() override { return "WebAssembly"; }
	String GetOutputExtWithDot() override { return ".wasm"; }
	static BackEndObject* MakeObject();
	void UpdateBackInfo(CompliationBackEndInfo& BackInfo) override;
private:
	const IRBuilder* _Input = nullptr;
	WasmFile _Output;

	TypeSection* _typeSection =nullptr;
	FuncSection* _funcSection = nullptr;
	CodeSection* _codeSection = nullptr;

	WasmFile::Code* _funccode = nullptr;

	void OnFunc(const IRFunc* Func);

	
	WasmType ToType(const IRType& Type);
	
	String ToWebName(const IRidentifierID Name);

};
UCodeLangEnd
