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

	static Vector<String_view> Split(const String_view Base, const String_view spliter)
	{
		Vector<String_view> r;
		Split(Base, spliter, r);
		return r;
	}
	static void Split(const String_view Base, const String_view spliter, Vector<String_view>& out)
	{
		out.clear();

		size_t startindex = 0;
		size_t splitstart = 0;
		size_t splitercount = 0;
		for (size_t i = 0; i < Base.size(); i++)
		{
			char basechar = Base[i];

			if (spliter[splitercount] == basechar)
			{
				splitercount++;

				if (splitercount == spliter.size())
				{
					String_view n = Base.substr(startindex, i - splitstart);

					out.push_back(n);
					splitercount = 0;
					splitstart = i + 1;
				}
			}
			else
			{
				splitercount = 0;
			}
		}

		if (out.size())
		{
			String_view n = Base.substr(splitstart, Base.size() - splitstart);

			out.push_back(n);

		}
	}
};


UCodeLangEnd