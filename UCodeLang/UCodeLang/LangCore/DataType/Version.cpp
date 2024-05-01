#include "Version.hpp"
#include "UCodeLang/LangCore/StringHelper.hpp"
UCodeLangStart

Optional<Version> Version::Parse(const String_view str)
{

	auto list = StringHelper::Split(str,".");

	if (list.size() == 3)
	{
		auto majora = String(list[0]);
		auto minora = String(list[1]);
		auto patcha = String(list[2]);

		return Version(std::stoi(majora), std::stoi(minora), std::stoi(patcha));
	}

	return Optional<Version>();
}

UCodeLangEnd