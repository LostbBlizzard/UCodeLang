#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>
#include "Tests/Test.hpp"
#include <future>
#include <optional>
#include "UCodeAnalyzer/Formater.hpp"
#include <sstream>
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/x86/X86BackEnd.hpp"
#include "UCodeLang/Compliation/ModuleFile.hpp"
#include "UCodeLang/RunTime/Debuger.hpp"
#include "Tests/TestGenerator.hpp"

using namespace UCodeLang;
const UCodeLang::String ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
const UCodeLang::String TopTestDir = ScrDir + "Tests/";
const UCodeLang::String TopTestUnitDir = TopTestDir + "Unit/";

const UCodeLang::String TopDir = ScrDir + "CodeTesting/";
const UCodeLang::String CodeTestingModluePath = TopDir + "ULangModule.ucm";
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

const UCodeLang::String UCodeLangVSProjectPath = "C:\\CoolStuff\\CoolCodeingStuff\\C++\\Projects\\UCodeLang";

const UCodeLang::String UCodeLangVSAPIPath = UCodeLangVSProjectPath + "\\UCodeAPI";

#define StandardLibrarynamespace "ULang"


int Test(int A)
{
	std::cout << "DLLCall Got Value " << A << std::endl;
	return A;
} 

void Test2()
{
	std::cout << "Hello World" << std::endl;
}

MakeNewCPPCall(Test,int,int);

MakeNewCPPCall_voidNoPar(Test2);

static Interpreter RunTime;
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

int main()
{

	//ULangTest::RunTests();

	/*
	{
		UCodeLang::TokenType_t v = (UCodeLang::TokenType_t)UCodeLang::TokenType::Null;
		while (v != (UCodeLang::TokenType_t)UCodeLang::TokenType::internal_Constant_expression)
		{
			auto str =UCodeLang::StringHelper::ToString((UCodeLang::TokenType)v);
			if ((String)str == (String)"Unknown token")
			{
				throw std::exception("not added");
			}
			v++;
		}
	}
	*/

	//TestFormater();

	ULangTest::TestGenerator V;
	V.SetSeed(1);
	
	V.Reset();
	V.MakeFile();

 	String OutFile =V.Get_OutFile();


	
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
	UCodeLang::CompliationSettings& Settings =_Compiler.Get_Settings();
	
	//Settings.PtrSize = IntSizes::Int8;

	
	Settings._Type = OutPutType::Lib;
	Settings._Flags = OptimizationFlags::Debug;
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
			String Path =  OutData.OutputItemPath.generic_string() + ".UA";
			std::ofstream out(Path);
			out << Text;
			out.close();
		}


		UCodeLang::RunTimeLib Lib;
		Lib.Init(&MLib);

		UCodeLang::RunTimeLib DLLib;

		Lib.Add_CPPCall("Test", GetCPPCallName(Test));
		Lib.Add_CPPCall("Test2", GetCPPCallName(Test2));

		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		UCodeLang::Debuger debuger;
		debuger.Attach(&RunTime);

		RunTime.Init(&State);

		auto FuncMain  = State.Get_Assembly().Get_GlobalObject_Class()->Get_ClassMethod("main");
		
		//auto Value = RunTime.RCall<char>("__ReadChar");
		RunTime.Call(StaticVariablesInitializeFunc);
		RunTime.Call(ThreadVariablesInitializeFunc);

		auto CallIndex = State.FindAddress(FuncMain->DecorationName);
		auto AutoPtr = RunTime.RCall<UAddress>(*FuncMain,5,5,5,10,2);

		

		std::cout << " Got Value " << (int)0 << std::endl;
	
		RunTime.Call(StaticVariablesUnLoadFunc);
		RunTime.Call(ThreadVariablesUnLoadFunc);

		RunTime.UnLoad();
	}
}
