#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"

namespace CppLinkTest {
	//This func should have two copys
	UCodeLangExportSymbol("UCodeLang") void SomeFuncDefault(int V = 5) {}

	//should be an outfunc
	UCodeLangExportSymbol("UCodeLang") void SomeFuncPtr(UCodeLangOutPartype(int) V) {}

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

		//this should be a member funcion
		UCodeLangExportSymbol("UCodeLang") void SomeMemberFunc(char V) {}

		//this should be a static funcion
		UCodeLangExportSymbol("UCodeLang") void SomeMemberStatic(char& V) {}
	};

	UCodeLangExportSymbol("UCodeLang") void SomeFunc() {}

	UCodeLangExportSymbol("UCodeLang") using CoolInt = int;
	UCodeLangExportSymbol("UCodeLang") typedef int CoolInt2;

}