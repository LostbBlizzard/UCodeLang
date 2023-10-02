#pragma once
#include <UCodeLang/LangCore/LangTypes.hpp>

#if UCodeLangDebug
inline const UCodeLang::String UCodeLang_SoultionDir = "../";

inline const UCodeLang::String UCodeLang_UCAppDir = UCodeLang_SoultionDir + "UCApp/";
inline const UCodeLang::String UCodeLang_UCAppDir_ScrDir = UCodeLang_UCAppDir + "src/";
inline const UCodeLang::String UCodeLang_UCAppDir_TestDir = UCodeLang_UCAppDir + "tests/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_UCodeFiles = UCodeLang_UCAppDir_TestDir + "UCodeFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_OutputFiles = UCodeLang_UCAppDir_TestDir + "UCodeFiles/Output/";

inline const UCodeLang::String UCodeLang_UCAppDir_Test_LanguageSeverFilesFiles = UCodeLang_UCAppDir_TestDir + "LanguageSeverFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_LanguageSeverFilesOut = UCodeLang_UCAppDir_TestDir + "LanguageSeverFiles/Output/";

inline const UCodeLang::String UCodeLang_UCAppDir_Test_CppHelperFiles = UCodeLang_UCAppDir_TestDir + "CppHelperFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_CppHelperOut = UCodeLang_UCAppDir_TestDir + "CppHelperFiles/Output/";

inline const UCodeLang::String  UCodeLang_UCAppDir_TestPerformanceTests = UCodeLang_UCAppDir_TestDir + "PerformanceTests/";
inline const UCodeLang::String  UCodeLang_UCAppDir_TestPerformanceTestsOut = UCodeLang_UCAppDir_TestDir + "PerformanceTests/Output/";
#endif // DEBUG