#pragma once

#ifndef UCodeLangNoCompiler
#include "../Typedef.hpp"
UCodeAnalyzerStart
class ULangDocMaker
{
public:

	struct ULangDocFileInfo
	{

	};
	struct ProjectDocInt
	{
		Vector<ULangDocFileInfo> Files;
	};


	using OutFileFormat_t = int;
	enum class OutFileFormat :OutFileFormat_t
	{
		MarkDown,
		XML,


		Default = (OutFileFormat_t)MarkDown,
	};
	struct OutputSettings
	{
		OutFileFormat OutFileType;
	};

	static bool ParseCppfileAndOutULang(const Path& SrcDir, const OutputSettings& Settings, const Path& OutDir);
};
UCodeAnalyzerEnd

#endif