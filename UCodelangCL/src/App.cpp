#include "App.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "UCodeLang/Compilation/ModuleFile.hpp"
#include "UCodeAnalyzer/Formater.hpp"
#include "UCodeAnalyzer/Preprocessors/CppHelper.hpp"
#include "UCodeLang/RunTime/AnyInterpreter.hpp"
#include "UCodeLang/Compilation/Back/NativeBackEnd.hpp"
#include "UCodeLang/Compilation/UAssembly/UAssembly.hpp"

#include "UCodeLang/Compilation/Back/Windows/WindowsBackEnd.hpp"
#include "UCodeLang/Compilation/Back/Linux/LinuxBackEnd.hpp"
#include "UCodeLang/Compilation/Back/MacOs/MacOSBackEnd.hpp"
#include "UCodeLang/Compilation/Back/IR/IRBackEnd.hpp"
#include "UCodeLang/Compilation/Back/WebAssembly/WasmBackEnd.hpp"
#include "UCodeLang/Compilation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compilation/Back/LLVM/LLVMBackEnd.hpp"

#include "UCodeLang/Compilation/Back/x86_64/X86_64UNativeBackEnd.hpp"

#include "UCodeLang/RunTime/TestRuner.hpp"
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
};
static AppInfo _This;
#if UCodeLang_Platform_Windows
#include <windows.h>
#endif
int App::main(int argc, char* argv[])
{
	namespace fs = std::filesystem;
	//while (true);


	bool IsDebuging = false;
	#if UCodeLang_Platform_Windows
	IsDebuging = IsDebuggerPresent();
	#endif

	#if UCodeLang_Platform_Windows
	#if UCodeLangDebug
	if (IsDebuging) 
	{
		auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalBin();
		auto Ulangexepath = ucodebinpath / Path("uclang.exe");
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
	}
	#endif // DEBUG
	#endif
	
	//break point here

	//SetPermanentEnvironmentVariable(R"PATH");
	//SetPermanentEnvironmentVariable(,)
	
		
	_This.output = &std::cout;
	_This.Input = &std::cin;
	

	bool nohasCmds = argc == 1;

	if (nohasCmds)
	{
		String_view Line = "help";
		ParseLine(Line);
		
		
		while (_This.EndApp == false)
		{
			std::string line;
			std::getline(*_This.Input, line);

			String_view Line = line;
			ParseLine(Line);
		}
	}
	else
	{
		String V;
		for (size_t i = 1; i < argc; i++)
		{
			V += String_view(argv[i]);
			V += " ";

		}
		//while (true);
		String_view Line =V;
		while (Line.size())
		{
			ParseLine(Line);

			bool AllSame = true;
			for (auto& Item : Line)
			{
				if (Item != ' ') {
					AllSame = false;
					break;
				}
			}
			if (AllSame) {break;}
		}
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

#define PathDef Letersdef Numdef ":/\\.+()-"

String_view GetWord_t(String_view& Line,String_view GoodChars_t)
{
	bool Reading = false;
	size_t goodindex = 0;
	for (size_t i = 0; i < Line.size(); i++)
	{
		char V = Line[i];
		bool GoodChar = IsList(GoodChars_t, V);
		if (!GoodChar && Reading == true)
		{
			auto r = String_view(&Line[goodindex],i- goodindex);
			Line = Line.substr(i);
			return r;
		}
		else
		{
			if (GoodChar)
			{
				if (Reading == false) {
					goodindex = i;
				}
				Reading = true;
			}
		}
	}

	if (Reading) {
		auto r2 = String_view(&Line[goodindex]);
		Line = Line.substr(Line.size());
		return r2;
	}
	else
	{
		auto r2 = String_view();
		return r2;
	}
}
String_view GetWord(String_view& Line)
{
	return GetWord_t(Line, Worddef);
}

String_view GetPath(String_view& Line)
{
	return GetWord_t(Line, PathDef);
}
#define AppPrint(x) *_This.output << x;
#define AppPrintin(x) *_This.output << x << std::endl;

void ParseLine(String_view& Line)
{
	namespace fs = std::filesystem;
	String_view Word1 = GetWord(Line);


	
	if (Word1 == "build")
	{
		ModuleIndex _ModuleIndex = ModuleIndex::GetModuleIndex();
		Compiler _Compiler;

		auto oldline = Line;
		String _Path = String(GetPath(Line));
		auto _PathAsPath = Path(_Path);
		if (_Path.size())
		{
			bool isflag = false;
			for (auto& Item : _Path)
			{
				if (Item == '-')
				{
					isflag = true;
					break;
				}
			}

			if (isflag) 
			{
				Line = oldline;
				_PathAsPath = std::filesystem::current_path();
				_Path = _PathAsPath.generic_string();
			}

		}
		else
		{
			_PathAsPath = std::filesystem::current_path();
			_Path = _PathAsPath.generic_string();
		}


	
		{
			bool use01 = UCodeLang::StringHelper::Contains(Line, "-01");
			bool use02 = UCodeLang::StringHelper::Contains(Line, "-02");
			bool use03 = UCodeLang::StringHelper::Contains(Line, "-03");
			bool usedebug = !(UCodeLang::StringHelper::Contains(Line, "-ndebug") || UCodeLang::StringHelper::Contains(Line, "-nd"));
			bool use32mode = UCodeLang::StringHelper::Contains(Line, "-32");
			bool use64mode = UCodeLang::StringHelper::Contains(Line, "-64");
			{
				auto& Settings = _Compiler.Get_Settings();

				if (use64mode) {
					Settings.PtrSize = IntSizes::Int64;
				}
				if (use32mode) {
					Settings.PtrSize = IntSizes::Int32;
				}
				if (usedebug) 
				{
					*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::Debug;
				}
				else
				{
					*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::Debug;
				}
				if (use01)
				{
					*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_1;
				}
				else
				{
					*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_1;
				}
				if (use02)
				{
					*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_2;
				}
				else
				{
					*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_2;
				}
				if (use03)
				{
					*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_3;
				}
				else
				{
					*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_3;
				}


				if (UCodeLang::StringHelper::Contains(Line, "-c89"))
				{
					_Compiler.Set_BackEnd(UCodeLang::C89Backend::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-llvm"))
				{
					_Compiler.Set_BackEnd(UCodeLang::LLVMBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-wasm"))
				{
					_Compiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-ir"))
				{
					_Compiler.Set_BackEnd(UCodeLang::IRBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-uvm:this"))
				{
					#ifdef UCodeLang_HasNoNativeULangBackEnd
					_This.ExeRet = EXIT_FAILURE;
					AppPrintin("There is no Native-UCodeVM backend for this CPU");
					#else
					_Compiler.Set_BackEnd(UCodeLang::NativeULangBackEnd::MakeObject);
					#endif
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-uvm:x86_64"))
				{
					_Compiler.Set_BackEnd(UCodeLang::X86_64UNativeBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-uvm:x86"))
				{
					//_Compiler.Set_BackEnd(UCodeLang::x::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-uvm:arm_64"))
				{
					
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-uvm:arm"))
				{
					//_Compiler.Set_BackEnd(UCodeLang::x::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-native:this"))
				{
					#ifdef UCodeLang_HasNoPlatformBackEndBackEnd 
					_This.ExeRet = EXIT_FAILURE;
					AppPrintin("There is no Native-Platform backend for this Operating System");
					#else
					_Compiler.Set_BackEnd(UCodeLang::NativePlatformBackEnd::MakeObject);
					#endif
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-native:linux"))
				{
					_Compiler.Set_BackEnd(UCodeLang::LinuxBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-native:windows"))
				{
					_Compiler.Set_BackEnd(UCodeLang::WindowsBackEnd::MakeObject);
				}
				else if (UCodeLang::StringHelper::Contains(Line, "-native:mac"))
				{
					_Compiler.Set_BackEnd(UCodeLang::MacOSBackEnd::MakeObject);
				}
			}

			for (size_t i = 0; i < Line.size(); i++)
			{
				auto& V = Line[i];
				if (V == '-')
				{
					auto NewStr =Line.substr(i + 1);
					String_view arg = GetWord(NewStr);
					size_t offsettomove = (arg.data() - &V);
					offsettomove += arg.size();
					i += offsettomove;

					if (i < Line.size()) {
						auto V2 = Line[i];
						auto& Settings = _Compiler.Get_Settings();

						if (V2 == ':') {
							auto newstr =Line.substr(i + 1);
							String_view arg2 = GetWord(newstr);
							Settings.AddArgValue(String(arg), String(arg2));

							size_t offsettomove2 = (arg2.data() - &V);
							offsettomove2 += arg2.size();
							i += offsettomove2;

						}
						else
						{
							Settings.AddArgFlag(String(arg));
						}
					}
				}
			}
		}
		if (!buildfile(_PathAsPath, _Compiler))
		{
			_This.ExeRet = EXIT_FAILURE;
		}
		else
		{
			AppPrintin(_Compiler.Get_Errors().ToString());
			_This.ExeRet = EXIT_SUCCESS;
		}
	}
	else if (Word1 == "--build")
	{
		*_This.output << "use \"build\" and not \"--build\"\n";
	}
	else if (Word1 == "index" || Word1 == "-i")
	{
		auto _Path = GetPath(Line);
		auto _PathAsPath = Path(_Path);
		if (_Path.size() == 0)
		{
			_PathAsPath = std::filesystem::current_path();
		}
		Optional<Path> ModulePath;

		if (fs::is_directory(_PathAsPath))
		{
			ModulePath = _PathAsPath / UCodeLang::ModuleFile::FileNameWithExt;
			if (!fs::exists(_PathAsPath))
			{
				ModulePath = {};
			}
		}
		else if (fs::exists(_PathAsPath) && _PathAsPath.extension() == UCodeLang::ModuleFile::FileExtWithDot)
		{
			ModulePath = _PathAsPath;
		}

		if (ModulePath.has_value()) 
		{
			UCodeLang::ModuleIndex f = UCodeLang::ModuleIndex::GetModuleIndex();
			f.AddModueToList(ModulePath.value());
			UCodeLang::ModuleIndex::SaveModuleIndex(f);
			_This.ExeRet = EXIT_SUCCESS;
		}
		else
		{
			_This.ExeRet =EXIT_FAILURE;
		}
	}
	else if (Word1 == "modules" || Word1 == "-m")
	{
		UCodeLang::ModuleIndex f = UCodeLang::ModuleIndex::GetModuleIndex();

		String ret;
		ret += "[";
		for (auto& Item : f._IndexedFiles)
		{
			ret += Item._ModuleFullPath.generic_string();
			if (&Item != &f._IndexedFiles.back())
			{
				ret += ',';
			}
		}
		ret += "]";

		* _This.output << Path(ret);
	}
	else if (Word1 == "path" || Word1 == "-p")
	{
		UCodeLang::Path::string_type exepath;
		
		#if UCodeLang_Platform_Windows
		exepath.resize(MAX_PATH);
		auto v = GetModuleFileName(nullptr, exepath.data(), exepath.size());
		
		#endif 
		#if UCodeLang_Platform_Linux
		exepath = std::filesystem::canonical("/proc/self/exe").native();
		#endif 

		* _This.output << Path(exepath) << '\n';
	}
	else if (Word1 == "globalpath" || Word1 == "-gp")
	{
		auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalDirectory();

		AppPrintin(Path(ucodebinpath));
	}
	else if (Word1 == "modulepath" || Word1 == "-mp")
	{
		auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalModulesDownloads();

		AppPrintin(Path(ucodebinpath));
	}
	else if (Word1 == "binpath" || Word1 == "-bin")
	{
		auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalBin();

		AppPrintin(Path(ucodebinpath));
	}
	else if (Word1 == "version" || Word1 == "-v")
	{
		*_This.output << Path(UCodeLang::LangInfo::VersionName);
	}
	else if (Word1 == "new" || Word1 == "-n")
	{
		
		std::string TepNameBuffer;
		bool DoGetFlag = false;

		String _Path = String(GetPath(Line));
		String Name = String(GetPath(Line));

		
		String OwnerName = String(GetPath(Line));

		auto DoGit = GetPath(Line);
		if (_Path.size())
		{
			auto path= std::filesystem::current_path();
			_Path = fs::absolute(path).generic_string();
		}
		if (Name.size() == 0)
		{
			*_This.output << "Enter Module Name:";

			std::getline(*_This.Input, TepNameBuffer);
			Name = std::move(TepNameBuffer);
		}

		if (OwnerName.size() == 0)
		{
			//system("git config user.name");

			*_This.output << "Enter Owner/Author Name:";

			std::getline(*_This.Input, TepNameBuffer);
			OwnerName = std::move(TepNameBuffer);
		}
		if (DoGit.size()==0)
		{
			*_This.output << "make git project(y/n):";

			std::getline(*_This.Input, TepNameBuffer);
			if (TepNameBuffer == "y" || TepNameBuffer == "yes" || TepNameBuffer == "y")
			{
				DoGetFlag = true;
			}
		}
		else
		{
			if (DoGit == "y" || DoGit == "yes" || DoGit  == "y")
			{
				DoGetFlag = true;
			}
		}

		Path NewDir = fs::absolute(Path(_Path).native() + Path(Name).native());
		Path modePath = NewDir / Path(ModuleFile::FileNameWithExt).native();

		fs::create_directories(NewDir);

		fs::create_directories(NewDir / ModuleFile::ModuleSourcePath);

		{
			std::ofstream file(NewDir / ModuleFile::ModuleSourcePath / (Path("main").native() + Path(UCodeLang::FileExt::SourceFileWithDot).native()));
			file << ModuleFile::DefaultSourceFile;
			file.close();
		}

		{
			std::ofstream file(NewDir / ModuleFile::ModuleBuildfile);
			file << ModuleFile::DefaultBuildFile;
			file.close();
		}


		fs::create_directories(NewDir / ModuleFile::ModuleOutPath);

		UCodeLang::ModuleFile module;
		module.NewInit((String)Name, (String)OwnerName);
		UCodeLang::ModuleFile::ToFile(&module, modePath);


		UCodeLang::ModuleIndex f = UCodeLang::ModuleIndex::GetModuleIndex();
		f.AddModueToList(modePath);
		UCodeLang::ModuleIndex::SaveModuleIndex(f);
	}
	else if (Word1 == "fmt" || Word1 == "-f")
	{
		String _Path = String(GetPath(Line));
		if (_Path.size())
		{
			auto path = std::filesystem::current_path();
			_Path = fs::absolute(path).generic_string();
		}

		Path Pathas = _Path;

		if (fs::is_directory(_Path))
		{
			for (const auto& dirEntry : fs::recursive_directory_iterator(_Path))
			{
				if (dirEntry.is_regular_file())
				{
					UCodeAnalyzer::Formatter f;
					String txt = Compiler::GetTextFromFile(dirEntry.path());
					auto newtxt = f.Format(UCodeAnalyzer::Formatter::StrScope::FileScope, String_view(txt));
					if (newtxt.has_value())
					{
						std::ofstream file(dirEntry.path());
						file << newtxt.value();
						file.close();
					}
				}
			}

		}
		else if (fs::is_regular_file(_Path))
		{
			UCodeAnalyzer::Formatter f;
			String txt = Compiler::GetTextFromFile(_Path);
			auto newtxt = f.Format(UCodeAnalyzer::Formatter::StrScope::FileScope, String_view(txt));
			if (newtxt.has_value())
			{
				std::ofstream file(_Path);
				file << newtxt.value();
				file.close();
			}
		}
	}
	else if (Word1 == "doc" || Word1 == "-dc")
	{
		//Generate documentation
	}
	else if (Word1 == "debug" || Word1 == "-d")
	{
		bool debugruning = false;
		while (debugruning)
		{

		}
	}
	else if (Word1 == "eval" || Word1 == "-e")
	{
		bool debugruning = false;
		while (debugruning)
		{

		}
	}
	else if (Word1 == "test" || Word1 == "-t")
	{
		bool use01 = UCodeLang::StringHelper::Contains(Line, "-01");
		bool use02 = UCodeLang::StringHelper::Contains(Line, "-02");
		bool use03 = UCodeLang::StringHelper::Contains(Line, "-03");
		bool usedebug = !(UCodeLang::StringHelper::Contains(Line, "-ndebug") || UCodeLang::StringHelper::Contains(Line, "-nd"));
		bool use32mode = UCodeLang::StringHelper::Contains(Line, "-32");
		bool use64mode = UCodeLang::StringHelper::Contains(Line, "-64");
		
		bool usejit = UCodeLang::StringHelper::Contains(Word1, "-jit");
		
		#ifdef UCodeLang_HasNoNativeULangBackEnd
		bool usenative = false;
		#else 
		bool usenative = UCodeLang::StringHelper::Contains(Word1, "-native");
		#endif
		auto oldline = Line;
		String _Path = String(GetPath(Line));
		auto _PathAsPath = Path(_Path);
		if (_Path.size())
		{
			

			bool isflag = false;
			for (auto& Item : _Path)
			{
				if (Item == '-')
				{
					isflag = true;
					break;
				}
			}

			if (isflag)
			{
				Line = oldline;
				_PathAsPath = std::filesystem::current_path();
				_Path = _PathAsPath.generic_string();
			}

		}

		Compiler _Compiler;
		{
			auto& Settings = _Compiler.Get_Settings();

			if (use64mode) {
				Settings.PtrSize = IntSizes::Int64;
			}
			if (use32mode) {
				Settings.PtrSize = IntSizes::Int32;
			}
			if (usedebug)
			{
				*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::Debug;
			}
			else
			{
				*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::Debug;
			}
			if (use01)
			{
				*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_1;
			}
			else
			{
				*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_1;
			}
			if (use02)
			{
				*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_2;
			}
			else
			{
				*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_2;
			}
			if (use03)
			{
				*(OptimizationFlags_t*)&Settings._Flags |= (OptimizationFlags_t)OptimizationFlags::O_3;
			}
			else
			{
				*(OptimizationFlags_t*)&Settings._Flags &= ~(OptimizationFlags_t)OptimizationFlags::O_3;
			}
		}
		Optional<UClib> libop;

		if (usenative)
		{
			#ifdef UCodeLang_HasNoNativeULangBackEnd
			UCodeLangUnreachable();
			#else
			_Compiler.Set_BackEnd(NativeULangBackEnd::MakeObject);
			#endif
		}

		if (!buildfile2(_PathAsPath, _Compiler, libop))
		{
			_This.ExeRet = EXIT_FAILURE;
		}
		else
		{
			auto& lib = libop.value();

			*_This.output << _Compiler.Get_Errors().ToString();
			AppPrintin("Runing Tests:");
			

			TestRuner::InterpreterType interpreter;
			if (usenative)
			{
				interpreter = TestRuner::InterpreterType::NativeInterpreter;
			}
			else if (usejit)
			{
				interpreter = TestRuner::InterpreterType::JitInterpreter;
			}
			else
			{
				interpreter = TestRuner::InterpreterType::Interpreter;
			}

			TestRuner runer;
			auto info = runer.RunTests(lib, interpreter, [](TestRuner::TestInfo& test)
			{
					if (test.Passed)
					{
						AppPrintin("Test :" << test.TestName << " Passed");
					}
					else
					{
						AppPrintin("Test :" << test.TestName << " Fail");
					}
			});
			bool passed = info.TestCount == info.TestPassedCount;
			AppPrintin("Ran all " << info.TestCount << " Tests");
			
			int passnumber;
			if (info.TestPassedCount)
			{
				passnumber = ((float)info.TestPassedCount / (float)info.TestCount) *100;
			}
			else
			{
				passnumber = 100;
			}
			
			
			if (passed)
			{
				AppPrintin("Tests Passed.all 100% of tests passed");
				_This.ExeRet = EXIT_SUCCESS;
			}
			else
			{
				AppPrintin("Tests Failed about " << passnumber << "% passed");

				_This.ExeRet = EXIT_FAILURE;
			}
		}
	}
	else if (Word1 == "run" || Word1 == "-r")
	{
		bool usejit = UCodeLang::StringHelper::Contains(Word1, "-jit");
		
		#ifdef UCodeLang_HasNoNativeULangBackEnd
		bool usenative = false;
		#else
		bool usenative = UCodeLang::StringHelper::Contains(Word1, "-native");
		#endif
		bool use03 = UCodeLang::StringHelper::Contains(Word1, "-03");
		bool tooutjson = UCodeLang::StringHelper::Contains(Word1, "-outjson");

		Path filetorun = "main.uc";
		
		String functocall = "main";

		String _Arg = String(GetPath(Line));

		auto oldv = Line;
		String _Arg2 = String(GetWord(Line));

		if (_Arg.size()) 
		{
			if (fs::exists(_Arg))
			{
				filetorun = _Arg;
			}
			else
			{
				if (_Arg.front() != '-') {
					functocall = _Arg;
				}
			}
		}

		if (_Arg2.size())
		{
			if (_Arg2.front() != '-') {
				functocall = _Arg2;
			}
			else
			{

				Line = oldv;
			}
		}
		else
		{
			Line = oldv;
		}

		filetorun = fs::absolute(filetorun);
		if (fs::exists(filetorun)) 
		{
			
			UCodeLang::Optional<UClib>  outlibOp;
			{
				Compiler _Compiler;
				if (usenative)
				{
					#ifdef UCodeLang_HasNoNativeULangBackEnd
					UCodeLangUnreachable();
					#else
					_Compiler.Set_BackEnd(NativeULangBackEnd::MakeObject);
					#endif
				}
				if (use03)
				{
					_Compiler.Get_Settings()._Flags = OptimizationFlags::O_3;
				}
				else
				{
					_Compiler.Get_Settings()._Flags = OptimizationFlags::O_1;
				}
				buildfile2(filetorun, _Compiler, outlibOp);
			}

			if (outlibOp)
			{
				UClib& outlib= outlibOp.value();
				auto& Assembly = outlib.Get_Assembly();
				Vector<String_view> Parstxt;


				bool isreadingpars =false;
				for (size_t i = 0; i < Line.size(); i++)
				{
					auto Item = Line[i];

					if (isreadingpars) 
					{
						String_view v;
						for (size_t i2 = i; i2 < Line.size(); i2++)
						{
							auto Item2 = Line[i2];

							if (!IsList(Worddef, Item2))
							{
								String_view v = Line.substr(i, i2 - 1);
								if (v.size())
								{
									Parstxt.push_back(v);
								}
								if (Item2 == ')')
								{
									isreadingpars = false;
								}
							}
						}

						if (isreadingpars)
						{
							AppPrintin("missing ')'");
							return;
						}
					}
					else
					{
						if (Item == '(')
						{
							isreadingpars = true;
							continue;
						}
						else if (Item == ')')
						{
							isreadingpars = false;
							break;
						}
					}
				}



				Vector<ClassAssembly::ParsedValue> Pars;
				Pars.resize(Parstxt.size());
				const ClassMethod* func = nullptr;
				auto funcs = Assembly.Find_Funcs(functocall);
				for (auto& Item : funcs)
				{
					if (Item->ParsType.size() == Parstxt.size())
					{
						func = Item;
						for (size_t i = 0; i < Parstxt.size(); i++)
						{
							auto info = ClassAssembly::ParseToValue(Parstxt[i], outlib.Get_Assembly(), {
							Item->ParsType[i].Type });

							if (info.has_value())
							{
								Pars[i] = std::move(info.value());
							}
							else
							{
								func = nullptr;
							}
						}

						break;
					}
				}
				if (func)
				{
					UCodeLang::RunTimeLangState runtime;
					UCodeLang::AnyInterpreter interpreter;
					if (usenative)
					{
						interpreter.SetAsNativeInterpreter();
					}
					else if (usejit)
					{
						interpreter.SetAsJitInterpreter();
					}
					else
					{
						interpreter.SetAsInterpreter();
					}
					RunTimeLib lib;
					lib.Init(&outlib);

					runtime.AddLib(&lib);
					runtime.LinkLibs();
					
					interpreter.Init(&runtime);

					interpreter.Call(StaticVariablesInitializeFunc);
					interpreter.Call(ThreadVariablesInitializeFunc);
					bool is32mode = sizeof(void*) == 4;
					for (auto& Item : Pars)
					{
						interpreter.PushParameter(Item.Value._Data.Get_Data(),
							Assembly.GetSize(Item.Value._Type, is32mode).value());
					}
					interpreter.Call(func);
					auto rettype = func->RetType;
					size_t retsize = Assembly.GetSize(rettype, is32mode).value();
					TypedRawReflectionData v;
					v._Type = rettype;
	
					v._Data.Resize(retsize);
					interpreter.Get_Return(v._Data.Get_Data(), retsize);

					interpreter.Call(ThreadVariablesUnLoadFunc);
					interpreter.Call(StaticVariablesUnLoadFunc);


					
					AppPrintin("Returned:");
					if (tooutjson)
					{
						AppPrintin(ClassAssembly::ToStringJson(v,Assembly,is32mode));
					}
					else
					{
						AppPrintin(ClassAssembly::ToString(v, Assembly, is32mode));
					}
				}
				else
				{
					AppPrint("Cant find function '" << func << '[');
					for (auto& Item : Pars)
					{
						AppPrint(ClassAssembly::ToString(Item.GetType(),Assembly));
						if (&Item != &Pars.back())
						{
							AppPrint(',');
						}
					}
					AppPrintin("]\'");
				}
			}
		}
		else
		{
			AppPrintin("Cant find file '" << filetorun << '\'');
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "get" || Word1 == "-g")
	{
		String _Path = String(GetPath(Line));
		auto _PathAsPath = Path(_Path);


		if (_Path.size()==0)
		{
			_PathAsPath = std::filesystem::current_path();
			_Path = _PathAsPath.generic_string();
		}

		Optional<ModuleFile> modfile;
		if (fs::is_directory(_PathAsPath))
		{
			Path modulepath = _PathAsPath / ModuleFile::FileNameWithExt;
			if (fs::exists(modulepath))
			{
				ModuleIndex _ModuleIndex = ModuleIndex::GetModuleIndex();

				UCodeLang::ModuleFile module;
				if (!UCodeLang::ModuleFile::FromFile(&module, modulepath))
				{
					*_This.output << "Cant Open module file\n";
					_This.ExeRet = EXIT_FAILURE;
				}
				else
				{
					modfile = std::move(module);
				}
			}
		}
		else if (fs::is_regular_file(_PathAsPath) && _PathAsPath.extension() == ModuleFile::FileExt)
		{
			UCodeLang::ModuleFile module;
			if (!UCodeLang::ModuleFile::FromFile(&module, _PathAsPath))
			{
				*_This.output << "Cant Open module file\n";
				_This.ExeRet = EXIT_FAILURE;
			}
			else
			{
				modfile = std::move(module);
			}
		}
		else
		{
			AppPrintin("Cant find Path '" << _PathAsPath << "'");
		}

		if (modfile.has_value())
		{
			ModuleIndex _ModuleIndex = ModuleIndex::GetModuleIndex();
			auto& mod = modfile.value();

			String out;
			bool ok = mod.DownloadModules(_ModuleIndex, Optionalref(out));

			AppPrintin(out);
			if (ok)
			{
				_This.ExeRet = EXIT_SUCCESS;
			}
			else
			{
				_This.ExeRet = EXIT_FAILURE;
			}
		}
	}
	else if (Word1 == "dump" || Word1 == "-dp")
	{
		String _Path = String(GetPath(Line));
		auto _PathAsPath = Path(_Path);
		if (!_Path.size())
		{
			_PathAsPath = std::filesystem::current_path();
			_Path = _PathAsPath.generic_string();
		}

		if (fs::exists(_PathAsPath))
		{
			if (fs::is_directory(_PathAsPath))
			{
				Compiler _Compiler;
				Optional<UClib> libop;
				if (!buildfile2(_PathAsPath, _Compiler, libop))
				{
					AppPrintin("Compiler Fail:");
					AppPrintin(_Compiler.Get_Errors().ToString());
					_This.ExeRet = EXIT_FAILURE;
				}
				else
				{
					AppPrintin(UCodeLang::UAssembly::UAssembly::ToString(&libop.value()));
					_This.ExeRet = EXIT_SUCCESS;
				}
			}
			else if (fs::is_regular_file(_PathAsPath))
			{
				auto ext = _PathAsPath.extension();
				if (ext == FileExt::SourceFileWithDot ||ext == ModuleFile::FileExtWithDot)
				{
					Compiler _Compiler;
					Optional<UClib> libop;
					if (!buildfile2(_PathAsPath, _Compiler, libop))
					{
						AppPrintin("Compiler Fail:");
						AppPrintin(_Compiler.Get_Errors().ToString());
						_This.ExeRet = EXIT_FAILURE;
					}
					else 
					{
						AppPrintin(UCodeLang::UAssembly::UAssembly::ToString(&libop.value()));
						_This.ExeRet = EXIT_SUCCESS;
					}
				}
				else if (ext == FileExt::ObjectWithDot
					|| ext == FileExt::LibWithDot
					|| ext == FileExt::DllWithDot)
				{
					UClib lib;
					if (UClib::FromFile(&lib, _PathAsPath))
					{
						AppPrintin(UCodeLang::UAssembly::UAssembly::ToString(&lib));
						_This.ExeRet = EXIT_SUCCESS;
					}
					else
					{
						AppPrintin("Cant Open " << _PathAsPath);
						_This.ExeRet = EXIT_FAILURE;

					}
				}
				else if (ext == FileExt::IRWithDot)
				{
					IRBuilder lib;
					if (IRBuilder::FromFile(lib, _PathAsPath))
					{
						AppPrintin(lib.ToString());
						_This.ExeRet = EXIT_SUCCESS;
					}
					else
					{
						AppPrintin("Cant Open " << _PathAsPath);
						_This.ExeRet = EXIT_FAILURE;

					}
				}
				else
				{
					AppPrintin("Unknown file type" << _PathAsPath);
					AppPrintin("Must be .uc,.uo,.ulib,.udll,.uir,.ucm");
					_This.ExeRet = EXIT_FAILURE;
				}
			}
			else
			{
				AppPrintin("file path must be file or directory.");
				_This.ExeRet = EXIT_FAILURE;
			}
		}
		else
		{
			AppPrintin("file path must exits.");
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "cpptoulangvm" || Word1 == "-cpptoulangvm")
	{
		Path cppfile = GetPath(Line);
		Path cpplink = GetPath(Line);
		Path ulangout = GetPath(Line);

		UCodeAnalyzer::CppHelper v;
		if (v.ParseCppfileAndOutULang(cppfile, cpplink, ulangout))
		{
			_This.ExeRet = EXIT_SUCCESS;
		}
		else
		{
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "cpptolink" || Word1 == "-cpptoulanglink")
	{
		Path cppfile;
		Path cpplink;
		Path ulangout;

		UCodeAnalyzer::CppHelper v;
		if (v.ParseCppfileAndOutULangLink(cppfile, cpplink, ulangout))
		{
			_This.ExeRet = EXIT_SUCCESS;
		}
		else
		{
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "ulangtocpp" || Word1 == "-ulangtocpp")
	{
		Path cppfile;
		Path cpplink;
		Path ulangout;

		UCodeAnalyzer::CppHelper v;
		if (v.ParseULangToCppStaticLink(cppfile, cpplink, ulangout))
		{
			_This.ExeRet = EXIT_SUCCESS;
		}
		else
		{
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "clear" || Word1 == "-cc")
	{
		String _Path = String(GetPath(Line));
		auto _PathAsPath = Path(_Path);
		if (_Path.size() == 0)
		{
			_PathAsPath = std::filesystem::current_path();
			_Path = _PathAsPath.generic_string();
		}
		else if (fs::exists(_PathAsPath) && _PathAsPath.extension() == UCodeLang::ModuleFile::FileExtWithDot)
		{
			_PathAsPath = _PathAsPath.parent_path();
			_Path = _PathAsPath.generic_string();
		}

		if (fs::is_directory(_PathAsPath))
		{
			auto intpath = _PathAsPath / "int";
			auto outpath = _PathAsPath / "out";

			fs::remove_all(intpath);
			fs::remove_all(outpath);
		}
		else
		{
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (Word1 == "runlines" || Word1 == "-el")
	{
		std::ifstream file(Path(GetPath(Line)));
		if (file.is_open())
		{

			for (std::string line; getline(file, line); )
			{
				std::string_view V = line;
				ParseLine(V);
			}

			file.close();
		}
	}
	else if (Word1 == "out" || Word1 == "-o")
	{
		//set the output
	}
	else if (Word1 == "reindex" || Word1 == "-ri")
	{
		UCodeLang::ModuleIndex f = UCodeLang::ModuleIndex::GetModuleIndex();
		f.RemoveDeletedModules();
		UCodeLang::ModuleIndex::SaveModuleIndex(f);


		_This.ExeRet = EXIT_SUCCESS;
	}
	else if (Word1 == "help" || Word1 == "-h")
	{
		AppPrintin("Usage: uclang [command] [args]");
		*_This.output << "put help info here.\n";

		AppPrintin("new           [Path]     :Makes a new Module Project");
		AppPrintin("build         [Path]     :Builds the UCode file,Module,folder");
		AppPrintin("run           [Path] <FunctionName(<Args>)>    :run the UCode file,Module,folder,.ulib");
		AppPrintin("test          [Path]     :Runs Tests the UCode file,Module,folder,.ulib");
		AppPrintin("dump          [Path]     :Convert the UCode file,Module,folder,.ulib to Readable Text");
		AppPrintin("fmt           [Path]     :Format the UCode file,Module,folder");
		AppPrintin("index         [Path]     :Adds Module to to the Module Index");
		AppPrintin("update        [Path]     :Updates Module Dependencies");
		AppPrintin("cpptoulangvm  [CppPath] [UCodeLangBindingPath] [CppVMBindingPath]     :Makes ULang Bindings from C++ Source for UCodeVm");

		AppPrintin("install [Name] :Installs a UCodeLangModule");
		AppPrintin("runmod [Name] :runs a Installed UCodeLangModule");
		AppPrintin("uninstallmod [Name] :uninstalls a Installed UCodeLangModule");

		AppPrintin("updatetools [Name] :updates UCodeLangTools");
		AppPrintin("uninstalltools [Name] :uninstalls UCodeLangTools");
		AppPrintin("exit : closes the window");
	}
	else if (Word1 == "exit" || Word1 == "-q")
	{
		_This.EndApp = true;
	}
	else if (Word1 == "--new")
	{
		AppPrintin("use \"new\" and not \"--new\"");


		_This.ExeRet = EXIT_FAILURE;
	}
	else if (Word1 == "--run")
	{
		AppPrintin("use \"run\" and not \"--run\"");

		_This.ExeRet = EXIT_FAILURE;
	}
	else if (Word1 == "--help")
	{
		AppPrintin("use \"help\" and not \"--help\"\n");

		_This.ExeRet = EXIT_FAILURE;
	}
	else
	{
		AppPrintin("bad command use the \"help\" command for help");
		//TokenCheck(Word1);
	}
}
bool buildfile(UCodeLang::Path& filetorun, UCodeLang::Compiler& _Compiler)
{
	UCodeLang::Optional<UCodeLang::UClib> lib;
	return buildfile2(filetorun, _Compiler,lib, true);
}
bool buildfile2(UCodeLang::Path& filetorun, UCodeLang::Compiler& _Compiler, UCodeLang::Optional<UCodeLang::UClib>& outlibOp,bool allwaysoutputfile)
{
	bool r = false;
	namespace fs = std::filesystem;
	if (fs::is_directory(filetorun))
	{
		Path modulepath = filetorun / ModuleFile::FileNameWithExt;
		if (fs::exists(filetorun))
		{
			ModuleIndex _ModuleIndex = ModuleIndex::GetModuleIndex();

			UCodeLang::ModuleFile module;
			if (!UCodeLang::ModuleFile::FromFile(&module, modulepath))
			{
				*_This.output << "Cant Open module file\n";
				_This.ExeRet = EXIT_FAILURE;
			}
			else {
				bool ItWorked = module.BuildModule(_Compiler, _ModuleIndex).CompilerRet.IsValue();
				if (!ItWorked)
				{
					*_This.output << "Compiler Fail:\n";
					*_This.output << _Compiler.Get_Errors().ToString();
					_This.ExeRet = EXIT_FAILURE;
				}
				else
				{
					r = true;
					if (allwaysoutputfile == false) {
						Path uclibpath = module.GetPaths(_Compiler).OutFile;

						UClib v;
						if (UClib::FromFile(&v, uclibpath))
						{
							outlibOp = std::move(v);
						}
					}
				}
			}
		}
		else
		{
			Compiler::CompilerPathData v;
			v.FileDir = filetorun;
			v.IntDir = (filetorun / ModuleFile::ModuleIntPath);
			v.OutFile = Path((filetorun / filetorun.filename()).native() + Path(_Compiler.GetOutputExtWithDot()).native());
			
			bool ItWorked = _Compiler.CompileFiles_UseIntDir(v).IsValue();
			if (!ItWorked)
			{
				*_This.output << "Compiler Fail:\n";
				*_This.output << _Compiler.Get_Errors().ToString();
				_This.ExeRet = EXIT_FAILURE;
			}
			else
			{
				r = true;
				if (allwaysoutputfile == false) {
					UClib lib;
					if (UClib::FromFile(&lib, v.OutFile))
					{
						outlibOp = std::move(lib);
					}
				}
			}
		}
	}
	else if (filetorun.extension() == Path(FileExt::SourceFileWithDot))
	{
		Path v = filetorun.native() + Path("out").native() + Path(_Compiler.GetOutputExtWithDot()).native();
		bool ItWorked = _Compiler.CompilePathToObj(filetorun, v).IsValue();
		
		if (!ItWorked)
		{
			*_This.output << "Compiler Fail:\n";
			*_This.output << _Compiler.Get_Errors().ToString();
			_This.ExeRet = EXIT_FAILURE;
		}
		else
		{
			r = true;
			if (allwaysoutputfile == false) {
				UClib lib;
				if (UClib::FromFile(&lib, v))
				{
					outlibOp = std::move(lib);
				}
			}
		}
	}
	else if (filetorun.extension() == Path(UCodeLang::ModuleFile::FileExtWithDot))
	{
		ModuleIndex _ModuleIndex = ModuleIndex::GetModuleIndex();

		UCodeLang::ModuleFile module;
		if (!UCodeLang::ModuleFile::FromFile(&module, filetorun))
		{
			*_This.output << "Cant Open module file\n";
			_This.ExeRet = EXIT_FAILURE;
		}
		else 
		{
			bool ItWorked = module.BuildModule(_Compiler, _ModuleIndex).CompilerRet.IsValue();
			if (!ItWorked)
			{
				*_This.output << "Compiler Fail:\n";
				*_This.output << _Compiler.Get_Errors().ToString();
				_This.ExeRet = EXIT_FAILURE;

				
			}
			else
			{
				Path uclibpath = module.GetPaths(_Compiler).OutFile;
				r = true;
				if (allwaysoutputfile == false) {
					UClib v;
					if (UClib::FromFile(&v, uclibpath))
					{
						outlibOp = std::move(v);
					}
				}
			}
		}
	}
	else if (filetorun.extension() == Path(UCodeLang::FileExt::AsmWithDot))
	{
		UAssembly::UAssembly compiler;
		UClib lib;
		auto txt = Compiler::GetTextFromFile(filetorun);
		if (compiler.Assemble(txt, &lib))
		{
			outlibOp = std::move(lib);

			if (allwaysoutputfile)
			{
				Path outpath = filetorun.native() + Path(UCodeLang::FileExt::LibWithDot).native();

				*_This.output << _Compiler.Get_Errors().ToString();
				if (UClib::ToFile(&lib, outpath))
				{
					_This.ExeRet = EXIT_SUCCESS;
				}
				else
				{
					*_This.output << "Cant Write File:\n";
					_This.ExeRet = EXIT_FAILURE;
				}
			}
		}
		else
		{
			*_This.output << "Compiler Fail:\n";
			*_This.output << _Compiler.Get_Errors().ToString();
			_This.ExeRet = EXIT_FAILURE;
		}
	}
	else if (filetorun.extension() == Path(UCodeLang::FileExt::IRTextWithDot))
	{
		IRBuilder builder;
		auto txt = Compiler::GetTextFromFile(filetorun);
		UCodeLangToDo();
	}
	else
	{
		AppPrintin("file type must be .uc,.ucm,.uirtxt,.uasm '" << filetorun << '\'');
	}
	return r;
}

void TokenCheck(const UCodeLang::String_view& Word1)
{
	*_This.output << "bad Token '" << Word1 << "'" << std::endl;
}
