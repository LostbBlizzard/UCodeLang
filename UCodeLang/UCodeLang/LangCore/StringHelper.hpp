#pragma once
#include "LangTypes.hpp"
UCodeLangStart


class StringHelper
{
public:
	static bool StartWith(const String& string, const String_view MatchString)
	{
		return  StartWith(String_view(string), String_view(MatchString));
	}
	static bool StartWith(const String_view string, const String_view MatchString)
	{
		if (string.size() < MatchString.size()) { return false; }
		for (size_t i = 0; i < MatchString.size(); i++)
		{
			if (string[i] != MatchString[i])
			{
				return false;
				break;
			}
		}
		return true;
	}
};


UCodeLangEnd