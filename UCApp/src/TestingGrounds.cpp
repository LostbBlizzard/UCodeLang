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
#include "UCodeLang/RunTime/Debuger.hpp"

#include "../tests/TestGenerator.hpp"
#include "../tests/Test.hpp"
#include "../tests/LanguageSeverTest.hpp"
#include "../tests/CppHelperTests.hpp"
#include <chrono>
#include "JitPerformance.hpp"
#include "UCodeLangProjectPaths.hpp"
using namespace UCodeLang;

const UCodeLang::String TopDir = UCodeLang_UCAppDir_ScrDir + "CodeTesting/";
const UCodeLang::String CodeTestingModluePath = TopDir + "ULangModule.ucm";

const UCodeLang::String UCodeLangVSAPIPath = UCodeLang_SoultionDir + "UCodeAPI/";

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

	int a = 0;
}


/// <summary>
/// this is for testing and debuging features. 
/// do whatever you want here.
/// </summary>
void TestingGround()
{

	//V
	{
		//JitPerformance::main(JitPerformance::Task::Main);
	}
	ULangTest::CppHelperTests::RunTests();
	//ULangTest::RunTests(false);
 	//ULangTest::RunLanguageSeverTests();
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

	//Settings.PtrSize = IntSizes::Int8;


	Settings._Type = OutPutType::Lib;
	Settings._Flags = OptimizationFlags::NoOptimization;
	//_Compiler.Set_BackEnd(ULangTest::C89Backend::MakeObject);

	ModuleFile Mfile;
	ModuleFile::FromFile(&Mfile, CodeTestingModluePath);

	auto OutData = Mfile.BuildModule(_Compiler, LangIndex);

	


	if (!ULangTest::LogErrors(std::cout, _Compiler))
	{
		UCodeLang::UClib MLib;
		if (UClib::FromFile(&MLib, OutData.OutputItemPath))
		{
			auto Text = UAssembly::UAssembly::ToString(&MLib);
			String Path = OutData.OutputItemPath.generic_string() + ".UA";
			std::ofstream out(Path);
			out << Text;
			out.close();
		}


		UCodeLang::RunTimeLib Lib;
		Lib.Init(&MLib);

		UCodeLang::RunTimeLib DLLib;

		Lib.Add_CPPCall("Testing:Test", ULang_Test, Test);

		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		//UCodeLang::Debuger debuger;
		//debuger.Attach(&RunTime);

		RunTime.Init(&State);
		//RunTime.IsDebug = true;
		//RunTime.AlwaysJit = true;

		auto FuncMain = State.Get_Assembly().Get_GlobalObject_Class()->Get_ClassMethod("main");

		//auto Value = RunTime.RCall<char>("__ReadChar");
		RunTime.Call(StaticVariablesInitializeFunc);
		RunTime.Call(ThreadVariablesInitializeFunc);

		auto CallIndex = State.FindAddress(FuncMain->DecorationName);



		Vec3 BufferToCopy[3]{ 1,2,3 };


		auto AutoPtr = RunTime.RCall<int>(*FuncMain,(int)8);


		//std::cout << " Got Value " << (int)AutoPtr << std::endl;

		RunTime.Call(ThreadVariablesUnLoadFunc);
		RunTime.Call(StaticVariablesUnLoadFunc);

		RunTime.UnLoad();
	}
}