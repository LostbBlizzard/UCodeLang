#include "NameDecoratior.hpp"

//#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
//#include "../../LangCore/LangTypes.hpp"
//#include "../../LangCore/TypeNames.hpp"
//#include "CompilerTypes.hpp"

UCodeLangStart
String NameDecoratior::GetDecoratedName(const String_view funcName, const Vector<ReflectionTypeInfo>& Pars)
{
	String r = (String)funcName;

	if (Pars.size())
	{
		r += "^";
		for (auto& Item : Pars)
		{
			r += DecoratedNameType(Item);
			
			if (&Item != &Pars.back())
			{
				r += ",";
			}
		}
	}

	return r;
}
String NameDecoratior::DecoratedNameType(const ReflectionTypeInfo& Type)
{
	/*
	if (Type.FullNameType == CharTypeName){return "c";}
	if (Type.FullNameType == boolTypeName) { return "b"; }

	if (Type.FullNameType == Uint8TypeName) { return "1"; }
	if (Type.FullNameType == Sint8TypeName) { return "s1"; }

	if (Type.FullNameType == Uint16TypeName) { return "2"; }
	if (Type.FullNameType == Sint16TypeName) { return "s2"; }

	if (Type.FullNameType == Uint32TypeName) { return "4"; }
	if (Type.FullNameType == Sint32TypeName) { return "s4"; }

	if (Type.FullNameType == Uint64TypeName) { return "8"; }
	if (Type.FullNameType == Sint64TypeName) { return "s8"; }

	if (Type.FullNameType == float32TypeName) { return "f4"; }
	if (Type.FullNameType == float64TypeName) { return "f8"; }
	*/

	return Type.FullNameType;
}
String NameDecoratior::GetUnDecoratedName(const String& DecoratedName)
{
	for (size_t i = 0; i < DecoratedName.size(); i++)
	{
		if (DecoratedName[i] == '^')
		{
			return DecoratedName.substr(0, i);
		}
	}
	return DecoratedName;
}
UCodeLangEnd