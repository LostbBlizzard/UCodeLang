#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "../BackEndInterface.hpp"
#include "WasmFile.hpp"
UCodeLangStart
class WasmBackEnd : BackEndObject
{
public:
	using TypeSection = WasmFile::TypeSection;
	using FuncSection = WasmFile::FuncSection;
	using CodeSection = WasmFile::CodeSection;
	using ExportSection = WasmFile::ExportSection;
	using MemSection = WasmFile::MemSection;
	using ValType = WasmFile::ValType;
	using WasmType = WasmFile::WasmType;
	using FuncType = WasmFile::FuncType;
	using Export = WasmFile::Export;

	WasmBackEnd();
	~WasmBackEnd() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;


	String GetBackEndName() override { return "WebAssembly"; }
	String GetOutputExtWithDot() override { return ".wasm"; }
	static BackEndObject* MakeObject();
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;

	static String ToWebName(const String& IRName);
private:
	const IRBuilder* _Input = nullptr;
	WasmFile _Output;

	TypeSection* _typeSection =nullptr;
	FuncSection* _funcSection = nullptr;
	ExportSection* _exportSection = nullptr;
	CodeSection* _codeSection = nullptr;
	MemSection* _memSection = nullptr;


	WasmFile::Code* _funccode = nullptr;

	void OnFunc(const IRFunc* Func);

	
	WasmType ToType(const IRType& Type);
	
	String ToWebName(const IRidentifierID Name);
	
	void LoadOp(const IRInstruction* ir, const IROperator& Op);

	UnorderedMap<const IRInstruction*,size_t> Position;

};
UCodeLangEnd

#endif