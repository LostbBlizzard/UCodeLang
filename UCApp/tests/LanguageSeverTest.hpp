#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>
#include "TestNameSpace.hpp"

#include <UCodeLang/Compliation/Front/Lexer.hpp>
#include <UCodeLang/Compliation/Front/Parser.hpp>

#include "../src/UCodeLangProjectPaths.hpp"
UCodeTestStart



class LanguageSeverTest
{

	static Vector<String_view> IntoLines(String_view String);

public:
	void RunTest(const Path& AsPath);
	void RunTest(String_view FileAsString);
};

int RunLanguageSeverTests();
UCodeTestEnd

