#include "Test.hpp"

using namespace UCodeLang;
std::string ModeType(OptimizationFlags Flags)
{
	if (Flags == OptimizationFlags::O_None)
	{
		return "flagsnone";
	}
	std::string r;
	auto flags = (OptimizationFlags_t)Flags;
	if (flags & (OptimizationFlags_t)OptimizationFlags::ForSize)
	{
		r += "Size";
	}
	if (flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed)
	{
		r += "Speed";
	}
	if (flags & (OptimizationFlags_t)OptimizationFlags::Debug)
	{
		r += "Debug";
	}
	return r;
}

const std::vector<OptimizationFlags> OptimizationFlagsToCheck
{
	OptimizationFlags::ForDebuging,
	OptimizationFlags::O_2,
	OptimizationFlags::O_3,
};
bool RunTestForFlag(const TestInfo& Test, OptimizationFlags flag)
{
	Compiler::CompilerPathData paths;
	Compiler Com;
	Com.Get_Settings()._Flags = flag;
	Compiler::CompilerRet Com_r;
	std::string InputFilesPath = Test_UCodeFiles + Test.InputFilesOrDir;
	std::string OutFileDir = Test_OutputFiles + Test.TestName;
	std::filesystem::path p = OutFileDir;
	OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

	std::filesystem::create_directories(OutFileDir);
	std::string OutFilePath = OutFileDir + Test.TestName + ModeType(flag) + ".ulibtest";

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
		if (Com_r._State == Compiler::CompilerState::Success)
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
		if (Com_r._State != Compiler::CompilerState::Success)
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

	if (Com_r._State != Compiler::CompilerState::Success)
	{
		std::cerr << "fail from test [Cant Compile File/Files] '" << Test.TestName << "'" << std::endl;
		return false;
	}


	RunTimeLangState state;
	UClib lib;
	if (!UClib::FromFile(&lib, OutFilePath))
	{
		std::cerr << "fail from test [Cant Open ULib File] '" << Test.TestName << "'" << std::endl;
		return false;
	}
	RunTimeLib rLib;
	rLib.Init(&lib);
	state.AddLib(&rLib);

	{
		Interpreter RunTime;
		RunTime.Init(&state);

		Interpreter::Return_t r;
		try
		{
			r = RunTime.Call(Test.TestName);
		}
		catch (const std::exception& ex)
		{
			std::cerr << "fail from test [exception] " << ex.what() << ": " << "'" << Test.TestName << "'" << std::endl;
			return false;
		}

		if (Test.Condition == SuccessCondition::RunTimeValue)
		{
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
		}
		RunTime.UnLoad();
	}

	{
		Jit_Interpreter JitRunTime;
		JitRunTime.Init(&state);
		
		Interpreter::Return_t r;
		try
		{
			r = JitRunTime.Call(Test.TestName);
		}
		catch (const std::exception& ex)
		{
			std::cerr << "fail from jit test [exception] " << ex.what() << ": " << "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
			return false;
		}


		if (Test.Condition == SuccessCondition::RunTimeValue)
		{
			auto RValue = r.ReturnValue.Value.AsUInt8;
			if (RValue == Test.RunTimeSuccess)
			{
				std::cout << "Success from jit test '" << Test.TestName << ModeType(flag) << "'" << std::endl;
			}
			else
			{
				std::cerr << "fail jit test got  value " << (int)RValue
					<< " but expecting " << (int)Test.RunTimeSuccess << ": '" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}
		}
		JitRunTime.UnLoad();
	}


	rLib.UnLoad();
}
bool RunTest(const TestInfo& Test)
{
	//std::cout << "Runing Test '" << Test.TestName << "'" << std::endl;
	bool V =true;
	for (auto Flag : OptimizationFlagsToCheck)
	{
		if (!RunTestForFlag(Test, Flag))
		{
			V = false;
		}
	}
	return V;
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
	std::cout << "passed " << TestPassed << "/" << Tests.size() << " Tests" << std::endl;
}
