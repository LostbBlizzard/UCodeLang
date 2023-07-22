#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"



namespace CppLinkTest
{
	//This func should have two copys
	UCodeLangExportSymbol("UCodeLang") void SpanTest(UCodeLang::BytesView Item) {}

	//should be an outfunc
	UCodeLangExportSymbol("UCodeLang") void SomeFuncPtrWithInline(UCodeLangOutPartype(int) V) {}

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
	UCodeLangExportSymbol("UCodeLang") constexpr bool SomeValue4 = true;

	UCodeLangExportSymbol("UCodeLang") struct SomeClass
	{
		UCodeLangExport int SomeValue = 0;
		UCodeLangExclude int NonExportedInt = 0;

		UCodeLangExport constexpr static size_t SomeValue2 = 0;

		//this should be a member funcion
		UCodeLangExport void SomeMemberFunc(char V) {}

		//this should be a static funcion
		UCodeLangExport void SomeMemberStatic(char& V) {}


		//should make new
		UCodeLangExport SomeClass()
		{

		}

		//should make drop
		UCodeLangExport ~SomeClass()
		{

		}


		//Overload
		UCodeLangExportSymbol("UCodeLang") void Overload(int V) {}
		//Overload
		UCodeLangExportSymbol("UCodeLang") void Overload(bool V) {}
		//Overload
		UCodeLangExportSymbol("UCodeLang") void Overload(bool V, int V2) {}

	};

	UCodeLangExportSymbol("UCodeLang") void SomeFunc() {}

	UCodeLangExportSymbol("UCodeLang") using CoolInt = int;
	UCodeLangExportSymbol("UCodeLang") typedef int CoolInt2;

}