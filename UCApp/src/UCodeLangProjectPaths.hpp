#pragma once
#include <UCodeLang/LangCore/LangTypes.hpp>

#ifdef DEBUG
inline const UCodeLang::String UCodeLang_SoultionDir = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/";

inline const UCodeLang::String UCodeLang_UCAppDir = UCodeLang_SoultionDir + "UCApp/";
inline const UCodeLang::String UCodeLang_UCAppDir_ScrDir = UCodeLang_UCAppDir + "src/";
inline const UCodeLang::String UCodeLang_UCAppDir_TestDir = UCodeLang_UCAppDir + "tests/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_UCodeFiles = UCodeLang_UCAppDir_TestDir + "UCodeFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_OutputFiles = UCodeLang_UCAppDir_ScrDir + "UCodeFiles/Output/";

inline const UCodeLang::String UCodeLang_UCAppDir_Test_LanguageSeverFilesFiles = UCodeLang_UCAppDir_TestDir + "LanguageSeverFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_LanguageSeverFilesOut = UCodeLang_UCAppDir_ScrDir + "LanguageSeverFiles/Output/";
#endif // DEBUG