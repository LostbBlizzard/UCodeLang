#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"


/// <summary>
/// Testing
/// </summary>
UCodeLangExportSymbol("UCodeLang") enum class Color : short
{
	SomeValue,
	OtherValue,
	YepValue,
};

UCodeLangExportSymbol("UCodeLang") enum Color2 : int
{
	SomeValue = 0,
	OtherValue = 1,
	YepValue = 2,

	UCodeLangExclude NoneShowedValue,
};

UCodeLangExportSymbol("UCodeLang") constexpr static size_t SomeValue3 = 0;
UCodeLangExportSymbol("UCodeLang") constexpr size_t SomeValue4 = 0;

UCodeLangExportSymbol("UCodeLang") struct SomeClass
{

	UCodeLangExport int SomeValue = 0;
	int NonExportedInt = 0;

	UCodeLangExport constexpr static size_t SomeValue2 = 0;
};

UCodeLangExportSymbol("UCodeLang") void SomeFunc() {}

UCodeLangExportSymbol("UCodeLang") using CoolInt = int;
UCodeLangExportSymbol("UCodeLang") typedef int CoolInt2;