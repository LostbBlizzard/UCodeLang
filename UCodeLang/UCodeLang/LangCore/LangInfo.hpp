#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangTypes.hpp"
UCodeLangStart
class LangInfo
{
public:
	class Vesion
	{
	public:
		UInt64 MajorVersion = 0;
		UInt64 MinorVersion = 0;
		UInt64 RevisionVersion = 0;
	
		Vesion(UInt64 Major, UInt64 Minor, UInt64 Revision)
		{
			MajorVersion = Major;
			MinorVersion = Minor;
			RevisionVersion = Revision;
		}
	};
	inline static Vesion CurrrentVersion = Vesion(0,0,1);
	static constexpr const char* VersionName = "0.0.1";
	UCodeLangAPIExport static Path GetUCodeGlobalDirectory();
	UCodeLangAPIExport static Path GetUCodeGlobalModulesDownloads();
	UCodeLangAPIExport static Path GetUCodeGlobalBin();
	UCodeLangAPIExport static Path GetUCodeGlobalCacheDirectory();
};

UCodeLangEnd