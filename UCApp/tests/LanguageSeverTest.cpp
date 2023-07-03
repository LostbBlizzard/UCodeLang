#include "LanguageSeverTest.hpp"
#include <fstream>
#include <filesystem>
#include "UCodeAnalyzer/CppHelper.hpp"
UCodeTestStart

/// <summary>
/// Testing
/// </summary>
UCodeLangExportSymbol("UCodeLang") enum class Color : short
{
	SomeValue,
	OtherValue,
	YepValue,
};

UCodeLangExportSymbol("UCodeLang") enum Color2 : int
{
	SomeValue = 0,
	OtherValue = 1,
	YepValue = 2,
};

UCodeLangExportSymbol("UCodeLang") constexpr static size_t SomeValue3 = 0;
UCodeLangExportSymbol("UCodeLang") constexpr size_t SomeValue4 = 0;

UCodeLangExportSymbol("UCodeLang") struct SomeClass
{

	UCodeLangExport int SomeValue = 0;
	int NonExportedInt = 0;

	UCodeLangExport constexpr static size_t SomeValue2 = 0;
};

UCodeLangExportSymbol("UCodeLang") void SomeFunc(){}

UCodeLangExportSymbol("UCodeLang") using CoolInt = int;
UCodeLangExportSymbol("UCodeLang") typedef int CoolInt2;


Vector<String_view> LanguageSeverTest::IntoLines(String_view String)
{
	size_t StartLine = 0;
	Vector<String_view> R;
	for (size_t i = 0; i < String.size(); i++)
	{
		auto& Item = String[i];

		if (Item == '\n')
		{
			size_t Size = i-StartLine;

			auto str = String.substr(StartLine, Size);
			R.push_back(str);
			StartLine = i + 1;
		}
	}
	StartLine = String.size();
	R.push_back(String.substr());



	return R;
}
void LanguageSeverTest::RunTest(const Path& AsPath)
{
	std::ifstream File(AsPath);
	if (File.is_open())
	{
		std::string Text;
		std::string line;
		while (std::getline(File, line)) { Text += line + '\n'; }
		File.close();

		return RunTest(String_view(Text));
	}
	else
	{
		return RunTest(String_view(""));
	}
}
void LanguageSeverTest::RunTest(String_view FileAsString)
{
	UCodeAnalyzer::CppHelper::ParseCppfileAndOutULang(UCodeLang_UCAppDir_TestDir + "LanguageSeverTest.cpp", "test.uc");

	Vector<String_view> Lines = IntoLines(FileAsString);
	struct Info
	{
		size_t FileLine;
		size_t FileOffset;
		String_view Line;
	};

	Vector<Info> FileActions;
	Vector<String_view> FileLines;
	
	for (size_t i = 0; i < Lines.size(); i++)
	{
		auto& V = Lines[i];
		
		if (V._Starts_with("@ulangsevertester::"))
		{
			Info info;
			info.FileLine = i;
			info.FileOffset = 0;
			info.Line = V;


			FileActions.push_back(std::move(info));
		}
		else
		{
			FileLines.push_back(V);
		}

	}
	String FileText;
	for (auto& Item : FileLines)
	{
		FileText += Item;
		FileText += '\n';
	}



	
}


int RunLanguageSeverTests()
{
	Path path = UCodeLang_UCAppDir_Test_LanguageSeverFilesFiles + "Goto/BasicFind.uc";

	

	LanguageSeverTest Test;
	Test.RunTest(path);
	return 0;
}

UCodeTestEnd
