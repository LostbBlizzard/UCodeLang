#pragma once

#include "../UCodeLang/UCodeLang/LangCore/ReflectionData.hpp"
UCodeLangStart
class NameDecoratior
{
public:
	struct Par
	{
		ClassMethod::Par par;
		String symbolFullName;
	};
	static String GetDecoratedName(const String_view funcName, const Vector<Par>& Pars);
	static String DecoratedNameType(const Par& Type);
	static String GetUnDecoratedName(const String& DecoratedName);
};
UCodeLangEnd

