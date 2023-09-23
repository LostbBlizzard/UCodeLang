#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>
#include "TestNameSpace.hpp"
#include "../src/UCodeLangProjectPaths.hpp"
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
	inline String ModeType(OptimizationFlags Flags)
	{
		if (Flags == OptimizationFlags::O_None)
		{
			return "flagsnone";
		}
		String r;
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
	static const Array<TestInfo, 90> Tests
	{

		TestInfo("main_0","BasicTests/main.uc","Main",SuccessCondition::Compilation),

		TestInfo("Byte_Test_ret_1_byte","BasicTests/main.uc","Byte_Test",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1),
		TestInfo("Char_Test_ret_1_char","BasicTests/main.uc","Char_Test",SuccessCondition::RunTimeValue,(UCodeLang::Byte)'1'),
		TestInfo("Float_Test_ret_1_Float","BasicTests/main.uc","Float_Test",SuccessCondition::RunTimeValue,(UCodeLang::float32)1.5),

		TestInfo("BasicObjects_0","Objects/BasicObjects.uc","Main2",SuccessCondition::Compilation),
		TestInfo("NewAndDrop","NewAndDrop/main.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("genericWithMemberAccess","Syntax/genericWithMemberAccess.uc","Main",SuccessCondition::CompilationFail),

		TestInfo("scopetest","BasicTests/scopetest.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)0),
		TestInfo("BasicObjects_ret_1","Objects/BasicObjects2.uc","Main",SuccessCondition::RunTimeValue,(UCodeLang::Byte)1),

		TestInfo("GenericSize_t_ret_4","Objects/GenericSize_t.uc","Main",SuccessCondition::RunTimeValue,sizeof(int)),
		TestInfo("DependencyCycle","Objects/DependencyCycle.uc","n/a",SuccessCondition::CompilationFail),

		TestInfo("pointers_1","BasicTests/pointers.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("pointers_2","BasicTests/pointers.uc","main2",SuccessCondition::RunTimeValue,(int)1),


		TestInfo("loops_1","BasicTests/loops.uc","main_if",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops_2","BasicTests/loops.uc","main_else",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("loops_3","BasicTests/loops.uc","main_while",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops_4","BasicTests/loops.uc","main_do",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops_5","BasicTests/loopsif.uc","main",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("loops_6","BasicTests/loopsfor.uc","main",SuccessCondition::RunTimeValue,(int)10),


		TestInfo("Func_1","BasicTests/Func.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("MemberInit","Objects/MemberInit.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Constructor","Objects/Constructor.uc","main",SuccessCondition::RunTimeValue,(float)0),
		TestInfo("MemberCall","Objects/MemberCall.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("pointer_3","Objects/pointer3.uc","main",SuccessCondition::RunTimeValue,(char)'0'),
		TestInfo("FuncOverloading","Objects/FuncOverloading.uc","main",SuccessCondition::RunTimeValue,(int)0),

		TestInfo("OperatorOverloading","Objects/OperatorOverloading.uc","test",SuccessCondition::RunTimeValue,Vec2(0,0)),

		TestInfo("factorial","BasicTests/factorial.uc","main",SuccessCondition::RunTimeValue,(int)3628800),

		TestInfo("Enum_1","Objects/Enum.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Enum_2","Objects/Enum2.uc","main",SuccessCondition::RunTimeValue,(float)1.5),

		TestInfo("FuncPointer_1","BasicTests/FuncPointer.uc","main",SuccessCondition::RunTimeValue,(int)0),
		TestInfo("FuncPointer_2","Objects/FuncPointer.uc","main",SuccessCondition::RunTimeValue,(int)0),

		TestInfo("GenericsParameter","Generics/GenericsParameter.uc","main",SuccessCondition::RunTimeValue,(size_t)0),
		TestInfo("GenericType","Generics/GenericType.uc","main",SuccessCondition::RunTimeValue,(size_t)0),
		TestInfo("GenericsParameter_2","Generics/GenericsParameter2.uc","main",SuccessCondition::RunTimeValue,(size_t)0),

		TestInfo("HardAlias","BasicTests/HardAlias.uc","main",SuccessCondition::Compilation),
		TestInfo("HardAlias_2","BasicTests/HardAlias2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("HardAlias_3","BasicTests/HardAlias3.uc","main",SuccessCondition::CompilationFail),

		TestInfo("Index","BasicTests/Index.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Index_2","BasicTests/Index2.uc","main",SuccessCondition::RunTimeValue,(bool)true),

		TestInfo("GenericsExpression","Generics/GenericsExpression.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("GenericsExpression2","Generics/GenericsExpression2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("GenericsExpression3","Generics/GenericsExpression3.uc","main",SuccessCondition::CompilationFail),

		TestInfo("Lamdba_1","Objects/Lambda.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba_2","Objects/Lambda.uc","main2",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba_3","Objects/Lambda2.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba_4","Objects/Lambda2.uc","main2",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba_5","Objects/Lambda3.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Lamdba_6","Objects/Lambda3.uc","main2",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("enumvariant","Objects/enumvariant.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("enumvariant_2","Objects/enumvariant2.uc","main",SuccessCondition::RunTimeValue,(int)10),

		TestInfo("enumvariantdrop","NewAndDrop/enumvariantdrop.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("StaticArrDrop","NewAndDrop/StaticArrDrop.uc","main",SuccessCondition::RunTimeValue,true),
		TestInfo("dymArr","NewAndDrop/dymArr.uc","main",SuccessCondition::RunTimeValue,true),

		TestInfo("trait_1","Objects/trait.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("trait_2","Objects/trait2.uc","main",SuccessCondition::CompilationFail),
		TestInfo("trait_3","Objects/dynamic_trait.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("trait_4","Objects/dynamic_trait2.uc","main",SuccessCondition::RunTimeValue,(int)1),

		TestInfo("GenericsPack","Generics/GenericsPack.uc","main",SuccessCondition::RunTimeValue,(int)3),

		TestInfo("ExtendedExpression","Objects/ExtendedExpression.uc","main",SuccessCondition::RunTimeValue,(int)2),

		TestInfo("EvalVarableMember","Objects/EvalVarableMember.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("TypeBinding","Objects/TypeBinding.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("GetFields","Eval/GetFields.uc","main",SuccessCondition::RunTimeValue,(int)15),
		TestInfo("GetFields2","Eval/GetFields2.uc","main",SuccessCondition::RunTimeValue,(int)7),
		TestInfo("GetFields3","Eval/GetFields3.uc","main",SuccessCondition::RunTimeValue,(int)16),
		TestInfo("GetFields_enum","Eval/GetEnumFields.uc","main",SuccessCondition::RunTimeValue,(int)3),
		TestInfo("GetFields_enum_2","Eval/GetEnumFields2.uc","main",SuccessCondition::RunTimeValue,(int)12),
		TestInfo("GetEnum_Base","Eval/GetEnumBase.uc","main",SuccessCondition::RunTimeValue,(bool)true),

		TestInfo("MatchAllSet","BasicTests/MatchAllSet.uc","main",SuccessCondition::CompilationFail),
		TestInfo("MatchAllSet_2","BasicTests/MatchAllSet2.uc","main",SuccessCondition::CompilationFail),

		TestInfo("outfunc_1","BasicTests/outfunc.uc","main",SuccessCondition::RunTimeValue,(int)10),
		TestInfo("outfunc_2","BasicTests/outfunc2.uc","main",SuccessCondition::RunTimeValue,(int)10),

		TestInfo("outoforder_1","OutofOrder/eval.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("tag_1","Eval/tag1.uc","main",SuccessCondition::Compilation),

		TestInfo("typeerror","Syntax/typeerror.uc","main",SuccessCondition::RunTimeValue,(bool)1),
		TestInfo("Result","Syntax/Result.uc","main",SuccessCondition::RunTimeValue,(int)1),
		TestInfo("Range", "Syntax/Range.uc", "main", SuccessCondition::RunTimeValue, (int)22),
		TestInfo("Stringliterals", "BasicTests/Stringliteral.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)25),
		TestInfo("Charliterals", "BasicTests/Charliteral.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)8),

		TestInfo("UseDotAndNotScope", "Objects/UseDotAndNotScope.uc","main", SuccessCondition::CompilationFail),
		TestInfo("UsescopeAndnotdot", "Objects/UsescopeAndnotdot.uc", "main", SuccessCondition::CompilationFail),
		TestInfo("UsescopeAndnotdot_2", "Objects/UsescopeAndnotdot2.uc", "main", SuccessCondition::CompilationFail),
		TestInfo("OptionalOp", "Syntax/OptionalOp.uc", "main", SuccessCondition::RunTimeValue, (int)5),
		TestInfo("ResultOp", "Syntax/ResultOp.uc", "main", SuccessCondition::RunTimeValue, (int)5),
		TestInfo("MoveContruct", "Objects/MoveContruct.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
		TestInfo("CopyContruct", "Objects/CopyContruct.uc", "main", SuccessCondition::RunTimeValue, (bool)true),

		TestInfo("Unique_ptr", "Std/Unique_ptr.uc", "main", SuccessCondition::RunTimeValue, (int)1),
		TestInfo("QuadLoops", "BasicTests/QuadLoops.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)10),
		TestInfo("Array", "Array/Array.uc", "main", SuccessCondition::RunTimeValue, (int)10),
		TestInfo("defer", "Objects/defer.uc", "main", SuccessCondition::RunTimeValue, (int)1),
		TestInfo("StdString", "Std/String.uc", "main", SuccessCondition::RunTimeValue, (bool)1),
	};



	struct ModuleTest
	{
		const char* TestName;
		String Modulefile;


		ModuleTest(const char* TestName, const String& Modulefile)
		{
			this->TestName = TestName;
			this->Modulefile = Modulefile;
		}
	};
	static const Array<ModuleTest, 1> ModuleTests
	{
		ModuleTest("BuildSystem","Modules/BuildSystem")

	};
	
	enum class TestMode
	{
		UCodeLangBackEnd,
		CLang89BackEnd,
	};

	bool RunTest(const TestInfo& Test, TestMode mode);

	bool RunTest(const ModuleTest& Test, TestMode mode);

	int RunTests(bool MultThread =true);

	bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler);

	inline bool CompileC89ToLib(const Path& Cfile, const Path& Outdllfile)
	{
		/*
		#if UCodeLangMSVC

		String msbuildfile = R"(<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="16.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
		<ItemGroup>
			<ProjectConfiguration Include="Debug|x64">
				<Configuration>Debug</Configuration>
				<Platform>x64</Platform>
			</ItemGroup>
			<Import Project = "$(VCTargetsPath)\Microsoft.Cpp.default.props" / >
			<PropertyGroup>
			<ConfigurationType>Application< / ConfigurationType>
			<PlatformToolset>v142< / PlatformToolset>
			</PropertyGroup>
			<Import Project = "$(VCTargetsPath)\Microsoft.Cpp.props" / >
			<ItemGroup>
			<ClCompile Include = ")" + Cfile.filename().generic_string() + R"("/>
			</ItemGroup>
			<ItemGroup>
			<ClInclude Include = ")" + Cfile.filename().generic_string() + R"("/>
			</ItemGroup>
			<Import Project = "$(VCTargetsPath)\Microsoft.Cpp.Targets" / >
			</Project>)";


		std::ofstream file(Path(Cfile.native() + Path(".vcxproj").native()));
		file << msbuildfile;
		file.close();

		String Cmd = "msbuild " + Cfile.generic_string();
		return system(Cmd.c_str()) == EXIT_SUCCESS;//yes i know the return value is not the same on all systems
		#endif
		
		#if UCodeLangGNUC
		*/
		String Cmd = "gcc " + Cfile.generic_string();
		Cmd += " -shared -std=c89 -g";
		Cmd += " -o " + Outdllfile.generic_string();
		return system(Cmd.c_str()) == EXIT_SUCCESS;//yes i know the return value is not the same on all systems
		/*
		#endif
		*/
	}
	
UCodeTestEnd