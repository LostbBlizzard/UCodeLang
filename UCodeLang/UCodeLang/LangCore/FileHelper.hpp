#pragma once

#include "LangTypes.hpp"
UCodeLangStart
class FileHelper
{
public:
	static String RelativePath(const String& SubPath, const String& Path);
	static void ChangeExt(String& Path, const String& NewExt);
};
UCodeLangEnd
