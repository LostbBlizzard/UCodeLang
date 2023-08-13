#include "App.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "UCodeLang/Compliation/ModuleFile.hpp"
using namespace UCodeLang;


struct AppInfo
{
	bool EndApp = false;
	//in/out
	std::ostream* output = nullptr;
	std::istream* Input = nullptr;

	std::ofstream OutputFile;
	std::ifstream InputFile;


	int ExeRet = EXIT_SUCCESS;

	//compiler
	ModuleIndex _ModuleIndex;
	Compiler::CompilerPathData _CompilerPaths;
	Compiler _Compiler;
	
	//runtime
	RunTimeLangState _RunTimeState;
	UCodeRunTime _RunTime;
};
static AppInfo _This;
#if UCodeLang_Platform_Windows
#include <windows.h>
bool SetPermanentEnvironmentVariable(LPCTSTR value, LPCTSTR data)
{
	HKEY hKey;
	LPCTSTR keyPath = TEXT("System\\CurrentControlSet\\Control\\Session Manager\\Environment");
	LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
	if (lOpenStatus == ERROR_SUCCESS)
	{
		LSTATUS lSetStatus = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)data, wcslen(data) + 1);
		RegCloseKey(hKey);

		if (lSetStatus == ERROR_SUCCESS)
		{
			SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
			return true;
		}
	}

	return false;
}
VOID startup(LPCTSTR lpApplicationName)
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(lpApplicationName,   // the path
	nullptr,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	while (true)
	{

	}
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
#endif
int App::main(int argc, char* argv[])
{
	namespace fs = std::filesystem;
	

	

	#if UCodeLang_Platform_Windows
	
	#if UCodeLangDebug
	auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalBin();
	auto Ulangexepath = ucodebinpath / Path("ucodelang.exe");
	Path ThisRuningExePath = argv[0];
	bool ShouldCopy = true;
	if (fs::exists(Ulangexepath))
	{
		ShouldCopy = fs::file_size(Ulangexepath) != fs::file_size(ThisRuningExePath);
		if (ShouldCopy)
		{
			fs::remove(Ulangexepath);
		}
	}

	if (ShouldCopy && (ThisRuningExePath != Ulangexepath))
	{


		fs::copy_file(ThisRuningExePath, Ulangexepath);
	}
	#endif // DEBUG
	#endif

	//while (true)
	{

	}//break point here

	//SetPermanentEnvironmentVariable(R"PATH");
	//SetPermanentEnvironmentVariable(,)
	
		
	_This.output = &std::cout;
	_This.Input = &std::cin;

	for (size_t i = 0; i < argc; i++)
	{
		ParseLine(String_view(argv[i]));
	}


	
	while (_This.EndApp == false)
	{
		std::string line;
		std::getline(*_This.Input, line);

		ParseLine(line);
	}

	

	return _This.ExeRet;
}



template<typename T>
bool InList(const T* buffer,size_t buffersize,T Value)
{
	for (size_t i = 0; i < buffersize; i++)
	{
		if (buffer[i] == Value) { return true; }
	}
	return false;
}

bool IsList(const char* List,char V)
{
	return InList(List, strlen(List), V);
}

bool IsList(String_view List, char V)
{
	return InList(List.data(), List.size(), V);
}

#define Letersdef "QWERTYUIOPASDFGHJKLZXCVBNM" "qwertyuiopasdfghjklzxcvbnm"
#define Numdef "1234567890"
#define NameChardef "_"

#define Worddef Letersdef Numdef NameChardef

#define PathDef Letersdef Numdef ":/\\.+()"

String_view GetWord_t(String_view& Line,String_view GoodChars_t)
{
	bool Reading = false;
	for (size_t i = 0; i < Line.size(); i++)
	{
		char V = Line[i];
		bool GoodChar = IsList(GoodChars_t, V);
		if (!GoodChar && Reading == true)
		{
			auto r = String_view(Line.data(), i);
			Line = Line.substr(i+1);
			return r;
		}
		else
		{
			if (GoodChar)
			{
				Reading = true;
			}
		}
	}
	auto r2 = String_view(Line.data(), Line.size());
	Line = Line.substr(Line.size());
	return r2;
}
String_view GetWord(String_view& Line)
{
	return GetWord_t(Line, Worddef);
}

String_view GetPath(String_view& Line)
{
	return GetWord_t(Line, PathDef);
}

void ParseLine(String_view Line)
{
	namespace fs = std::filesystem;
	String_view Word1 = GetWord(Line);

	if (Word1 == "build")
	{
		auto _Path = GetPath(Line);
		auto _PathAsPath = Path(_Path);

		auto _PathVar2 = GetPath(Line);
		//
		_This._CompilerPaths.FileDir = _Path;

		Path Backpath = _PathAsPath;
		_This._CompilerPaths.OutFile = Path(Backpath.native() + Path("out").native()).generic_string();
		_This._CompilerPaths.IntDir = Path(Backpath.native() + Path("int").native()).generic_string();
		//
		_This._Compiler.Get_Errors().Remove_Errors();

		bool ItWorked = false;
		if (fs::is_directory(_PathAsPath))
		{
			ItWorked = _This._Compiler.CompileFiles(_This._CompilerPaths)._State == Compiler::CompilerState::Success;
		}
		else
		{
			auto _PathExt = _PathAsPath.extension();
			if (_PathExt == UCodeLang::ModuleFile::FileExtWithDot)
			{
				UCodeLang::ModuleFile module;
				if (!UCodeLang::ModuleFile::FromFile(&module, _PathAsPath))
				{
					*_This.output << "Cant Open module file\n";
					return;
				}
				ItWorked = module.BuildModule(_This._Compiler, _This._ModuleIndex).CompilerRet._State == Compiler::CompilerState::Fail;
			}
			else
			{
				if (_PathVar2.size())
				{
					_This._CompilerPaths.OutFile = Path(Backpath.native() + Path(_PathVar2).native()).generic_string();
				}

				ItWorked = _This._Compiler.CompilePathToObj(_This._CompilerPaths.FileDir, _This._CompilerPaths.OutFile)._State == Compiler::CompilerState::Success;
			}
		}

		if (!ItWorked)
		{
			*_This.output << "Compiler Fail:\n";
			*_This.output << _This._Compiler.Get_Errors().ToString();
			_This.ExeRet = EXIT_FAILURE;
		}
		else
		{
			*_This.output << "Compiler Success:\n";
			*_This.output << "output is in " + _This._CompilerPaths.OutFile;
			_This.ExeRet = EXIT_SUCCESS;
		}
	}
	else if (Word1 == "--build")
	{
		*_This.output << "use \"build\" and not \"--build\"\n";
	}
	else if (Word1 == "new" )
	{
		
		std::string TepNameBuffer;
		std::string TepOwnerBuffer;


		auto _Path = GetPath(Line);
		auto Name = GetPath(Line);

		auto OwnerName = GetPath(Line);

		if (Name.size() == 0)
		{
			*_This.output << "Enter Module Name:";

			std::getline(*_This.Input, TepNameBuffer);
			Name = TepNameBuffer;
		}

		if (OwnerName.size() == 0)
		{
			*_This.output << "Enter Owner/Author Name:";

			std::getline(*_This.Input, TepOwnerBuffer);
			Name = TepOwnerBuffer;
		}

		Path NewDir = Path(_Path).native() + Path(Name).native();
		Path modePath = NewDir.native() + Path(Name).native() + Path(ModuleFile::FileExtWithDot).native();

		fs::create_directories(NewDir);

		UCodeLang::ModuleFile module;
		module.NewInit((String)Name, (String)OwnerName);
		UCodeLang::ModuleFile::ToFile(&module, modePath);
	}
	else if (Word1 == "--new")
	{
		*_This.output << "use \"new\" and not \"--new\"\n";
	}
	else if (Word1 == "run")
	{

	}
	else if (Word1 == "--run")
	{
		*_This.output << "use \"run\" and not \"--run\"\n";
	}
	else if (Word1 == "runlines")
	{
		std::ifstream file(Path(GetPath(Line)));
		if (file.is_open())
		{

			for (std::string line; getline(file, line); )
			{
				ParseLine(line);
			}

			file.close();
		}
	}
	else if (Word1 == "out")
	{
		
	}
	else if (Word1 == "--help")
	{
		*_This.output << "use \"help\" and not \"--help\"\n";
	}
	else if (Word1 == "help")
	{
		*_This.output << "put help info here.\n";
	}
	else if (Word1 == "close")
	{
		_This.EndApp = true;
	}
	else
	{
		*_This.output << "bad command use the \"help\" command for help\n";
		TokenCheck(Word1);
	}
}

void TokenCheck(const UCodeLang::String_view& Word1)
{
	*_This.output << "bad Token '" << Word1 << "'" << std::endl;
}
