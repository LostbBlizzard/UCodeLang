#pragma once

#include "../UCodeLang/UCodeLang/LangCore/ReflectionData.hpp""
UCodeLangStart
class NameDecoratior
{
public:
	static String GetDecoratedName(const String_view funcName, const Vector<ReflectionTypeInfo>& Pars);
	static String DecoratedNameType(const ReflectionTypeInfo& Type);
};
UCodeLangEnd

