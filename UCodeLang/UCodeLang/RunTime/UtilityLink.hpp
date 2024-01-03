#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "RunTimeLib.hpp"
UCodeLangStart


//Math,SIMD,Atomic,Hardware and every thing that wouldn't fit in [IO/SandBoxed]Link
struct UtilityLink
{
	struct LinkSettings
	{
		bool AddMath = true;
		bool AddSIMD = true;
		bool AddThreading = true;
	};


	static void Link(RunTimeLib& lib, const LinkSettings& settings = {});
};
UCodeLangEnd