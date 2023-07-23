#pragma once
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"

namespace CppLinkTest {

	

	UCodeLangExportSymbol("UCodeLang") struct ItemClass
	{
		UCodeLangExport void ThisFunc(){}
		UCodeLangExport void ThisFunc(int H) {}
		UCodeLangExport void ThisFunc(bool H) {}

		UCodeLangExport static void StaticFunc(){}

	};
}