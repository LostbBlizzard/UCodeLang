#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>

#include <future>
#include <optional>
#include "UCodeAnalyzer/Formater.hpp"
#include <sstream>
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/x86/X86BackEnd.hpp"
#include "UCodeLang/Compliation/ModuleFile.hpp"
#include "UCodeLang/RunTime/ProfilerDebuger.hpp"

#include "../tests/TestGenerator.hpp"
#include "../tests/Test.hpp"
#include "../tests/LanguageSeverTest.hpp"
#include "../tests/CppHelperTests.hpp"
#include <chrono>
#include "JitPerformance.hpp"
#include "UCodeLangProjectPaths.hpp"
#include "UCodeLang/RunTime/TestRuner.hpp"
using namespace UCodeLang;

#ifdef UCodeLangDebug
const UCodeLang::String TopDir = UCodeLang_UCAppDir_ScrDir + "CodeTesting/";
const UCodeLang::String CodeTestingModluePath = TopDir + "ULangModule.ucm";
#else
const UCodeLang::String TopDir = "n/a";
const UCodeLang::String CodeTestingModluePath = "n/a";
const UCodeLang::String UCodeLangVSAPIPath = "n/a";
#endif // UCodeLangDebug

#define StandardLibrarynamespace "ULang"


struct Vec3
{
	int X;
	int Y;
	int Z;
};
struct Vec2
{
	int X;
	int Y;
};

int UCodeLangAPI Test(int A,int B)
{
	std::cout << "DLLCall Got Value " << A << "," << B << std::endl;
	return 0xbbc;
}

void UCodeLangAPI ULang_Test(InterpreterCPPinterface& Input)
{
	int A = Input.GetParameter<int>();
	int B = Input.GetParameter<int>();
	Input.Set_Return(Test(A,B));
}



template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}
void TestFormater()
{
	String Str = "$Hello :\n  private:\n   int a = 10;\n   int b = 20;\n  public:\n    int a = 10;\n    int b = 20;\n";

	UCodeAnalyzer::Formater _F;
	auto V = _F.Format(UCodeAnalyzer::Formater::StrScope::FileScope, Str);

	auto Output = _F.Get_Output();
}


/// <summary>
/// this is for testing and debuging features. 
/// do whatever you want here.
/// </summary>
void TestingGround()
{
	Interpreter RunTime;



	ULangTest::TestGenerator V;
	V.SetSeed(1);

	V.Reset();
	V.MakeFile();

	String OutFile = V.Get_OutFile();



	ModuleIndex LangIndex;
	auto Path = ModuleIndex::GetModuleIndexFilePath();
	//if (!std::filesystem::exists(Path))
	{
		LangIndex.AddModueToList(UCodeLangVSAPIPath + "\\StandardLibrary\\ULangModule.ucm");
		LangIndex.AddModueToList(UCodeLangVSAPIPath + "\\Win32\\ULangModule.ucm");

		LangIndex.AddModueToList(UCodeLangVSAPIPath + "\\NStandardLibrary\\ULangModule.ucm");
		LangIndex.AddModueToList(UCodeLangVSAPIPath + "\\NWin32\\ULangModule.ucm");
		ModuleIndex::ToFile(&LangIndex, Path);
	}


	ModuleIndex::FromFile(&LangIndex, Path);


	UCodeLang::Compiler _Compiler;
	UCodeLang::CompliationSettings& Settings = _Compiler.Get_Settings();

	Settings._Type = OutPutType::Lib;
	Settings._Flags = OptimizationFlags::Stable_ForDebuging;
	//_Compiler.Set_BackEnd(ULangTest::C89Backend::MakeObject);

	ModuleFile Mfile;
	ModuleFile::FromFile(&Mfile, CodeTestingModluePath);

	auto OutData = Mfile.BuildModule(_Compiler, LangIndex);



	if (!ULangTest::LogErrors(std::cout, _Compiler))
	{
		UCodeLang::UClib MLib;
		if (UClib::FromFile(&MLib, OutData.OutputItemPath))
		{
			auto Text = UAssembly::UAssembly::ToString(&MLib, Mfile.ThisModuleDir / Mfile.ModuleSourcePath);
			String Path = OutData.OutputItemPath.generic_string() + ".UA";
			std::ofstream out(Path);
			out << Text;
			out.close();
		}

		{
			TestRuner runer;
			auto info = runer.RunTests(MLib, TestRuner::InterpreterType::Interpreter, [](TestRuner::TestInfo& test)
				{
					if (test.Passed)
					{
						std::cout << "Test :" << test.TestName << " Passed\n";
					}
					else
					{
						std::cout <<  "Test :" << test.TestName << " Fail\n";
					}
				});
			bool passed = info.TestCount == info.TestPassedCount;
			std::cout << "Ran all " << info.TestCount << " Tests\m";

			int passnumber;
			if (info.TestPassedCount)
			{
				passnumber = ((float)info.TestPassedCount / (float)info.TestCount) * 100;
			}
			else
			{
				passnumber = 100;
			}


			if (passed)
			{
				std::cout << "Tests Passed.all 100% of tests passed\n";
			}
			else
			{
				std::cout << "Tests Failed about " << passnumber << "% passed\n";
			}
		}


		UCodeLang::RunTimeLib Lib;
		Lib.Init(&MLib);

		UCodeLang::RunTimeLib DLLib;

		Lib.Add_CPPCall("Testing:Test", ULang_Test, Test);

		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		UCodeLang::ProfilerDebuger debuger;
		debuger.Attach(&State);

		RunTime.Init(&State);
		//RunTime.IsDebug = true;
		//RunTime.AlwaysJit = true;

		auto FuncMain = State.Get_Assembly().Get_GlobalObject_Class()->Get_ClassMethod("main");

		//auto Value = RunTime.RCall<char>("__ReadChar");
		RunTime.Call(StaticVariablesInitializeFunc);
		RunTime.Call(ThreadVariablesInitializeFunc);

		auto CallIndex = State.FindAddress(FuncMain->DecorationName);



		Vec3 BufferToCopy[3]{ 1,2,3 };


		auto AutoPtr = RunTime.RCall<int>(FuncMain);


		//std::cout << " Got Value " << (int)AutoPtr << std::endl;

		RunTime.Call(ThreadVariablesUnLoadFunc);
		RunTime.Call(StaticVariablesUnLoadFunc);

		RunTime.UnLoad();
	}
}