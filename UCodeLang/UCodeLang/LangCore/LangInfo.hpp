#pragma once
#include "UCodeLangNameSpace.hpp"
UCodeLangStart
class LangInfo
{
public:
	enum class Vesion
	{
		Null,
		_0_0_1,
	};
	static constexpr Vesion CurrrentVersion = Vesion::_0_0_1;
	static constexpr const char* VersionName = "0.0.1";
};

UCodeLangEnd