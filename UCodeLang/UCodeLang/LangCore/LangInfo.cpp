#include "LangInfo.hpp"



#include <windows.h>
#include <iostream>
#include <shlobj.h>

#include <filesystem>
UCodeLangStart



Path LangInfo::GetUCodeGlobalDirectory()
{
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