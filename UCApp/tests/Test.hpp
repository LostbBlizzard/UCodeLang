#pragma once
#include <string>
#include <vector>
#include <UCodeLang/UCodeLang.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
#include <filesystem>
#include "TestNameSpace.hpp"
#include "../src/UCodeLangProjectPaths.hpp"
#include <UCodeLang/RunTime/TestRuner.hpp>

#include <UCodeLang/Compilation/Back/C11/C11Backend.hpp>

#include <UCodeLang/Compilation/UAssembly/UAssembly.hpp>
#include <UCodeLang/Compilation/ModuleFile.hpp>

#include <UCodeLang/Compilation/Back/WebAssembly/WasmBackEnd.hpp>
#include "../src/UCodeLangProjectPaths.hpp"
#include <fstream>
#include <sstream>
#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix
#include <dlfcn.h>
#else

#endif
UCodeTestStart

// Are for Unit testing and Regression Testing

bool LogErrors(std::ostream& out, Compiler& _Compiler);

inline String OutputBytesToString(Byte *Bytes, size_t Size)
{
	std::stringstream stream;
	for (size_t i = 0; i < Size; i++)
	{
		if (i == 0)
		{
			// stream << "0x";
		}
		stream << std::to_string(Bytes[i]);
		if (i + 1 != Size)
		{
			stream << ",";
		}
	}
	stream << '\0';
	return stream.str();
}
enum class SuccessCondition {
	Compilation,
	CompilationFail,

	RunTimeValue,
	Panic,
	PanicUnitCheck,
};
struct TestInfo
{
	const char* TestName;
	String InputFilesOrDir;
	String FuncToCall;
	SuccessCondition Condition;

	Unique_Array<Byte> RunTimeSuccess;
	size_t RunTimeSuccessSize = 0;

	TestInfo(const char* TestName, const String InputFilesOrDir, String FuncToCall, SuccessCondition Condition)
	{
		this->TestName = TestName;
		this->InputFilesOrDir = InputFilesOrDir;
		this->FuncToCall = FuncToCall;
		this->Condition = Condition;
	}

	template <typename T>
	TestInfo(const char* TestName, const String InputFilesOrDir, const String FuncToCall, SuccessCondition Condition, const T& Value)
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

bool RunTimeOutput(
	std::ostream& LogStream,
	std::ostream& ErrStream,
	const TestInfo& Test,
	OptimizationFlags flag,
	std::unique_ptr<Byte[]>& RetState, std::string Type = "jit");

bool CompileC89ToLib(const Path& Cfile, const Path& Outdllfile);
struct Vec2
{
	float X;
	float Y;
	Vec2()
	{
		X = 0;
		Y = 0;
	}
	Vec2(float x, float y)
	{
		X = x;
		Y = y;
	}
};

template<typename T>
struct Array3
{
	Array<T, 3> arrays;
	Array3(T a, T b, T c)
	{
		arrays[0] = a;
		arrays[1] = b;
		arrays[2] = c;
	}
};
inline String ModeType(OptimizationFlags Flags)
{
	if (Flags == OptimizationFlags::O_None)
	{
		return "-None";
	}
	String r;
	auto flags = (OptimizationFlags_t)Flags;
	if (flags & (OptimizationFlags_t)OptimizationFlags::O_1)
	{
		r += "-01";
	}
	if (flags & (OptimizationFlags_t)OptimizationFlags::O_2)
	{
		r += "-02";
	}
	if (flags & (OptimizationFlags_t)OptimizationFlags::Debug)
	{
		r += "-Debug";
	}
	return r;
}

static const Array<TestInfo, 152> Tests{

	TestInfo("main_0", "BasicTests/main.uc", "Main", SuccessCondition::Compilation),

	TestInfo("Byte_Test_ret_1_byte", "BasicTests/main.uc", "Byte_Test", SuccessCondition::RunTimeValue, (UCodeLang::Byte)1),
	TestInfo("Char_Test_ret_1_char", "BasicTests/main.uc", "Char_Test", SuccessCondition::RunTimeValue, (UCodeLang::Byte)'1'),
	TestInfo("Float_Test_ret_1_Float", "BasicTests/main.uc", "Float_Test", SuccessCondition::RunTimeValue, (UCodeLang::float32)1.5),

	TestInfo("BasicObjects_0", "Objects/BasicObjects.uc", "Main2", SuccessCondition::Compilation),
	TestInfo("NewAndDrop", "NewAndDrop/main.uc", "main", SuccessCondition::RunTimeValue, (int)0),
	TestInfo("genericWithMemberAccess", "Syntax/genericWithMemberAccess.uc", "Main", SuccessCondition::CompilationFail),

	TestInfo("scopetest", "BasicTests/scopetest.uc", "Main", SuccessCondition::RunTimeValue, (UCodeLang::Byte)0),
	TestInfo("BasicObjects_ret_1", "Objects/BasicObjects2.uc", "Main", SuccessCondition::RunTimeValue, (UCodeLang::Byte)1),

	TestInfo("GenericSize_t_ret_4", "Objects/GenericSize_t.uc", "Main", SuccessCondition::RunTimeValue, sizeof(int)),
	TestInfo("DependencyCycle", "Objects/DependencyCycle.uc", "n/a", SuccessCondition::CompilationFail),

	TestInfo("pointers_1", "BasicTests/pointers.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("pointers_2", "BasicTests/pointers.uc", "main2", SuccessCondition::RunTimeValue, (int)1),

	TestInfo("loops_1", "BasicTests/loops.uc", "main_if", SuccessCondition::RunTimeValue, (int)10),
	TestInfo("loops_2", "BasicTests/loops.uc", "main_else", SuccessCondition::RunTimeValue, (int)0),
	TestInfo("loops_3", "BasicTests/loops.uc", "main_while", SuccessCondition::RunTimeValue, (int)10),
	TestInfo("loops_4", "BasicTests/loops.uc", "main_do", SuccessCondition::RunTimeValue, (int)10),
	TestInfo("loops_5", "BasicTests/loopsif.uc", "main", SuccessCondition::RunTimeValue, (int)10),
	TestInfo("loops_6", "BasicTests/loopsfor.uc", "main", SuccessCondition::RunTimeValue, (int)10),

	TestInfo("Func_1", "BasicTests/Func.uc", "main", SuccessCondition::RunTimeValue, (int)0),
	TestInfo("MemberInit", "Objects/MemberInit.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Constructor", "Objects/Constructor.uc", "func", SuccessCondition::RunTimeValue, (float)0),
	TestInfo("MemberCall", "Objects/MemberCall.uc", "main", SuccessCondition::RunTimeValue, (int)0),
	TestInfo("pointer_3", "Objects/pointer3.uc", "main", SuccessCondition::RunTimeValue, (char)'0'),
	TestInfo("FuncOverloading", "Objects/FuncOverloading.uc", "main", SuccessCondition::RunTimeValue, (int)0),

	TestInfo("OperatorOverloading", "Objects/OperatorOverloading.uc", "test", SuccessCondition::RunTimeValue, Vec2(0, 0)),

	TestInfo("factorial", "BasicTests/factorial.uc", "main", SuccessCondition::RunTimeValue, (int)3628800),

	TestInfo("Enum_1", "Objects/Enum.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Enum_2", "Objects/Enum2.uc", "func", SuccessCondition::RunTimeValue, (float)1.5),

	TestInfo("FuncPointer_1", "BasicTests/FuncPointer.uc", "main", SuccessCondition::RunTimeValue, (int)0),
	TestInfo("FuncPointer_2", "Objects/FuncPointer.uc", "func", SuccessCondition::RunTimeValue, (int)0),

	TestInfo("GenericsParameter", "Generics/GenericsParameter.uc", "main", SuccessCondition::RunTimeValue, (size_t)0),
	TestInfo("GenericType", "Generics/GenericType.uc", "main", SuccessCondition::RunTimeValue, (size_t)0),
	TestInfo("GenericsParameter_2", "Generics/GenericsParameter2.uc", "main", SuccessCondition::RunTimeValue, (size_t)0),

	TestInfo("HardAlias", "BasicTests/HardAlias.uc", "main", SuccessCondition::Compilation),
	TestInfo("HardAlias_2", "BasicTests/HardAlias2.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("HardAlias_3", "BasicTests/HardAlias3.uc", "main", SuccessCondition::CompilationFail),

	TestInfo("Index", "BasicTests/Index.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Index_2", "BasicTests/Index2.uc", "main", SuccessCondition::RunTimeValue, (bool)true),

	TestInfo("GenericsExpression", "Generics/GenericsExpression.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("GenericsExpression2", "Generics/GenericsExpression2.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("GenericsExpression3", "Generics/GenericsExpression3.uc", "main", SuccessCondition::CompilationFail),

	TestInfo("BitWise1", "BasicTests/Bitwise.uc", "main1", SuccessCondition::RunTimeValue, (int)2),
	TestInfo("BitWise2", "BasicTests/Bitwise.uc", "main2", SuccessCondition::RunTimeValue, (int)4),
	TestInfo("BitWise3", "BasicTests/Bitwise.uc", "main3", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("BitWise4", "BasicTests/Bitwise.uc", "main4", SuccessCondition::RunTimeValue, (int)13),
	TestInfo("BitWise5", "BasicTests/Bitwise.uc", "main5", SuccessCondition::RunTimeValue, (int)12),
	TestInfo("BitWise6", "BasicTests/Bitwise.uc", "main6", SuccessCondition::RunTimeValue, (int)-6),

	TestInfo("OperatorPrecedent1", "BasicTests/OperatorPrecedent.uc", "main", SuccessCondition::RunTimeValue, (int)7),
	TestInfo("OperatorPrecedent2", "BasicTests/OperatorPrecedent.uc", "main1", SuccessCondition::RunTimeValue, (int)7),
	TestInfo("OperatorPrecedent3", "BasicTests/OperatorPrecedent.uc", "main2", SuccessCondition::RunTimeValue, (int)9),
	TestInfo("OperatorPrecedent4", "BasicTests/OperatorPrecedent.uc", "main3", SuccessCondition::RunTimeValue, (int)-5),

	TestInfo("Lamdba_1", "Objects/Lambda.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Lamdba_2", "Objects/Lambda.uc", "main2", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("Lamdba_3", "Objects/Lambda2.uc", "main", SuccessCondition::RunTimeValue, (int)-5),
	TestInfo("Lamdba_4", "Objects/Lambda2.uc", "main2", SuccessCondition::RunTimeValue, (int)-4),
	TestInfo("Lamdba_5", "Objects/Lambda3.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Lamdba_6", "Objects/Lambda3.uc", "main2", SuccessCondition::RunTimeValue, (int)1),

	TestInfo("enumvariant", "Objects/enumvariant.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("enumvariant_2", "Objects/enumvariant2.uc", "main", SuccessCondition::RunTimeValue, (int)10),

	TestInfo("enumvariantdrop", "NewAndDrop/enumvariantdrop.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("enumvariantdrop2", "NewAndDrop/enumvariantdrop2.uc", "main", SuccessCondition::RunTimeValue, (int)4),

	TestInfo("StaticArrDrop", "NewAndDrop/StaticArrDrop.uc", "main", SuccessCondition::RunTimeValue, true),
	TestInfo("StaticArrDrop2", "NewAndDrop/StaticArrDrop2.uc", "main", SuccessCondition::RunTimeValue, true),

	TestInfo("dymArr", "NewAndDrop/dymArr.uc", "main", SuccessCondition::RunTimeValue, true),

	TestInfo("trait_1", "Objects/trait.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("trait_2", "Objects/trait2.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("trait_3", "Objects/dynamic_trait.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("trait_4", "Objects/dynamic_trait2.uc", "main", SuccessCondition::RunTimeValue, (int)1),

	TestInfo("GenericsPack", "Generics/GenericsPack.uc", "main", SuccessCondition::RunTimeValue, (int)3),

	TestInfo("ExtendedExpression", "Objects/ExtendedExpression.uc", "main", SuccessCondition::RunTimeValue, (int)2),

	TestInfo("EvalVarableMember", "Objects/EvalVarableMember.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("TypeBinding", "Objects/TypeBinding.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("GetFields", "Eval/GetFields.uc", "main", SuccessCondition::RunTimeValue, (size_t)15),
	TestInfo("GetFields2", "Eval/GetFields2.uc", "main", SuccessCondition::RunTimeValue, (size_t)7),
	TestInfo("GetFields3", "Eval/GetFields3.uc", "main", SuccessCondition::RunTimeValue, (size_t)24),
	TestInfo("GetFields_enum", "Eval/GetEnumFields.uc", "main", SuccessCondition::RunTimeValue, (int)3),
	TestInfo("GetFields_enum_2", "Eval/GetEnumFields2.uc", "main", SuccessCondition::RunTimeValue, (size_t)12),
	TestInfo("GetEnum_Base", "Eval/GetEnumBase.uc", "main", SuccessCondition::RunTimeValue, (bool)true),

	TestInfo("MatchAllSet", "BasicTests/MatchAllSet.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("MatchAllSet_2", "BasicTests/MatchAllSet2.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("MatchSameValueError", "BasicTests/MatchSameValueError.uc", "main", SuccessCondition::CompilationFail),

	TestInfo("outfunc_1", "BasicTests/outfunc.uc", "main", SuccessCondition::RunTimeValue, (int)10),

	TestInfo("outoforder_1", "OutofOrder/eval.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("tag_1", "Eval/tag1.uc", "main", SuccessCondition::Compilation),

	TestInfo("typeerror", "Syntax/typeerror.uc", "main", SuccessCondition::RunTimeValue, (bool)1),
	TestInfo("Result", "Syntax/Result.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("Range", "Syntax/Range.uc", "main", SuccessCondition::RunTimeValue, (int)22),
	TestInfo("Stringliterals", "BasicTests/Stringliteral.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)25),
	TestInfo("Charliterals", "BasicTests/Charliteral.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)8),

	TestInfo("UseDotAndNotScope", "Objects/UseDotAndNotScope.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("UsescopeAndnotdot", "Objects/UsescopeAndnotdot.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("UsescopeAndnotdot_2", "Objects/UsescopeAndnotdot2.uc", "main", SuccessCondition::CompilationFail),

	TestInfo("OptionalOp", "Syntax/OptionalOp.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("ResultOp", "Syntax/ResultOp.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("OptionalOp2", "Syntax/OptionalOp2.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("ResultOp2", "Syntax/ResultOp2.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("OptionalOp3", "Syntax/OptionalOp3.uc", "main", SuccessCondition::RunTimeValue, (int)5),
	TestInfo("ResultOp3", "Syntax/ResultOp3.uc", "main", SuccessCondition::RunTimeValue, (int)5),

	TestInfo("MoveContruct", "Objects/MoveContruct.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	TestInfo("CopyContruct", "Objects/CopyContruct.uc", "main", SuccessCondition::RunTimeValue, (bool)true),

	TestInfo("Unique_ptr", "Std/Unique_ptr.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("QuadLoops", "BasicTests/QuadLoops.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)10),
	TestInfo("Array", "Array/Array.uc", "main", SuccessCondition::RunTimeValue, (int)10),
	TestInfo("Array2", "Array/Array.uc", "main2", SuccessCondition::RunTimeValue, (int)6),

	TestInfo("defer", "Objects/defer.uc", "main", SuccessCondition::RunTimeValue, (int)1),
	TestInfo("StdString", "Std/String.uc", "main", SuccessCondition::RunTimeValue, (bool)1),
	TestInfo("inferredgenericreturn", "Generics/inferredgenericreturn.uc", "main", SuccessCondition::RunTimeValue, (uintptr_t)10),
	TestInfo("inferredenumfield", "Generics/inferredenumfield.uc", "main", SuccessCondition::RunTimeValue, (int)5),

	TestInfo("OptionalAsPtr", "Objects/OptionalAsPtr.uc", "NullOp", SuccessCondition::RunTimeValue, (bool)true),
	TestInfo("OptionalAsPtr2", "Objects/OptionalAsPtr.uc", "NullOp2", SuccessCondition::RunTimeValue, (int)20),
	TestInfo("factorial2", "BasicTests/factorial2.uc", "main", SuccessCondition::RunTimeValue, (int)3628800),
	TestInfo("StressTest1", "StressTest/StressTest1.uc", "main", SuccessCondition::RunTimeValue, (int)6),

	TestInfo("GenericOverload", "Generics/GenericsOverload.uc", "main", SuccessCondition::RunTimeValue, (bool)1),

	TestInfo("Attribute Contruct", "Eval/Attribute.uc","", SuccessCondition::Compilation),
	TestInfo("Attribute Contruct 2", "Eval/Attribute2.uc","", SuccessCondition::Compilation),

	TestInfo("defer2", "Objects/defer2.uc", "main", SuccessCondition::RunTimeValue, (int)64),

	TestInfo("ForType", "Objects/ForType.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	TestInfo("ForType2", "Objects/ForType2.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("NotLocaton", "BasicTests/NotLocation.uc", "main", SuccessCondition::CompilationFail),
	TestInfo("GetBytes",  "Array/GetBytes.uc", "main", SuccessCondition::RunTimeValue,(bool)1),
	
	TestInfo("MoveDrops",  "NewAndDrop/MoveDrops.uc", "main", SuccessCondition::RunTimeValue,(int)5),
	TestInfo("MoveDrops_2",  "NewAndDrop/MoveDrops2.uc", "main", SuccessCondition::RunTimeValue,(int)5),

	TestInfo("GenericTypePackUnPack","Generics/GenericTypePackUnPack.uc", "main", SuccessCondition::RunTimeValue,(int)15),
	TestInfo("GenericTypePackUnPack_2","Generics/GenericTypePackUnPack.uc", "main2", SuccessCondition::RunTimeValue,(int)30),

	TestInfo("TypePackWithNone","Generics/TypePackWithNone.uc", "main", SuccessCondition::RunTimeValue,(int)5),
	TestInfo("TypePackWithNone_2","Generics/TypePackWithNone.uc", "main2", SuccessCondition::RunTimeValue,(int)0),
	
	TestInfo("GenericEvalWithType","Generics/GenericEvalWithType.uc", "main", SuccessCondition::RunTimeValue,(uintptr_t)3),
	TestInfo("GenericEvalWithType_2","Generics/GenericEvalWithType2.uc", "main", SuccessCondition::CompilationFail),
	
	TestInfo("UnusedGenericPackParameter","Bugs/UnusedGenericPackParameter.uc", "main", SuccessCondition::CompilationFail),
	
	TestInfo("GenericPackWithType","Generics/GenericPackWithType.uc", "main", SuccessCondition::CompilationFail),
	
	TestInfo("GenericPackWithType_2","Generics/GenericPackWithType2.uc", "main", SuccessCondition::RunTimeValue,bool(true)),
	
	TestInfo("CompileTime_ForNode_Spit", "Objects/CompileTimeForNodeSplit.uc", "main", SuccessCondition::RunTimeValue,bool(true)),
	
	TestInfo("GetFields4", "Eval/GetFields4.uc", "main", SuccessCondition::RunTimeValue, (size_t)27),
	
	TestInfo("CustomForLoop", "Array/CustomForLoop.uc", "main", SuccessCondition::RunTimeValue, (int)6),
	TestInfo("CustomForLoop_2", "Array/CustomForLoop2.uc", "main", SuccessCondition::RunTimeValue, (int)9),
	
	TestInfo("AutoMadeContruct", "Objects/AutoMadeContruct.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("EnumCasts", "BasicTests/EnumCasts.uc", "main", SuccessCondition::RunTimeValue, (UCodeLang::Byte)2),
	TestInfo("EnumCasts_2", "BasicTests/EnumCasts.uc", "main2", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("Swap", "Std/Swap.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("ParOffsetCheck", "StressTest/ParOffsetCheck.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	TestInfo("ForTypePrimitive", "Objects/ForTypePrimitive.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("StringInterpolation", "Syntax/StringInterpolation.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	
	TestInfo("EnableTrue", "Eval/EnableTrue.uc", "main", SuccessCondition::RunTimeValue, (bool)true),
	TestInfo("EnableFalse", "Eval/EnableFalse.uc", "main", SuccessCondition::CompilationFail),
	
	TestInfo("Panic_1", "Panic/Panic.uc", "main", SuccessCondition::Panic,String_view("TestPanic")),
	
	TestInfo("Panic_2", "Panic/Panic2.uc", "main", SuccessCondition::PanicUnitCheck,String_view("TestPanic")),
	TestInfo("Panic_3", "Panic/Panic3.uc", "main", SuccessCondition::PanicUnitCheck,String_view("TestPanic")),
	
	TestInfo("DoubleConstructer", "Objects/DoubleConstructer.uc", "main", SuccessCondition::RunTimeValue,bool(true)),
	TestInfo("DoubleConstructer_2", "Objects/DoubleConstructer.uc", "main2", SuccessCondition::RunTimeValue,bool(true)),
	
	TestInfo("SpanKeepImut", "Std/SpanKeepImut.uc", "main", SuccessCondition::CompilationFail),

	TestInfo("Attribute Contruct 3", "Eval/Attribute3.uc","", SuccessCondition::CompilationFail),
	//TestInfo("ForTypeReadThis", "Objects/ForTypeReadThis.uc", "main", SuccessCondition::RunTimeValue,int(20)),
};
struct SkipTestRange
{
	size_t Start;
	size_t End;
};
static const Vector<SkipTestRange> UCodeVmSkipTests = { };
static const Vector<SkipTestRange> C89SkipTests = {  };
static const Vector<SkipTestRange> WasmSkipTests = { {0, Tests.size()} };

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
static const Array<ModuleTest, 1> ModuleTests{
	ModuleTest("Fmt_Print", "Modules/StandardLibraryCheck")

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
inline bool ShouldSkipTests(size_t Index, TestMode mode)
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

inline bool RunTimeOutput(
	std::ostream &LogStream,
	std::ostream &ErrStream,
	const TestInfo &Test,
	OptimizationFlags flag,
	std::unique_ptr<Byte[]> &RetState, std::string Type)
{
	bool IsSame = true;

	for (size_t i = 0; i < Test.RunTimeSuccessSize; i++)
	{
		if (RetState[i] != Test.RunTimeSuccess[i])
		{
			IsSame = false;
			break;
		}
	}

	if (IsSame)
	{
		LogStream << "Success from test '" << Test.TestName << "'" << ModeType(flag) << " " << Type << std::endl;
	}
	else
	{
		ErrStream << "fail from got value '";
		ErrStream << OutputBytesToString(RetState.get(), Test.RunTimeSuccessSize);

		ErrStream << "' but expecting '";
		ErrStream << OutputBytesToString(Test.RunTimeSuccess.get(), Test.RunTimeSuccessSize);
		ErrStream << ": '" << Type << "," << ModeType(flag) << "'" << Type << std::endl;
		return false;
	}
	return true;
}
const UCodeLang::Array<OptimizationFlags, 2> OptimizationFlagsToCheck{
	OptimizationFlags::NoOptimization,
	OptimizationFlags::Debug,

	// OptimizationFlags::O_1,
	// OptimizationFlags::ForDebuging,

	// OptimizationFlags::ForSize,
	// OptimizationFlags::ForSpeed,
	// OptimizationFlags::ForMaxSpeed,
};
inline bool RunTestForFlag(const TestInfo& Test, OptimizationFlags flag, std::ostream& LogStream, std::ostream& ErrStream, TestMode mode)
{
#define NoTry 0

#if UCodeLangDebug
	Compiler::CompilerPathData paths;
	Compiler Com;
	Com.Get_Settings()._Flags = flag;
	Com.Get_Settings().PtrSize = IntSizes::Native;

	Compiler::CompilerRet Com_r = NeverNullptr(&Com.Get_Errors());
	std::string InputFilesPath = UCodeLang_UCAppDir_Test_UCodeFiles + Test.InputFilesOrDir;
	std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + Test.TestName;
	std::filesystem::path p = OutFileDir;
	OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

	std::filesystem::create_directories(OutFileDir);

	if (mode == TestMode::CLang89BackEnd)
	{
		Com.Set_BackEnd(C11Backend::MakeObject);
	}
	else if (mode == TestMode::WasmBackEnd)
	{
		Com.Set_BackEnd(WasmBackEnd::MakeObject);
	}
	std::string OutFilePath = OutFileDir + Test.TestName + ModeType(flag) + ".ulibtest" + Com.GetOutputExtWithDot();

	paths.FileDir = InputFilesPath;
	paths.OutFile = OutFilePath;

#if !NoTry
	try
	{
#endif
		if (std::filesystem::is_directory(paths.FileDir))
		{
			Com_r = Com.CompileFiles(paths);
		}
		else
		{
			Com_r = Com.CompilePathToObj(paths.FileDir, paths.OutFile);
		}

#if !NoTry
	}
	catch (const std::exception &ex)
	{
		ErrStream << "fail from Compile [exception] '" << ex.what() << "' : "
				  << "'" << Test.TestName << "'" << std::endl;
		return false;
	}
#endif

	if (Test.Condition == SuccessCondition::Compilation || Test.Condition == SuccessCondition::CompilationFail)
	{
		if (
			(Com_r.IsValue() && Test.Condition == SuccessCondition::Compilation) ||
			(Com_r.IsError() && Test.Condition == SuccessCondition::CompilationFail))
		{
			LogStream << "Success from test '" << Test.TestName << ModeType(flag) << "'" << std::endl;
			return true;
		}
		else
		{
			ErrStream << "fail from test '" << Test.TestName << "'" << std::endl;

			LogErrors(ErrStream, Com);
			return false;
		}
	}

	if ( (Test.Condition == SuccessCondition::Panic||Test.Condition == SuccessCondition::PanicUnitCheck) && mode !=TestMode::UCodeLangBackEnd)
	{
		LogStream << "Test Skiped because it trys to panic";
		return true;
	}

	if (Com_r.IfError())
	{
		ErrStream << "fail from test [Cant Compile File/Files] '" << Test.TestName << ModeType(flag) << "'" << std::endl;

		LogErrors(ErrStream, Com);
		return false;
	}

	if (mode == TestMode::UCodeLangBackEnd)
	{
		RunTimeLangState state;
		UClib lib;
		if (!UClib::FromFile(&lib, OutFilePath))
		{

			ErrStream << "fail from test [Cant Open ULib File] '" << Test.TestName << ModeType(flag) << "'" << std::endl;
			return false;
		}

		//{
		auto Text = UAssembly::UAssembly::ToString(&lib);
		String Path = OutFilePath + ".UA";
		std::ofstream out(Path);
		if (out.is_open())
		{
			out << Text;
			out.close();
		}
		//}
		RunTimeLib rLib;
		rLib.Init(&lib);
		state.AddLib(&rLib);
		state.LinkLibs();

		{
			Interpreter RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;
#if !NoTry
			try
			{
#endif
				RunTime.Call(StaticVariablesInitializeFunc);

				RunTime.Call(ThreadVariablesInitializeFunc);

				r = RunTime.Call(Test.FuncToCall);

				RunTime.Call(ThreadVariablesUnLoadFunc);

				RunTime.Call(StaticVariablesUnLoadFunc);
#if !NoTry
			}
			catch (const std::exception &ex)
			{
				ErrStream << "fail from test [exception] '" << ex.what() << "' : "
						  << "'" << Test.TestName << "'" << ModeType(flag) << std::endl;
				return false;
			}
#endif

			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Interpreter"))
				{
					return false;
				}
			}
			else if (Test.Condition == SuccessCondition::Panic || Test.Condition == SuccessCondition::PanicUnitCheck)
			{
				String_view PanicStr = *(String_view*)Test.RunTimeSuccess.get();

				auto error = RunTime.CheckForIntperpreterError().value();

				if (PanicStr == error.ErrorType.Get<UCodeLang::PanicCalled>().PanicMsg)
				{
					bool ok = true;
					if (Test.Condition == SuccessCondition::PanicUnitCheck)
					{
						ok = RunTime.RCall<bool>("UnitCheck");
						if (!ok)
						{
							ErrStream << "fail from UnitCheck '" << std::endl;
						}
					}
					return ok;
				}


				return false;
			}
			RunTime.UnLoad();
		}

		{
			Jit_Interpreter RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;
			
#if !NoTry
			try
			{
#endif
				RunTime.Call(StaticVariablesInitializeFunc);

				RunTime.Call(ThreadVariablesInitializeFunc);

				r = RunTime.Call(Test.FuncToCall);

				RunTime.Call(ThreadVariablesUnLoadFunc);

				RunTime.Call(StaticVariablesUnLoadFunc);
#if !NoTry
			}

			catch (const std::exception& ex)
			{
				RunTime.UnLoad();
				ErrStream << "fail from jit test [exception] '" << ex.what() << "' : "
					<< "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}
#endif

			UCodeLangDefer(RunTime.UnLoad());
			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Jit_Interpreter"))
				{
					return false;
				}
			}
			else if (Test.Condition == SuccessCondition::Panic || Test.Condition == SuccessCondition::PanicUnitCheck)
			{
				String_view PanicStr = *(String_view*)Test.RunTimeSuccess.get();

				auto error = RunTime.CheckForIntperpreterError().value();

				if (PanicStr == error.ErrorType.Get<UCodeLang::PanicCalled>().PanicMsg)
				{
					bool ok = true;
					if (Test.Condition == SuccessCondition::PanicUnitCheck)
					{
						ok = RunTime.RCall<bool>("UnitCheck");
						if (!ok)
						{
							ErrStream << "fail from UnitCheck '" << std::endl;
						}
					}
					return ok;
				}


				return false;
			}
		}

		{
			UCodeRunTime RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;

#if !NoTry
			try
			{
#endif
				RunTime.Call(StaticVariablesInitializeFunc);
				RunTime.Call(ThreadVariablesInitializeFunc);

				r = RunTime.Call(Test.FuncToCall);

				RunTime.Call(ThreadVariablesUnLoadFunc);
				RunTime.Call(StaticVariablesUnLoadFunc);
#if !NoTry
			}
			catch (const std::exception& ex)
			{
				RunTime.UnLoad();
				ErrStream << "fail from UCodeRunTime test [exception] '" << ex.what() << "' : "
					<< "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}
#endif
			UCodeLangDefer(RunTime.UnLoad());

			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "UCodeRunTime"))
				{
					return false;
				}
			}
			else if (Test.Condition == SuccessCondition::Panic || Test.Condition == SuccessCondition::PanicUnitCheck)
			{
				String_view PanicStr = *(String_view*)Test.RunTimeSuccess.get();

				auto error = RunTime.CheckForIntperpreterError().value();

				if (PanicStr == error.ErrorType.Get<UCodeLang::PanicCalled>().PanicMsg)
				{
					bool ok = true;
					if (Test.Condition == SuccessCondition::PanicUnitCheck)
					{
						ok = RunTime.RCall<bool>("UnitCheck");
						if (!ok)
						{
							ErrStream << "fail from UnitCheck '" << std::endl;
						}
					}
					return ok;
				}


				return false;
			}
		}
		rLib.UnLoad();
		return true;
	}
	else if (mode == TestMode::CLang89BackEnd)
	{
		UClib &ulib = *Com_r.GetValue().OutPut;

		auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
		UCodeLangAssert(ufunc);
		{
			// String filetxt = Compiler::GetTextFromFile(OutFilePath);

			Path dllfile = OutFilePath + ".lib";
			Path Testablefile = OutFilePath;
			UCodeLangAssert(CompileC89ToLib(Testablefile, dllfile));

			auto &Assembly = ulib.Get_Assembly();
			auto cfuncname = C11Backend::UpdateToCindentifier(ufunc->DecorationName);

			auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
			auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

			auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
			auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

#if UCodeLang_Platform_Windows
			auto lib = LoadLibrary(dllfile.c_str());
			UCodeLangDefer(FreeLibrary(lib));

			auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
			auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
			auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
			auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());

			auto functocall = GetProcAddress(lib, cfuncname.c_str());
#elif UCodeLang_Platform_Posix
			auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
			UCodeLangDefer(dlclose(lib));
			auto staticinittocall = dlsym(lib, staticinitname.c_str());
			auto threadinittocall = dlsym(lib, threadinitname.c_str());
			auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
			auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());

			auto functocall = dlsym(lib, cfuncname.c_str());
#endif

			UCodeLangAssert(functocall);

			bool hasautocall = cfuncname == "main";
			if (!hasautocall)
			{
				using Func = void (*)();
				((Func)staticinittocall)();
				((Func)threadinittocall)();
			}

			auto rettype = ufunc->RetType;
			if (auto val = Assembly.Find_Node(rettype))
			{
				if (val->Get_Type() == ClassType::Enum)
				{
					auto &Enum = val->Get_EnumData();
					rettype = Enum.BaseType;
				}
			}

			auto RetValue = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
			{
				if (rettype._Type == ReflectionTypes::Bool)
				{
					using GetValueFunc = bool (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::sInt8 || rettype._Type == ReflectionTypes::uInt8 || rettype._Type == ReflectionTypes::Char)
				{
					using GetValueFunc = UInt8 (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::uInt16 || rettype._Type == ReflectionTypes::sInt16)
				{
					using GetValueFunc = Int16 (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::uInt32 || rettype._Type == ReflectionTypes::sInt32)
				{
					using GetValueFunc = Int32 (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::float32)
				{
					using GetValueFunc = float32 (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::float64)
				{
					using GetValueFunc = float64 (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (rettype._Type == ReflectionTypes::uIntPtr || rettype._Type == ReflectionTypes::sIntPtr)
				{
					using GetValueFunc = uintptr_t (*)();
					auto val = ((GetValueFunc)functocall)();

					UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
					memcpy(RetValue.get(), &val, sizeof(val));
				}
				else if (auto typenod = Assembly.Find_Node(rettype))
				{
					if (StringHelper::StartWith(typenod->FullName, "Vec2") || StringHelper::StartWith(typenod->FullName, "vec2"))
					{
						// using GetValueFunc = Vec2(*)();//I have no idea why this does not work
						// auto val = ((GetValueFunc)functocall)();

						using GetValueFunc2 = Int64 (*)();
						auto val2 = ((GetValueFunc2)functocall)();

						Vec2 &val = *(Vec2 *)&val2;

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else
					{
						UCodeLangUnreachable();
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}

			if (!hasautocall)
			{
				using Func = void (*)();
				((Func)staticdeinittocall)();
				((Func)threaddeinittocall)();
			}
		}
	}
	else if (mode == TestMode::WasmBackEnd)
	{
		UClib &ulib = *Com_r.GetValue().OutPut;
		auto &OutFile = Com_r.GetValue().OutFile.value();

		auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
		UCodeLangAssert(ufunc);

		String JsString = "const wasm = new Uint8Array([";

		std::stringstream ss;
		ss << "const wasm = new Uint8Array([";
		for (const auto &b : OutFile)
		{
			ss << "0x" << std::hex << static_cast<int>(b) << ", ";
		}
		ss << "]);\n";
		ss << "const m = new WebAssembly.Module(wasm);\n";
		ss << "const instance = new WebAssembly.Instance(m, {});\n";
		ss << "console.log(instance.exports.";
		ss << WasmBackEnd::ToWebName(ufunc->DecorationName);
		ss << "());";

		Path node_file = paths.OutFile.native() + Path("test.js").native();
		Path out_file = paths.OutFile.native() + Path("test.js.out").native();

		std::ofstream nf(node_file);
		nf << ss.str();
		nf << std::flush;

		String expected;
		auto rettype = ufunc->RetType;

		if (rettype._Type == ReflectionTypes::sInt32)
		{
			expected += std::to_string(*(int *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::sInt16)
		{
			expected += std::to_string(*(Int16 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::sInt8)
		{
			expected += std::to_string(*(Int8 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::uInt16)
		{
			expected += std::to_string(*(UInt16 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::uInt8)
		{
			expected += std::to_string(*(UInt8 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::Char)
		{
			expected += std::to_string(*(Int8 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::float32)
		{
			expected += std::to_string(*(float32 *)Test.RunTimeSuccess.get());
		}
		else if (rettype._Type == ReflectionTypes::float64)
		{
			expected += std::to_string(*(float64 *)Test.RunTimeSuccess.get());
		}
		else
		{
			UCodeLangUnreachable();
		}

		expected += '\n';

		{
			std::system(("node " + node_file.generic_string() + " > " + out_file.generic_string()).c_str());
		}

		std::stringstream ss_out;
		ss_out << std::ifstream(out_file).rdbuf();
		auto outstr = ss_out.str();

		{
			// C++ adds trailing zeros but node.js does not
			if (rettype._Type == ReflectionTypes::float32)
			{
				float newfloat = std::stof(outstr.substr(0, outstr.size() - 1)); //-1 to remove /n
				outstr = std::to_string(newfloat);
				outstr += '\n';
			}
			else if (rettype._Type == ReflectionTypes::float64)
			{
				float64 newfloat = std::stof(outstr.substr(0, outstr.size() - 1)); //-1 to remove /n
				outstr = std::to_string(newfloat);
				outstr += '\n';
			}
		}
		if (outstr != expected)
		{

			std::cerr << "got: " << ss_out.str();
			std::cerr << "expected: " << expected;
			return false;
		}
	}
	else
	{
		UCodeLangUnreachable();
	}

	return true;
#endif
}

inline std::mutex Coutlock;

inline bool RunTest(const TestInfo &Test, TestMode mode)
{
	bool V = true;

	std::stringstream Log;
	std::stringstream Err;
	Log << "Runing Test '" << Test.TestName << "'" << std::endl;

	if (Test.Condition == SuccessCondition::RunTimeValue)
	{

		for (auto Flag : OptimizationFlagsToCheck)
		{
			if (!RunTestForFlag(Test, Flag, Log, Err, mode))
			{
				V = false;
				break;
			}
		}
	}
	else
	{
		if (!RunTestForFlag(Test, OptimizationFlagsToCheck[0], Log, Err, mode))
		{
			V = false;
		}
	}
	Coutlock.lock();

	std::cout << Log.str();
	std::cout << Err.str();

	Coutlock.unlock();

	return V;
}
bool RunTest(const TestInfo& Test, TestMode mode);

bool RunTest(const ModuleTest& Test, TestMode mode);

int RunTests(bool MultThread = true);

inline bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler)
{
	out << "[\n";
	auto &Errors = _Compiler.Get_Errors().Get_Errors();
	for (auto &Item : Errors)
	{
		out << Item.ToString() << std::endl;
		out << '\n';
	}
	out << "]\n";
	return _Compiler.Get_Errors().Has_Errors();
}

inline bool CompileC89ToLib(const Path& Cfile, const Path& Outdllfile)
{
	String Cmd = "gcc " + Cfile.generic_string();
	Cmd += " -shared -std=c89 -g";
#if UCodeLang_32BitSytem
	Cmd += " -m32";
#endif
	Cmd += " -fPIC";
	Cmd += " -o " + Outdllfile.generic_string();
	return system(Cmd.c_str()) == EXIT_SUCCESS;
}

class TestRuner
{
public:
	using TestInfo = UCodeLang::TestRuner::TestInfo;
	using TestsResult = UCodeLang::TestRuner::TestsResult;
	using OnTestDone = UCodeLang::TestRuner::OnTestDone;
	inline TestsResult RunTests(UClib& lib, const Path& outfilepath, TestMode Type, Optional<OnTestDone> OnDone = {})
	{
		switch (Type)
		{
		case TestMode::UCodeLangBackEnd:
		{
			UCodeLang::TestRuner runer;
			return runer.RunTests(lib, UCodeLang::TestRuner::InterpreterType::Interpreter, OnDone);
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

				auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
				auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

				auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
				auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

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

					auto cfuncname = C11Backend::UpdateToCindentifier(TestFunc->DecorationName);

#if UCodeLang_Platform_Windows
					auto functocall = GetProcAddress(lib, cfuncname.c_str());
#elif UCodeLang_Platform_Posix
					auto functocall = dlsym(lib, cfuncname.c_str());
#endif

					UCodeLangAssert(functocall);

					{
						using Func = void (*)();
						((Func)staticinittocall)();
						((Func)threadinittocall)();
					}

					{
						using Func = bool (*)();
						passed = ((Func)functocall)();
					}

					{
						using Func = void (*)();
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