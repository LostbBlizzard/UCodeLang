#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
UCodeLangStart
class ParseHelper
{
public:
	static bool ParseStringToInt8(String_view string, Int8& out)
	{
		out = std::stoi((String)string);
		return true;
	}
	static bool ParseStringToUInt8(String_view string, UInt8& out)
	{
		out = std::stoi((String)string);
		return true;
	}
	static bool ParseStringliteralToString(String_view string,String& out)
	{
		out = string;
		return true;
	}
};
UCodeLangEnd

