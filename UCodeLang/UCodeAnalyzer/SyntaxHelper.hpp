#pragma once

#ifndef UCodeLangNoCompiler
#include "Typedef.hpp"
UCodeAnalyzerStart
class SyntaxHelper
{
public:

	static Vector<String> GetControlFlowKeywords();
	static Vector<String> GetTypeKeywords();
	static Vector<String> GetOtherKeywords();

	static Vector<String> GetKeyWords();

	static String TmLanguageString();
};

UCodeAnalyzerEnd

#endif