#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>
#include "main.h"
#include <future>

#include "UCodeAnalyzer/TextBuilder/UCodeTextBuilder.hpp"
#include "UCodeAnalyzer/CodeBuilds/StandardLibraryBuilder.hpp"

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


struct MyStruct
{
	void Hi2(Int32 A)
	{

	}
	Int32 Hi()
	{
		return 10;
	}
};

void VoidTest(Int32 A)
{

}


Int32 Test(Int32 A)
{
	return A;
}

static UCodeRunTime RunTime;
int main()
{
	//std::async(std::launch::async, [] { RunTests(); });

	
	
	UCodeLang::Compiler _Compiler;
	UCodeLang::CompliationSettings& Settings =_Compiler.Get_Settings();
	UCodeLang::Compiler::CompilerPathData Data;
	//Main
	Settings._Type = OutPutType::Lib;
	Data.FileDir = FileDir;
	Data.OutFile = OutFilePath;
	Settings._Flags = OptimizationFlags::Debug;
	_Compiler.CompileFiles(Data);
	if (!LogErrors(_Compiler))
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
		Lib.Add_CPPCall<'0'>("Test", &Test);
		Lib.Add_CPPCall<'1'>("VoidTest", &VoidTest);
		//Lib.Add_CPPCall("Hi2", &MyStruct::Hi);
		//Lib.Add_CPPCall("Hi", &MyStruct::Hi2);

		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		
		RunTime.Init(&State);
		
		auto Pointer = 50;

		auto r =
			RunTime.retCall<int>("main2",(Int32&)Pointer);
 		
		

		std::cout << " Got Value " << r << std::endl;

		RunTime.Call(StaticVariablesUnLoadFunc);


	}
}

void CompileTests()
{
	const UCodeLang::String Output = TopDir + "Output/";
	UCodeLang::Compiler _Compiler;
	UCodeLang::CompliationSettings& Settings = _Compiler.Get_Settings();
	UCodeLang::Compiler::CompilerPathData Data;
	//Loop Dir for TestMain.Uc and make a test for it.
}

bool LogErrors(UCodeLang::Compiler& _Compiler)
{
	auto& Errors = _Compiler.Get_Errors().Get_Errors();
	for (auto& Item : Errors)
	{
		std::cout << Item.ToString() << std::endl;
	}
	return _Compiler.Get_Errors().Has_Errors();
}
