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
	String R;
	switch (Type._Type)
	{
	case ReflectionTypes::Void:
		R += "v";
		break;
	case ReflectionTypes::Bool:
		R += "b";
		break;
	case ReflectionTypes::Char:
		R += "c";
		break;

	case ReflectionTypes::uInt8:
		R += "u8";
		break;
	case ReflectionTypes::uInt16:
		R += "u16";
		break;
	case ReflectionTypes::uInt32:
		R += "u32";
		break;
	case ReflectionTypes::uInt64:
		R += "u64";
		break;

	case ReflectionTypes::sInt8:
		R += "i8";
		break;
	case ReflectionTypes::sInt16:
		R += "i16";
		break;
	case ReflectionTypes::sInt32:
		R += "i32";
		break;
	case ReflectionTypes::sInt64:
		R += "i64";
		break;

	case ReflectionTypes::sIntPtr:
		R += "iptr";
		break;
	case ReflectionTypes::uIntPtr:
		R += "uptr";
		break;
	case ReflectionTypes::float32:
		R += "f32";
		break;
	case ReflectionTypes::float64:
		R += "f64";
		break;
	default:
		R += "[]";
		break;
	}
	if (Type._IsAddress)
	{
		R += "&";
	}
	if (Type._IsAddressArray)
	{
		R += "[&]";
	}
	if (Type._Isimmutable)
	{
		R += "umut";
	}
	if (Type._IsDynamic)
	{
		R += "dyn";
	}
	if (Type._MoveData == ReflectionMoveData::Moved)
	{
		R += "mov";
	}

	

	return R;
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