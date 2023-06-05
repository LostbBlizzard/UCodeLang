#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>
#include "TestNameSpace.h"
UCodeTestStart

//Are for Unit testing and Regression Testing 
	
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

	static const Array<TestInfo, 58> Tests
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

		TestInfo("Index","BasicTests/Index.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Index","BasicTests/Index2.uc","main",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("GenericsExpression","Generics/GenericsExpression.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("GenericsExpression2","Generics/GenericsExpression2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("GenericsExpression3","Generics/GenericsExpression3.uc","main",SuccessCondition::CompilationFail),
	
		TestInfo("Lamdba 1","Objects/Lamdba.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba 2","Objects/Lamdba.uc","main2",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba 3","Objects/Lamdba2.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba 4","Objects/Lamdba2.uc","main2",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba 5","Objects/Lamdba3.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba 6","Objects/Lamdba3.uc","main2",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("enumvariant","Objects/enumvariant.uc","main",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("enumvariantdrop","NewAndDrop/enumvariantdrop.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("StaticArrDrop","NewAndDrop/StaticArrDrop.uc","main",SuccessCondition::RunTimeValue,true),
		TestInfo("dymArr","NewAndDrop/dymArr.uc","main",SuccessCondition::RunTimeValue,true),

		TestInfo("trait 1","Objects/trait.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("trait 2","Objects/trait2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("trait 3","Objects/dynamic_trait.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("trait 4","Objects/dynamic_trait2.uc","main",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("GenericsPack","Generics/GenericsPack.uc","main",SuccessCondition::RunTimeValue,(int)3),

		TestInfo("ExtendedExpression","Objects/ExtendedExpression.uc","main",SuccessCondition::RunTimeValue,(int)2),

		TestInfo("EvalVarableMember","Objects/EvalVarableMember.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("TypeBinding","Objects/TypeBinding.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("GetFields","Eval/GetFields.uc","main",SuccessCondition::RunTimeValue,(int)15),
		TestInfo("GetFields2","Eval/GetFields2.uc","main",SuccessCondition::RunTimeValue,(int)7),
		TestInfo("GetFields3","Eval/GetFields3.uc","main",SuccessCondition::RunTimeValue,(int)16),
	};



	const String _ScrDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCApp/src/";
	const String Test_UCodeFiles = _ScrDir + "Tests/UCodeFiles/Files/";
	const String Test_OutputFiles = _ScrDir + "Tests/UCodeFiles/Output/";

	bool RunTest(const TestInfo& Test);

	void RunTests();

	bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler);
	
UCodeTestEnd