#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>
#include "main.h"

#include "UCodeCross/Core/UCodeTextBuilder.hpp"
#include "StandardLibraryBuilder.hpp"

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

void Test(Jit_Interpreter::CPPInput Input)
{



	Input.Set_Return();
}
static UCodeRunTime RunTime;
int main()
{
	UCodeLang::Compiler _Compiler;
	UCodeLang::CompliationSettings& Settings =_Compiler.Get_Settings();
	UCodeLang::Compiler::CompilerPathData Data;
	
	/*
	{//StandardLibrary
		StandardLibraryBuilder::BuildLibraryToDir(StandardLibraryPath);
		
		Settings._Type = OutPutType::DLL;
		Data.FileDir = StandardLibraryPath;
		Data.IntDir = StandardLibraryinit;
		Data.OutFile = StandardLibraryOut + StandardLibraryLibName + UCodeLang::FileExt::DllWithDot;
		_Compiler.CompileFiles(Data);
		_Compiler.LinkFilesToFile(Data);
		LogErrors(_Compiler);
	}
	*/
	
	
	
	//Main
	Settings._Type = OutPutType::Lib;
	Data.FileDir = FileDir;
	Data.IntDir = IntPath;
	Data.OutFile = OutFilePath;
	Settings._Flags = OptimizationFlags::Debug;
	_Compiler.CompileFiles(Data);
	_Compiler.LinkFilesToFile(Data);
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

		{
			Data.OutFile = OutFilePath + "Fast";
			Settings._Flags = OptimizationFlags::ForMaxSpeed;
			_Compiler.LinkFilesToFile(Data);

			UCodeLang::UClib FastLib;
			if (UClib::FromFile(&FastLib, Data.OutFile))
			{
				auto Text = UAssembly::UAssembly::ToString(&FastLib);
				String Path = Data.OutFile + ".UA";
				std::ofstream out(Path);
				out << Text;
				out.close();
			}

		}
		UCodeLang::RunTimeLib Lib;
		Lib.Init(&MLib);

		UCodeLang::RunTimeLib DLLib;
		Lib.Add_CPPCall("Test",Test);

		UCodeLang::RunTimeLangState State;
		State.AddLib(&Lib);
		State.AddLib(&DLLib);
		State.LinkLibs();

		
		RunTime.Init(&State);
		auto r = RunTime.Call("Main");
 		if (r._Succeed == UCodeLang::Interpreter::RetState::Error || r._Succeed == UCodeLang::Interpreter::RetState::Error_Function_doesnt_exist)
		{
			std::cout << "Calling Main Got us an Error" << std::endl;
		}
		auto V = r.ReturnValue.Value.AsUInt8;

		std::cout << " Got Value " << (void*)V << std::endl;

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

		const char* Type;
		if (UCodeLang::CompliationErrors::IsError(Item._Code))
		{
			Type = "Error";
		}
		else if (UCodeLang::CompliationErrors::IsWarning(Item._Code))
		{
			Type = "Warning";
		}
		else
		{
			Type = "N/A";
		}

		std::cout << Type  << " At Line:" << (Item.Line != Token::EndOfFile ? std::to_string(Item.Line) : "End of File") << ":" << Item._Msg << " In " << Item.File << std::endl;
	}
	return _Compiler.Get_Errors().Has_Errors();
}
