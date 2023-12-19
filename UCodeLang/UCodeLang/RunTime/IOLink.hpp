#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "RunTimeLib.hpp"
UCodeLangStart

struct IOLink
{
	struct LinkSettings
	{
		bool AddCInAndOut = true;
		bool AddFileIO = true;
		bool AddNetworking = true;
		LinkSettings()
		{
			
		}
	};
	static void Link(RunTimeLib& lib, const LinkSettings& settings = {});
};

UCodeLangEnd