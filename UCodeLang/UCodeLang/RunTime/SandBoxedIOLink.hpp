#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "RunTimeLib.hpp"
UCodeLangStart

struct SandBoxedIOLink
{
	struct LinkSettings
	{
		bool AddCInAndOut = true;
	};
	static void Link(RunTimeLib& lib,const LinkSettings& settings ={});
};

UCodeLangEnd