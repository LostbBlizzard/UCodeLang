#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
UCodeLangStart

class IRBuilder;
class CompilationErrors;
struct IRFunc;

class IRUBChecker
{
public:
	IRUBChecker()
	{

	}
	~IRUBChecker()
	{

	}
	void Reset();
	void CheckForUB(const IRBuilder* Input);

	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _Errs = V; }
private:
	const IRBuilder* _Input = nullptr;
	CompilationErrors* _Errs = nullptr;


	struct PointerMemberInfo
	{
		Vector<size_t> Field;
	};

	struct ParInfo
	{

		bool PointerRead = false;
		bool PointerWrite = false;
		PointerMemberInfo PointerMemberRead;
		PointerMemberInfo PointerMemberWrite;

	};
	struct FuncInfo
	{
		Vector<ParInfo> parInfo;
	};
	UnorderedMap<const IRFunc*, FuncInfo> FuncInfos;
};

UCodeLangEnd

#endif