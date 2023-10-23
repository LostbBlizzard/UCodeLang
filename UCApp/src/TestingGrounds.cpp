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
#include "UCodeLang/Compliation/Back/WebAssembly/WebAssembly.hpp"
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
#include "TestingGrounds.hpp"
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

/// <summary>
/// this is for testing and debuging features. 
/// do whatever you want here.
/// </summary>
void TestingGround()
{
	Interpreter RunTime;


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
	_Compiler.Set_BackEnd(ULangTest::WebAssemblyBackEnd::MakeObject);
	
	ModuleFile Mfile;
	ModuleFile::FromFile(&Mfile, CodeTestingModluePath);


	std::filesystem::remove_all(Mfile.GetPaths(_Compiler).IntDir);
	std::filesystem::remove(Mfile.GetPaths(_Compiler).OutFile);

	auto OutData = Mfile.BuildModule(_Compiler, LangIndex);
	
	
	if (!ULangTest::LogErrors(std::cout, _Compiler))
	{
		UCodeLang::UClib& MLib =*OutData.CompilerRet.OutPut;
		

		ULangTest::RunTests(false);


		RunTests(MLib, OutData.OutputItemPath);


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
		
		RunTime.Call(StaticVariablesInitializeFunc);
		RunTime.Call(ThreadVariablesInitializeFunc);

		auto AutoPtr = RunTime.RCall<int>(FuncMain);

		//std::cout << " Got Value " << (int)AutoPtr << std::endl;

		RunTime.Call(ThreadVariablesUnLoadFunc);
		RunTime.Call(StaticVariablesUnLoadFunc);

		RunTime.UnLoad();
	}
}

void RunTests(UCodeLang::UClib& MLib,const Path& output)
{
	{
		ULangTest::TestRuner runer;
		auto info = runer.RunTests(MLib, output, ULangTest::TestMode::WasmBackEnd, [](TestRuner::TestInfo& test)
			{
				if (test.Passed)
				{
					std::cout << "Test :" << test.TestName << " Passed\n";
				}
				else
				{
					std::cout << "Test :" << test.TestName << " Fail\n";
				}
			});
		bool passed = info.TestCount == info.TestPassedCount;
		std::cout << "Ran all " << info.TestCount << " Tests\n";

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
}
