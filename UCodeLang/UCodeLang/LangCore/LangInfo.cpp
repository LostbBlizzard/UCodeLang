#include "LangInfo.hpp"

#include <iostream>
#include <filesystem>


#if UCodeLang_Platform_Windows
#include <windows.h>
#include <shlobj.h>
#elif UCodeLang_Platform_Linux
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif


UCodeLangStart



Path LangInfo::GetUCodeGlobalDirectory()
{
    #if UCodeLang_Platform_Windows
    WCHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPathW(0, CSIDL_PROFILE, 0, 0,my_documents);

    if (SUCCEEDED(result)) 
    {
        Path DocPath = my_documents;
    
        DocPath /= ".ucodelang";

        if (!std::filesystem::exists(DocPath)) {
            std::filesystem::create_directory(DocPath);
        }

        return DocPath;
    }
    else
    {
        return "";
    }
    #elif UCodeLang_Platform_Linux
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;
    Path DocPath = homedir;
    DocPath /= ".ucodelang";
    if (!std::filesystem::exists(DocPath)) {
            std::filesystem::create_directory(DocPath);
    }
    return DocPath;
    #elif UCodeLang_Platform_MacOS
    UCodeLangToDo();
    #endif
}
Path LangInfo::GetUCodeGlobalModulesDownloads()
{
 auto Path = GetUCodeGlobalDirectory() / "Modules";  

 if (!std::filesystem::exists(Path)) {
     std::filesystem::create_directory(Path);
 }
 return Path;
}
Path LangInfo::GetUCodeGlobalBin()
{
    auto Path = GetUCodeGlobalDirectory() / "bin";
    if (!std::filesystem::exists(Path)) {
        std::filesystem::create_directory(Path);
    }
    return Path;
}
Path LangInfo::GetUCodeGlobalCacheDirectory()
{
    auto Path = GetUCodeGlobalDirectory() / "cashe";
    if (!std::filesystem::exists(Path)) {
        std::filesystem::create_directory(Path);
    }
    return Path;
}
UCodeLangEnd