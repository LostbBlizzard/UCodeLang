#pragma once

#include "UCodeLang/UCodeLang.hpp"
#include "../Typedef.hpp"

#include <filesystem>
#include <iostream>
#include "UCodeLang/LangCore/FileHelper.hpp"
#define StandardLibrarynamespace "ULang"
UCodeAnalyzerStart
class StandardLibraryBuilder
{
public:
	

	static void PackageUCodeTextFiles(std::ostream& output,const Path& path)
	{
		namespace fs = std::filesystem;
		for (const auto& dirEntry : fs::recursive_directory_iterator(path))
		{

			if (!dirEntry.is_regular_file()) { continue; }

			auto Ext = dirEntry.path().extension();	
			if (Ext == UCodeLang::FileExt::SourceFileWithDot)
			{

				Path RePath = UCodeLang::FileHelper::RelativePath(dirEntry.path(), path);

				output << "\n// \n";
				output << "// Packaged FilePath:" << RePath.generic_string() << "\n";
				output << "// \n\n";
				output << UCodeLang::Compiler::GetTextFromFile(dirEntry.path());
			}
		}
	}

	static StringView Get_UCodeStandardLibraryText()
	{
		return  UCodeStandardLibrary;
	}
private:
	static StringView UCodeStandardLibrary;
};

UCodeAnalyzerEnd