#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>

namespace ULangTest 
{
	using namespace UCodeLang;
	enum class SuccessCondition
	{
		Compilation,
		CompilationFail,

		RunTimeValue,
	};
	struct TestInfo
	{
		const char* TestName;
	String	InputFilesOrDir;
	String	FuncToCall;
		SuccessCondition Condition;


		Unique_Array<Byte> RunTimeSuccess;
		size_t RunTimeSuccessSize = 0;



		TestInfo(const char* TestName, const String InputFilesOrDir, String FuncToCall
			, SuccessCondition Condition)
		{
			this->TestName = TestName;
			this->InputFilesOrDir = InputFilesOrDir;
			this->FuncToCall = FuncToCall;
			this->Condition = Condition;

		}


		template<typename T>
		TestInfo(const char* TestName, const String InputFilesOrDir, const String FuncToCall
			, SuccessCondition Condition, const T& Value)
		{
			this->TestName = TestName;
			this->InputFilesOrDir = InputFilesOrDir;
			this->FuncToCall = FuncToCall;
			this->Condition = Condition;

			RunTimeSuccessSize = sizeof(Value);
			RunTimeSuccess = std::make_unique<Byte[]>(RunTimeSuccessSize);

			memcpy(RunTimeSuccess.get(), &Value, RunTimeSuccessSize);
		}
	};
#define AddTest 
#define AddTestEnd 

	static const Array<TestInfo, 9> Tests
	{

		AddTest TestInfo("main_0","BasicTests/main.uc","Main",SuccessCondition::Compilation) AddTestEnd,
		AddTest TestInfo("main ret 1","BasicTests/main.uc","Main2",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1) AddTestEnd,
		AddTest TestInfo("BasicObjects_0","Objects/BasicObjects.uc","Main2",SuccessCondition::Compilation) AddTestEnd,
		AddTest TestInfo("NewAndDrop","NewAndDrop/main.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0) AddTestEnd ,
		AddTest TestInfo("genericWithMemberAccess","Syntax/genericWithMemberAccess.uc","Main",SuccessCondition::CompilationFail) AddTestEnd,
		
		AddTest TestInfo("scopetest","BasicTests/scopetest.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0) AddTestEnd,
		AddTest TestInfo("BasicObjects ret 1","Objects/BasicObjects2.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1) AddTestEnd,
	
		AddTest TestInfo("GenericSize_t ret 4","Objects/GenericSize_t.uc","Main",SuccessCondition::RunTimeValue,(uintptr_t)sizeof(4)) AddTestEnd,
		AddTest TestInfo("DependencyCycle","Objects/DependencyCycle.uc","n/a",SuccessCondition::CompilationFail) AddTestEnd,
	};



	const String _ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
	const String Test_UCodeFiles = _ScrDir + "Tests/UCodeFiles/Files/";
	const String Test_OutputFiles = _ScrDir + "Tests/UCodeFiles/Output/";

	bool RunTest(const TestInfo& Test);

	void RunTests();

	bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler);
}