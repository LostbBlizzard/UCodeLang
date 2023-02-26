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

	struct Vec2
	{
		float X;
		float Y;
		Vec2()
		{
			X = 0;
			Y = 0;
		}
		Vec2(float x,float y)
		{
			X = x;
			Y = y;
		}
	};

	static const Array<TestInfo, 33> Tests
	{

		TestInfo("main_0","BasicTests/main.uc","Main",SuccessCondition::Compilation),
		
		TestInfo("Byte_Test ret 1(byte)","BasicTests/main.uc","Byte_Test",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1),
		TestInfo("Char_Test ret 1(char)","BasicTests/main.uc","Char_Test",SuccessCondition::RunTimeValue,(UCodeLang::Byte)'1'),
		TestInfo("Float_Test ret 1(Float)","BasicTests/main.uc","Float_Test",SuccessCondition::RunTimeValue,(UCodeLang::float32)1.5),
		
		TestInfo("BasicObjects_0","Objects/BasicObjects.uc","Main2",SuccessCondition::Compilation),
		TestInfo("NewAndDrop","NewAndDrop/main.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0),
		TestInfo("genericWithMemberAccess","Syntax/genericWithMemberAccess.uc","Main",SuccessCondition::CompilationFail),
		
	    TestInfo("scopetest","BasicTests/scopetest.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0),
		TestInfo("BasicObjects ret 1","Objects/BasicObjects2.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1),
	
		TestInfo("GenericSize_t ret 4","Objects/GenericSize_t.uc","Main",SuccessCondition::RunTimeValue,sizeof(int)),
		TestInfo("DependencyCycle","Objects/DependencyCycle.uc","n/a",SuccessCondition::CompilationFail),

		TestInfo("pointers1","BasicTests/pointers.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("pointers2","BasicTests/pointers.uc","main2",SuccessCondition::RunTimeValue,(int)1),


		TestInfo("loops1","BasicTests/loops.uc","main_if",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops2","BasicTests/loops.uc","main_while",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops3","BasicTests/loops.uc","main_do",SuccessCondition::RunTimeValue,(int)10),


		TestInfo("Func 1","BasicTests/Func.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("MemberInit","Objects/MemberInit.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("Constructor","Objects/Constructor.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("MemberCall","Objects/MemberCall.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("pointer3","Objects/pointer3.uc","main",SuccessCondition::RunTimeValue,(char)'\0'),
		TestInfo("FuncOverloading","Objects/FuncOverloading.uc","main",SuccessCondition::RunTimeValue,(int)0),

		TestInfo("OperatorOverloading","Objects/OperatorOverloading.uc","main",SuccessCondition::RunTimeValue,Vec2(0,0)),

		TestInfo("Enum 1","Objects/Enum.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Enum 2","Objects/Enum2.uc","main",SuccessCondition::RunTimeValue,(float)1.5),

		TestInfo("FuncPointer 1","BasicTests/FuncPointer.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("FuncPointer 2","Objects/FuncPointer.uc","main",SuccessCondition::RunTimeValue,(int)0),

		TestInfo("GenericsParameter","Generics/GenericsParameter.uc","main",SuccessCondition::RunTimeValue,(size_t)0),
		TestInfo("GenericType","Generics/GenericType.uc","main",SuccessCondition::RunTimeValue,(size_t)0),
		TestInfo("GenericsParameter2","Generics/GenericsParameter2.uc","main",SuccessCondition::RunTimeValue,(size_t)0),

		TestInfo("HardAlias","BasicTests/HardAlias.uc","main",SuccessCondition::Compilation),
		TestInfo("HardAlias2","BasicTests/HardAlias2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("HardAlias3","BasicTests/HardAlias3.uc","main",SuccessCondition::CompilationFail),
	};



	const String _ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
	const String Test_UCodeFiles = _ScrDir + "Tests/UCodeFiles/Files/";
	const String Test_OutputFiles = _ScrDir + "Tests/UCodeFiles/Output/";

	bool RunTest(const TestInfo& Test);

	void RunTests();

	bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler);
}