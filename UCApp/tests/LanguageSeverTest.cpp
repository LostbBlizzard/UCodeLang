#include "LanguageSeverTest.hpp"
#include <fstream>
#include <filesystem>
#include "UCodeLang/Compliation/Back/x86_64/X86_64Gen.hpp"
#include "Zydis/Zydis.h"
#include <inttypes.h>
#include <stdio.h>
UCodeTestStart




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
	{
		ImportUseing86x64Gen
		X86_64Gen gen;
		gen.mov64(GReg::RAX, IndrReg(GReg::RAX));
		

		void* InsData = gen.GetData();
		size_t InsSize = gen._Base.Size();
		
		
		ZyanU64 runtime_address = (ZyanU64)InsData;
		//runtime_address = 0;
		// Loop over the instructions in our buffer.
		ZyanUSize offset = 0;
		ZydisDisassembledInstruction instruction;

		String Str;

		const size_t MaxInsSize = 8;
		while (ZYAN_SUCCESS(ZydisDisassembleIntel(
			ZYDIS_MACHINE_MODE_LONG_64,
			runtime_address,
			(void*)((uintptr_t)InsData + offset),
			InsSize - offset,
			&instruction
		)))
		{

			String InsStr = instruction.text;
			Str += InsStr + "\n";


			offset += instruction.info.length;
			runtime_address += instruction.info.length;
		}
		if (Str != "mov [rax], rax\n")
		{
			int a = 0;
		}
		int a = 0;
	}


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
	#if UCodeLangDebug
	Path path = UCodeLang_UCAppDir_Test_LanguageSeverFilesFiles + "Goto/BasicFind.uc";

	

	LanguageSeverTest Test;
	Test.RunTest(path);
	return 0;
	#endif
	return 0;
}

UCodeTestEnd
