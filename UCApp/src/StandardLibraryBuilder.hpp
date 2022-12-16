#pragma once

#include "UCodeLang/UCodeLang.hpp"

#define StandardLibrarynamespace "ULang"

class StandardLibraryBuilder
{
public:
	static void Get_VecFile(UCodeLang::String& Out);
	static void Get_LibFile(UCodeLang::String& Out);
	static void Get_LowLevelFile(UCodeLang::String& Out);
	static void Get_ConsoleFile(UCodeLang::String& Out);
	static void Get_MathFile(UCodeLang::String& Out);
	static void Get_GenericContainersFile(UCodeLang::String& Out);
	static void Get_MemoryFile(UCodeLang::String& Out);

	static void BuildLibraryToDir(const UCodeLang::String& Dir);
	static void WirteToFile(const UCodeLang::String& Path, const UCodeLang::String& String);
};

