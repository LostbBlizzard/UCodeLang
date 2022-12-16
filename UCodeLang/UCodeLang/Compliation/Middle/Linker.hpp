#pragma once
#include "../../LangCore.hpp"
#include "../CompliationSettings.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "LangCore/UClib.hpp"
UCodeLangStart
class Linker
{
public:
	Linker();
	~Linker();

	void Link(const UClib* ToSelf, UClib* outLib);
	void Link(const Vector<const UClib*>& libsToLink, UClib* outLib);

	void Combine(const UCodeLang::Vector<const UCodeLang::UClib*>& libsToLink, UCodeLang::UClib* outLib);
	void Reset();

	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V)
	{
		_ErrorsOutput = V;
	}
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
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
	CompliationErrors* _ErrorsOutput =nullptr;
	CompliationSettings* _Settings = nullptr;
	unordered_map<String_view, UAddress> _Strings;
};
UCodeLangEnd

