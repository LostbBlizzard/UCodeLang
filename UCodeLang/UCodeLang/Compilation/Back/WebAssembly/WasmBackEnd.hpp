#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "../BackEndInterface.hpp"
#include "WasmFile.hpp"
UCodeLangStart


//Uses the Wasm C ABi https://github.com/WebAssembly/tool-conventions/blob/main/BasicCABI.md
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

	TypeSection* _typeSection = nullptr;
	FuncSection* _funcSection = nullptr;
	ExportSection* _exportSection = nullptr;
	CodeSection* _codeSection = nullptr;
	MemSection* _memSection = nullptr;


	WasmFile::Code* _funccode = nullptr;
	const IRFunc* _func;

	void OnFunc(const IRFunc* Func);

	struct InsToUpdateMap
	{
		size_t FuncIndex;
		size_t InsIndex;

		IRidentifierID lookingforfunc;
	};
	Vector< InsToUpdateMap> InsToUpdate;
	UnorderedMap< IRidentifierID, size_t> FuncToIndex;

	WasmType ToType(const IRType& Type);

	String ToWebName(const IRidentifierID Name);

	void LoadOp(const IRInstruction* ir, const IROperator& Op);

	struct SaveInfo
	{
		const IRInstruction* ir;
	};
	SaveInfo StartSave(const IRInstruction* ir);

	void EndSave(SaveInfo& info);

	UnorderedMap<const IRInstruction*, size_t> Position;

	//Compile Flags
	bool Flag_NoExceptions = false;

};
UCodeLangEnd

#endif