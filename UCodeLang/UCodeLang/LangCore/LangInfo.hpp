#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangTypes.hpp"
UCodeLangStart
class LangInfo
{
public:
	class Version
	{
	public:
		UInt64 MajorVersion = 0;
		UInt64 MinorVersion = 0;
		UInt64 RevisionVersion = 0;
	
		Version(UInt64 Major, UInt64 Minor, UInt64 Revision)
		{
			MajorVersion = Major;
			MinorVersion = Minor;
			RevisionVersion = Revision;
		}
	};
	static Version CurrrentVersion;
	static const char* VersionName;
	UCodeLangAPIExport static Path GetUCodeGlobalDirectory();
	UCodeLangAPIExport static Path GetUCodeGlobalModulesDownloads();
	UCodeLangAPIExport static Path GetUCodeGlobalBin();
	UCodeLangAPIExport static Path GetUCodeGlobalCacheDirectory();
};

UCodeLangEnd