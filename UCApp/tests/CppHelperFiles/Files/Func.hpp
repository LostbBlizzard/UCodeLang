#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"

namespace CppLinkTest 
{
	UCodeLangExportSymbol("UCodeLang") struct ItemClass
	{
		UCodeLangExport void ThisFunc(){}
		UCodeLangExport void ThisFunc(int H) {}
		UCodeLangExport int ThisFunc(bool H) { return 0; }

		UCodeLangExport static void StaticFunc(){}

	};

	//This func should have two copys
	UCodeLangExportSymbol("UCodeLang") void SomeFuncDefault(int V = 5) {}

	//should be an outfunc
	UCodeLangExportSymbol("UCodeLang") void SomeFuncPtrWithInline(UCodeLangOutPartype(int) V) {}
}