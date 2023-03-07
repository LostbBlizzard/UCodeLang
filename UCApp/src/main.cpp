#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>
#include "Tests/Test.hpp"
#include <future>
#include <optional>
#include "UCodeAnalyzer/CodeBuilds/StandardLibrary.hpp"
#include <sstream>
#include "UCodeLang/Compliation/Back/C98/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/x86/X86BackEnd.hpp"

using namespace UCodeLang;
const UCodeLang::String ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
const UCodeLang::String TopTestDir = ScrDir + "Tests/";
const UCodeLang::String TopTestUnitDir = TopTestDir + "Unit/";

const UCodeLang::String TopDir = ScrDir + "CodeTesting/";
const UCodeLang::String FileDir = TopDir + "Files/";
const UCodeLang::String DLLDir = TopDir + "Dlls/";

const UCodeLang::String OutPath = TopDir + "Output/";
const UCodeLang::String IntPath = OutPath + "int/";


#define ULibName(x) OutPath + x + FileExt::LibWithDot;
#define ULibNamedll(x) OutPath +  x + FileExt::DllWithDot;

const UCodeLang::String OutLibPath = ULibName("LibTest");
const UCodeLang::String OutDLLPath = ULibNamedll("DLLTest");
const UCodeLang::String OutFilePath = ULibName("OutFile");


const UCodeLang::String StandardLibraryPath = ScrDir + "StandardLibrary/";

const UCodeLang::String StandardLibraryOut = ScrDir + "StandardLibrary_Out/";
const UCodeLang::String StandardLibraryinit = StandardLibraryOut + "init/";

#define StandardLibrarynamespace "ULang"


void Test(int A)
{
	std::cout << "DLLCall Got Value " << A << std::endl;
} 
static CPPCallRet UCodeLangAPI Invoke_Test(int A)
{
	Test(A);
	return 0;
}

static UCodeRunTime RunTime;
template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}
int main()
{


	/*
	{
		namespace fs = std::filesystem;
		std::stringstream Text;
		UCodeAnalyzer::StandardLibraryBuilder::PackageUCodeTextFiles(Text, StandardLibraryPath);

		String Str = Text.str();
		
		{
			String outpath = FileDir + "StandardLibrary.uc";
			std::ofstream out(outpath);
			out << Str;
			out.close();
		}

		{
			String outpath = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCodeLang/"
				"UCodeAnalyzer/CodeBuilds/StandardLibrary.cpp";
			
			String Str2 = "#include \"StandardLibrary.hpp\"\ \n" "UCodeAnalyzerStart \n";

			Str2 += "StringView StandardLibraryBuilder::UCodeStandardLibrary = \" ";
			
			for (size_t i = 0; i < Str.size(); i++)
			{
				char V = Str[i];
				if (V == '\n')
				{
					Str2 +='\\';
				}
				Str2 += V;
			}
			Str2 += "\" ; \n UCodeAnalyzerEnd";

			std::ofstream out(outpath);
			out << Str2;
			out.close();
		}
	}
	*/

	//ULangTest::RunTests();
	
	
	UCodeLang::Compiler _Compiler;
	UCodeLang::CompliationSettings& Settings =_Compiler.Get_Settings();
	UCodeLang::Compiler::CompilerPathData Data;
	//Settings.PtrSize = IntSizes::Int8;

	//Main
	Settings._Type = OutPutType::Lib;
	Data.FileDir = FileDir;
	Data.OutFile = OutFilePath;
	Data.IntDir = IntPath;
	Settings._Flags = OptimizationFlags::Debug;

	//_Compiler.Set_BackEnd(ULangTest::X86BackEnd_UCodeLib::MakeObject);
	_Compiler.CompileFiles_UseIntDir(Data);

	
	


	/*auto Bits = Compiler::GetBytesFromFile(Data.OutFile);
	{
		String Tep;
		{
			char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
			for (size_t i = 0; i < Bits.Size; i++)
			{
				Byte byte = Bits.Bytes[i];
				Tep += hex_chars[(byte & 0xF0) >> 4];
				Tep += hex_chars[(byte & 0x0F) >> 0];
			}
		}


		AsmBuffer V = AsmBuffer(Bits.Bytes.get(), Bits.Size);
		V.SetToExecuteMode();
		using FuncP = int(*)();

		FuncP CallFunc = (FuncP)V.Data;
		int tep = CallFunc();
	}*/

	if (!ULangTest::LogErrors(std::cout,_Compiler))
	{
		UCodeLang::UClib MLib;
		if (UClib::FromFile(&MLib, Data.OutFile))
		{
			auto Text = UAssembly::UAssembly::ToString(&MLib);
			String Path = Data.OutFile + ".UA";
			std::ofstream out(Path);
			out << Text;
			out.close();
		}

		
		UCodeLang::RunTimeLib Lib;
		Lib.Init(&MLib);

		UCodeLang::RunTimeLib DLLib;
		
		Lib.Add_CPPCall("DLLCall", Invoke_Test);
		
		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		
		RunTime.Init(&State);

		
		
		int item = 0;

		auto r = RunTime.RCall <UAddress> ("main", item);
 		
		
	   std::cout << " Got Value " << (int)r << std::endl;

	}
}
