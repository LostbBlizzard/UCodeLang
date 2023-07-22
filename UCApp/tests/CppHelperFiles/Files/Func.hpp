#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"

namespace CppLinkTest {
	//This func should have two copys
	UCodeLangExportSymbol("UCodeLang") void SomeFuncDefault(int V = 5) {}

	//should be an outfunc
	UCodeLangExportSymbol("UCodeLang") void SomeFuncPtr(UCodeLangOutPartype(int) V) {}

	//Overload
	UCodeLangExportSymbol("UCodeLang") void Overload(int V) {}
	//Overload
	UCodeLangExportSymbol("UCodeLang") void Overload(bool V) {}
	//Overload
	UCodeLangExportSymbol("UCodeLang") void Overload(bool V,int V2) {}

}