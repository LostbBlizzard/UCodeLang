#pragma once
#include "../../LangCore.hpp"
#include "../CompliationSettings.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../UClib.hpp"
UCodeLangStart
class Linker
{
public:
	Linker() :_ErrorsOutput(nullptr), _Settings(nullptr)
	{

	}

	void Link(const UClib* ToSelf, UClib* outLib);
	void Link(const Vector<const UClib*>& libsToLink, UClib* outLib);

	void Combine(const UCodeLang::Vector<const UCodeLang::UClib*>& libsToLink, UCodeLang::UClib* outLib);

	inline void Set_ErrorsOutput(CompliationErrors* V)
	{
		_ErrorsOutput = V;
	}
	inline void Set_Settings(CompliationSettings* V) { _Settings = V; }
	UAddress* Add_String(const String_view& String)
	{
		if (_Strings.count(String))
		{
			return &_Strings.at(String);
		}
		else
		{
			return &(_Strings[String] = 0);
		}
	}

private:
	CompliationErrors* _ErrorsOutput;
	CompliationSettings* _Settings;
	unordered_map<String_view, UAddress> _Strings;
};
UCodeLangEnd

