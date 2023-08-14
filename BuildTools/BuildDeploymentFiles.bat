call BuildWin32Published.bat

call BuildWin64Published.bat

xcopy /y ..\Output\UCodelangCL\Win32\Published\ucodelang.exe ..\Output\Deployment\Runables\Win32\ucodelang.exe*
xcopy /y ..\Output\UCodeLanguageSever\Win32\Published\UCodeLanguageSever.exe ..\Output\Deployment\Runables\Win32\UCodeLanguageSever.exe*
xcopy /y ..\Output\UCodeIDE\Win32\Published\UCodeIDE.exe ..\Output\Deployment\Runables\Win32\UCodeIDE.exe*
xcopy /y ..\Output\UCodeLang\Win32\Published\UCodeLang.lib ..\Output\Deployment\Precompiled\Win32\UCodeLang.lib*

xcopy /y ..\Output\UCodelangCL\Win64\Published\ucodelang.exe ..\Output\Deployment\Runables\Win64\ucodelang.exe*
xcopy /y ..\Output\UCodeLanguageSever\Win64\Published\UCodeLanguageSever.exe ..\Output\Deployment\Runables\Win64\UCodeLanguageSever.exe*
xcopy /y ..\Output\UCodeIDE\Win64\Published\UCodeLanguageSever.exe ..\Output\Deployment\Runables\Win64\UCodeIDE.exe*
xcopy /y ..\Output\UCodeLang\Win64\Published\UCodeLang.lib ..\Output\Deployment\Precompiled\Win64\UCodeLang.lib*

xcopy /y /s/e ..\UCodeAPI\StandardLibrary\ ..\Output\Deployment\ULangSource\StandardLibrary\
xcopy /y /s/e ..\UCodeAPI\NStandardLibrary\ ..\Output\Deployment\ULangSource\NStandardLibrary\
xcopy /y /s/e ..\UCodeAPI\NWin32\ ..\Output\Deployment\ULangSource\NWin32\
xcopy /y /s/e ..\UCodeAPI\Win32\ ..\Output\Deployment\ULangSource\Win32\

cd ..\UCodeDocumentation
call Build.bat
xcopy /y /s/e ..\Output\UCodeDocumentation\ ..\Output\Deployment\Documentation\
cd ..\BuildTools
