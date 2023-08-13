#include "CppHelperTests.hpp"
#include <filesystem>

UCodeTestStart

bool CppHelperTests::RunTests()
{
	
	return true;
	
	#if UCodeLangDebug
	return UCodeAnalyzer::CppHelper::ParseCppfileAndOutULang(
		UCodeLang_UCAppDir_Test_CppHelperFiles + "Func.hpp",
		UCodeLang_UCAppDir_Test_CppHelperOut + "FuncLink.cpp",
		UCodeLang_UCAppDir_Test_CppHelperOut + "Func.uc");
	#endif
}
UCodeTestEnd
