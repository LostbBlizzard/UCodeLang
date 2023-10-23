#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>
#include "TestNameSpace.hpp"
#include "../src/UCodeLangProjectPaths.hpp"
#include <UCodeLang/RunTime/TestRuner.hpp>

#include <UCodeLang/Compliation/Back/C89/C89Backend.hpp>

#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix
#include <dlfcn.h>
#else

#endif 
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

	

	static const Array<TestInfo, 92> Tests
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
		TestInfo("Lamdba_2","Objects/Lambda.uc","main2",SuccessCondition::RunTimeValue,(int)5),
		TestInfo("Lamdba_3","Objects/Lambda2.uc","main",SuccessCondition::RunTimeValue,(int)-5),
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
		TestInfo("inferredgenericreturn", "Generics/inferredgenericreturn.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)10),
		TestInfo("inferredenumfield", "Generics/inferredenumfield.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	};

	struct SkipTestRange
	{
		size_t Start;
		size_t End;
	};
	static const Vector<SkipTestRange> UCodeVmSkipTests
		= { {10, Tests.size() } };
	static const Vector<SkipTestRange> C89SkipTests
		= { {40, Tests.size() } };
	static const Vector<SkipTestRange> WasmSkipTests
		= { {4, Tests.size() } };

	inline bool ShouldSkipTest(size_t Index, const Vector<SkipTestRange>& list)
	{
		for (auto& Item : list)
		{
			if (Item.Start <= Index && Item.End > Index)
			{
				return true;
			}
		}
		return false;
	}

	inline bool ShouldVmSkipTest(size_t Index)
	{
		return ShouldSkipTest(Index, UCodeVmSkipTests);
	}
	inline bool ShouldC89SkipTest(size_t Index)
	{
		return ShouldSkipTest(Index, C89SkipTests);
	}
	inline bool ShouldWasmSkipTest(size_t Index)
	{
		return ShouldSkipTest(Index, WasmSkipTests);
	}

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
		WasmBackEnd,

		Max,
	};
	inline String TestModeToName(TestMode mode)
	{
		switch (mode)
		{
		case ULangTest::TestMode::UCodeLangBackEnd:
		{
			return "UCodeVm";
		}
		break;
		case ULangTest::TestMode::CLang89BackEnd:
		{
			return "C89";
		}
		break;
		case ULangTest::TestMode::WasmBackEnd:
		{
			return "Wasm";
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	inline bool ShouldSkipTests(size_t Index,TestMode mode)
	{
		switch (mode)
		{
		case ULangTest::TestMode::UCodeLangBackEnd:
			return ShouldVmSkipTest(Index);
		case ULangTest::TestMode::CLang89BackEnd:
			return ShouldC89SkipTest(Index);
		case ULangTest::TestMode::WasmBackEnd:
			return ShouldWasmSkipTest(Index);
		default:
			UCodeLangUnreachable();
			break;
		}
	}

	bool RunTest(const TestInfo& Test, TestMode mode);

	bool RunTest(const ModuleTest& Test, TestMode mode);

	int RunTests(bool MultThread =true);

	bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler);

	inline bool CompileC89ToLib(const Path& Cfile, const Path& Outdllfile)
	{
		String Cmd = "gcc " + Cfile.generic_string();
		Cmd += " -shared -std=c89 -g";
		#if UCodeLang_32BitSytem
		Cmd += " -m32";
		#endif
		Cmd += " -o " + Outdllfile.generic_string();
		return system(Cmd.c_str()) == EXIT_SUCCESS;
	}

	class TestRuner
	{
	public:
		using TestInfo = UCodeLang::TestRuner::TestInfo;
		using TestsResult = UCodeLang::TestRuner::TestsResult;
		using OnTestDone = UCodeLang::TestRuner::OnTestDone;
		inline TestsResult RunTests(UClib& lib,const Path& outfilepath, TestMode Type, Optional<OnTestDone> OnDone = {})
		{
			switch (Type)
			{
			case TestMode::UCodeLangBackEnd:
			{
				UCodeLang::TestRuner runer;
				return runer.RunTests(lib, UCodeLang::TestRuner::InterpreterType::Interpreter,OnDone);
			}
			break;
			case TestMode::CLang89BackEnd:
			{
				auto tests = UCodeLang::TestRuner::GetTests(lib.Get_Assembly());
				Path dllfile = outfilepath.native() + Path(".lib").native();

				TestsResult r;
				r.Tests.resize(tests.size());

				bool v = CompileC89ToLib(outfilepath, dllfile);
				UCodeLangAssert(v);
				
				if (v)
				{

					auto staticinitname = C89Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
					auto threadinitname = C89Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

					auto staticdeinitname = C89Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
					auto threaddeinitname = C89Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);


					#if UCodeLang_Platform_Windows
					auto lib = LoadLibrary(dllfile.c_str());
					UCodeLangDefer(FreeLibrary(lib));
					auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
					auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
					auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
					auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());
					#elif UCodeLang_Platform_Posix
					auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
					UCodeLangDefer(dlclose(lib));

					auto staticinittocall = dlsym(lib, staticinitname.c_str());
					auto threadinittocall = dlsym(lib, threadinitname.c_str());
					auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
					auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());
					#endif       


					for (size_t i = 0; i < r.Tests.size(); i++)
					{
						auto& TestOut = r.Tests[i];
						auto& TestFunc = tests[i];

						bool passed = true;

						
						auto cfuncname = C89Backend::UpdateToCindentifier(TestFunc->DecorationName);
						
						#if UCodeLang_Platform_Windows
						auto functocall = GetProcAddress(lib, cfuncname.c_str());
						#elif UCodeLang_Platform_Posix
						auto functocall = dlsym(lib, cfuncname.c_str());
						#endif   

						UCodeLangAssert(functocall);

						{
							using Func = void(*)();
							((Func)staticinittocall)();
							((Func)threadinittocall)();
						}

						{
							using Func = bool(*)();
							passed = ((Func)functocall)();
						}

						{
							using Func = void(*)();
							((Func)staticdeinittocall)();
							((Func)threaddeinittocall)();
						}

						TestOut.Passed = passed;
						TestOut.TestName = TestFunc->DecorationName;

						if (passed)
						{
							r.TestPassedCount++;
						}
						r.TestCount++;

						if (OnDone.has_value())
						{
							OnDone.value()(TestOut);
						}
					}
				}
				return r;
			}
			break;
			case TestMode::WasmBackEnd:
			{
				int a = 0;
			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
			return {};
		}
	private:
	};
	
UCodeTestEnd