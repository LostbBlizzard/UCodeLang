#pragma once
#include "../LangCore.hpp"
#include "../Compliation/Helpers/CompliationErrors.hpp"
#include "../Compliation/UClib.hpp"
#include "../Compliation/CompliationSettings.hpp"
UCodeLangStart
class UAssembly
{
public:
	UAssembly() :_ErrorsOutput(nullptr),_Settings(nullptr)
	{

	}
	void Build(const String& Text, UCodeLang::UClib* Out);
	inline void Set_ErrorsOutput(CompliationErrors* V)
	{
		_ErrorsOutput = V;
	}
	inline void Set_Settings(CompliationSettings* V) {
		_Settings = V;
	}


	static String ToString(const UCodeLang::UClib* Lib);
	static String GetRegisterToString(RegisterID reg)
	{
		switch (reg)
		{
		case UCodeLang::RegisterID::A:return "[A]";
		case UCodeLang::RegisterID::B:return "[B]";
		case UCodeLang::RegisterID::C:return "[C]";
		case UCodeLang::RegisterID::D:return "[D]";
		case UCodeLang::RegisterID::E:return "[E]";
		case UCodeLang::RegisterID::F:return "[F]";
		case UCodeLang::RegisterID::NullRegister:return "[null]";
		default:return "[bad]";
		}
	}
	static String GetRegisterToString(UInt64 reg) { return GetRegisterToString((RegisterID)reg); }
	static String GetInsName(const UClib* Lib, UAddress Pos)
	{
		return "[&" + std::to_string(Pos) + "]";
	}
private:
	CompliationErrors* _ErrorsOutput;
	CompliationSettings* _Settings;
};
UCodeLangEnd

