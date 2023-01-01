#include "Test.hpp"

bool RunTest(const TestInfo& Test)
{
	//std::cout << "Runing Test '" << Test.TestName << "'" << std::endl;

	UCodeLang::Compiler::CompilerPathData paths;
	UCodeLang::Compiler Com;
	UCodeLang::Compiler::CompilerRet Com_r;
	std::string InputFilesPath = Test_UCodeFiles + Test.InputFilesOrDir;
	std::string OutFileDir = Test_OutputFiles + Test.TestName;
	std::filesystem::path p = OutFileDir;
	OutFileDir = p.parent_path().generic_string() + Test.TestName + "/";

	std::filesystem::create_directories(OutFileDir);
	std::string OutFilePath = OutFileDir + Test.TestName + ".ulibtest";

	paths.FileDir = InputFilesPath;
	paths.OutFile = OutFilePath;

	if (std::filesystem::is_directory(InputFilesPath))
	{
		Com_r = Com.CompileFiles(paths);
	}
	else
	{
		Com_r = Com.CompilePathToObj(paths.FileDir, paths.OutFile);
	}

	if (Test.Condition == SuccessCondition::Compilation)
	{
		if (Com_r._State == UCodeLang::Compiler::CompilerState::Success)
		{
			std::cout << "Success from test '" << Test.TestName << "'" << std::endl;
			return true;
		}
		else
		{
			std::cerr << "fail from test '" << Test.TestName << "'" << std::endl;
			return false;
		}

	}

	if (Test.Condition == SuccessCondition::CompilationFail)
	{
		if (Com_r._State != UCodeLang::Compiler::CompilerState::Success)
		{
			std::cout << "Success from test '" << Test.TestName << "'" << std::endl;
			return true;
		}
		else
		{
			std::cerr << "fail from test '" << Test.TestName << "'" << std::endl;
			return false;
		}

	}

	if (Com_r._State != UCodeLang::Compiler::CompilerState::Success)
	{
		std::cerr << "fail from test [Cant Compile File/Files] '" << Test.TestName << "'" << std::endl;
		return false;
	}


	UCodeLang::RunTimeLangState state;
	UCodeLang::UClib lib;
	if (!UCodeLang::UClib::FromFile(&lib, OutFilePath))
	{
		std::cerr << "fail from test [Cant Open ULib File] '" << Test.TestName << "'" << std::endl;
		return false;
	}
	UCodeLang::RunTimeLib rLib;
	rLib.Init(&lib);
	state.AddLib(&rLib);

	UCodeLang::Interpreter RunTime;
	RunTime.Init(&state);

	UCodeLang::Interpreter::Return_t r;
	try
	{
		r = RunTime.Call(Test.TestName);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "fail from test [exception] " << ex.what() << ": " << "'" << Test.TestName << "'" << std::endl;
		return false;
	}

	auto RValue = r.ReturnValue.Value.AsUInt8;
	if (RValue == Test.RunTimeSuccess)
	{
		std::cout << "Success from test '" << Test.TestName << "'" << std::endl;
	}
	else
	{
		std::cerr << "fail from got value " << (int)RValue
			<< " but expecting " << (int)Test.RunTimeSuccess << ": '" << Test.TestName << "'" << std::endl;
		return false;
	}

	rLib.UnLoad();
	RunTime.UnLoad();

	return true;
}

void RunTests()
{
	size_t TestPassed = 0;
	std::cout << "---runing Test" << std::endl;
	for (auto& Test : Tests)
	{
		if (RunTest(Test))
		{
			TestPassed++;
		}
	}
	std::cout << "---Tests ended" << std::endl;
	std::cout << "passed " << TestPassed << "/" << Tests.size() << " Tests";
}
