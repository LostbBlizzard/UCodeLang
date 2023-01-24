#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
UCodeLangStart
class ParseHelper
{
public:
	#define ParseHelpertoIntSet(x) \
	static bool ParseStringToInt##x(String_view string, Int##x& out) \
	{ \
		out = std::stoi((String)string); \
		return true; \
	} \
		static bool ParseStringToUInt##x(String_view string, UInt##x& out) \
	{ \
		out = std::stoi((String)string); \
		return true; \
	} \


	ParseHelpertoIntSet(8);
	ParseHelpertoIntSet(16);
	ParseHelpertoIntSet(32);
	ParseHelpertoIntSet(64);

	static bool ParseStringliteralToString(String_view string,String& out)
	{
		out = string;
		return true;
	}
};
UCodeLangEnd

