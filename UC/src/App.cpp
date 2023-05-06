#include "App.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>
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

	//compiler
	ModuleIndex _ModuleIndex;
	Compiler::CompilerPathData _CompilerPaths;
	Compiler _Compiler;
	
	//runtime
	RunTimeLangState _RunTimeState;
	UCodeRunTime _RunTime;
};
static AppInfo _This;

#include <windows.h>

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
int App::main(int argc, char* argv[])
{
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

	

	return EXIT_SUCCESS;
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

	if (Word1 == "Build") 
	{ 
		auto _Path = GetPath(Line);
		auto _PathAsPath = Path(_Path);

		auto _PathVar2 = GetPath(Line);
		//
		_This._CompilerPaths.FileDir = _Path;
		
		Path Backpath = _PathAsPath;
		_This._CompilerPaths.OutFile = Path(Backpath.native() + Path("out").native()).generic_string();
		_This._CompilerPaths.IntDir  = Path(Backpath.native() + Path("int").native()).generic_string();
		//
		_This._Compiler.Get_Errors().Remove_Errors();

		if (fs::is_directory(_PathAsPath))
		{
			_This._Compiler.CompileFiles(_This._CompilerPaths);
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
				if (module.BuildModule(_This._Compiler,_This._ModuleIndex).CompilerRet._State ==Compiler::CompilerState::Fail)
				{
					*_This.output << "Compiler Fail:\n";
					*_This.output << _This._Compiler.Get_Errors().ToString();
				}
				else
				{
					*_This.output << "Compiler Success:\n";
					*_This.output << "output is in " + _This._CompilerPaths.OutFile;
				}
			}
			else
			{
				if (_PathVar2.size())
				{
					_This._CompilerPaths.OutFile = Path(Backpath.native() + Path(_PathVar2).native()).generic_string();
				}

				_This._Compiler.CompilePathToObj(_This._CompilerPaths.FileDir, _This._CompilerPaths.OutFile);
			}
		}
	}
	else if (Word1 == "make")
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
	else if (Word1 == "uc_vm")
	{

	}
	else if (Word1 == "parsefile")
	{
		std::ifstream file(GetPath(Line));
		if (file.is_open())
		{

			for (std::string line; getline(file, line); )
			{
				ParseLine(line);
			}

			file.close();
		}
	}
	else if (Word1 == "set_input_io")
	{
		
	}
	else if (Word1 == "close")
	{
		_This.EndApp = true;
	}
	else
	{
		TokenCheck(Word1);
	}
}

void TokenCheck(const UCodeLang::String_view& Word1)
{
	*_This.output << "bad word '" << Word1 << "'" << std::endl;
}
