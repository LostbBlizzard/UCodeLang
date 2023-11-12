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

	static bool Contains(const String& string, const String_view MatchString)
	{
		return  Contains(String_view(string), String_view(MatchString));
	}
	static bool Contains(const String_view string, const String_view MatchString)
	{
		if (string.find(MatchString) != std::string::npos) {
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool EndWith(const String& string, const String_view MatchString)
	{
		return  EndWith(String_view(string), String_view(MatchString));
	}
	static bool EndWith(const String_view string, const String_view MatchString)
	{
		if (string.size() < MatchString.size()) { return false; }
		for (int i = MatchString.size() - 1; i >= 0; i--)
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