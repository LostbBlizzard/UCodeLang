
msbuild ../UCodeLang.sln /t:Build /p:Configuration=Published /p:Platform=Win32 -maxcpucount

msbuild ../UCodeLang.sln /t:Build /p:Configuration=Published /p:Platform=Win64 -maxcpucount