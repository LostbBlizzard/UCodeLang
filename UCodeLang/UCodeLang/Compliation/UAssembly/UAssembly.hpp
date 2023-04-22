#pragma once
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/CompliationSettings.hpp"
#include "UCodeLang/Compliation/Helpers/CompliationErrors.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UAssembly_NameSpace.hpp"

#include "UAssembly_Lexer.hpp"
#include "UAssembly_Parser.hpp"
UAssemblyStart
class UAssembly
{
public:
	UAssembly(){}
	~UAssembly(){}
	void Assemble(const String_view& Text, UClib* Out);
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V){_ErrorsOutput = V;}
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) {_Settings = V;}


	static String ToString(const UCodeLang::UClib* Lib);
	static String ToString(const ReflectionTypeInfo& Value);
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
		default:return "[bad]";
		}
	}
	static String GetRegisterToString(UInt64 reg) { return GetRegisterToString((RegisterID)reg); }
	static String GetInsName(const UClib* Lib, UAddress Pos)
	{
		return "[&" + std::to_string(Pos) + "]";
	}
	static void OpValueToString(OpCodeType OpType,const AnyInt64& In,const BinaryVectorMap<UAddress, String>& AddressToName,String& out, const UCodeLang::UClib* Lib);
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
};
UAssemblyEnd

