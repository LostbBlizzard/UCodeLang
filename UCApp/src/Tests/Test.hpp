#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
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
	std::string InputFilesOrDir;
	std::string FuncToCall;
	SuccessCondition Condition;
	
	
	UCodeLang::Unique_Array<UCodeLang::Byte> RunTimeSuccess;
	size_t RunTimeSuccessSize = 0;


	
	TestInfo(const char* TestName, const std::string InputFilesOrDir, std::string FuncToCall
		, SuccessCondition Condition)
	{
		this->TestName = TestName;
		this->InputFilesOrDir = FuncToCall;
		this->FuncToCall = FuncToCall;
		this->Condition = Condition;

	}


	template<typename T>
	TestInfo(const char* TestName, const std::string InputFilesOrDir, const std::string FuncToCall
		, SuccessCondition Condition, const T& Value)
	{
		this->TestName = TestName;
		this->InputFilesOrDir = FuncToCall;
		this->FuncToCall = FuncToCall;
		this->Condition = Condition;

		RunTimeSuccessSize = sizeof(Value);
		RunTimeSuccess = std::make_unique<Byte[]>(RunTimeSuccessSize);

		memcpy(RunTimeSuccess.get(), &Value, RunTimeSuccessSize);
	}
};
#define AddTest 
#define AddTestEnd 

static const UCodeLang::Array<TestInfo,5> Tests
{
	
	AddTest TestInfo("main_0","BasicTests/main.uc","Main",SuccessCondition::Compilation) AddTestEnd,
	AddTest TestInfo("main ret 1","BasicTests/main.uc","Main2",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1) AddTestEnd,
	AddTest TestInfo("BasicObjects_0","Objects/BasicObjects.uc","Main2",SuccessCondition::Compilation) AddTestEnd,
	AddTest TestInfo("NewAndDrop","NewAndDrop/main","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0) AddTestEnd ,
	AddTest TestInfo("genericWithMemberAccess","Syntax/genericWithMemberAccess.uc","Main",SuccessCondition::CompilationFail) AddTestEnd,
};



const UCodeLang::String _ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
const std::string Test_UCodeFiles = _ScrDir + "Tests/UCodeFiles/Files/";
const std::string Test_OutputFiles = _ScrDir + "Tests/UCodeFiles/Output/";

bool RunTest(const TestInfo& Test);

void RunTests();
