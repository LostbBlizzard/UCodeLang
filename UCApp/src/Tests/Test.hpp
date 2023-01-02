#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <filesystem>

enum class SuccessCondition
{
	Compilation,
	CompilationFail,

	RunTimeValue,
};
struct TestInfo
{
	const char* TestName;
	const std::string InputFilesOrDir;
	const std::string FuncToCall;
	SuccessCondition Condition;
	UCodeLang::UInt8 RunTimeSuccess = 0;
};
static const std::vector<TestInfo> Tests
{
	{"main_0","BasicTests/main.uc","Main",SuccessCondition::Compilation},
	{"main ret 1","BasicTests/main.uc","Main2",SuccessCondition::RunTimeValue,1},
	{"BasicObjects_0","Objects/BasicObjects.uc","Main2",SuccessCondition::Compilation},
};



const UCodeLang::String _ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
const std::string Test_UCodeFiles = _ScrDir + "Tests/UCodeFiles/Files/";
const std::string Test_OutputFiles = _ScrDir + "Tests/UCodeFiles/Output/";

bool RunTest(const TestInfo& Test);

void RunTests();
