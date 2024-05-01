#pragma once
#ifndef UCodeLangNoCompiler
#include "Compiler.hpp"
UCodeLangStart

struct ModuleIdentifier
{
	String AuthorName;
	String ModuleName;

	UInt64 MajorVersion = 0;
	UInt64 MinorVersion = 0;
	UInt64 RevisionVersion = 0;
};

class ModuleIndex
{
public:

	struct IndexModuleFile
	{
		Path _ModuleFullPath;
		ModuleIdentifier _ModuleData;
	};

	ModuleIndex()
	{

	}
	~ModuleIndex()
	{

	}


	static BytesPtr ToBytes(const ModuleIndex* Lib);
	static bool FromBytes(ModuleIndex* Lib, const BytesView Bytes);
	static bool ToFile(const ModuleIndex* Lib, const Path& path);
	static bool FromFile(ModuleIndex* Lib, const Path& path);

	Optional<size_t> FindFile(const ModuleIdentifier& ID) const;

	Optional<size_t> FindFileFullPath(const Path& file) const;

	Vector<IndexModuleFile> _IndexedFiles;
	void AddModueToList(const Path& path);

	static Path GetModuleIndexFilePath();

	static void WriteType(BitMaker& bit, const Path& Value);
	static void FromType(BitReader& bit, Path& Value);


	static ModuleIndex GetModuleIndex();

	static bool SaveModuleIndex(ModuleIndex& Lib);


	void RemoveDeletedModules();

	inline static const char* FileName = "ModuleIndex";
	inline static const char* FileExtWithDot = ".ucmi";
	inline static const char* FileExt = "ucmi";
private:
};



class ModuleFile
{
public:
	ModuleIdentifier ModuleName;
	Path ThisModuleDir;

	bool ForceImport = false;
	bool RemoveUnSafe = false;
	String ModuleNameSpace = "";

	struct ModuleRet
	{
		Compiler::CompilerRet CompilerRet;
		Path OutputItemPath;
		ModuleRet(NeverNullPtr<UClib> OutPut) :
			CompilerRet(OutPut)
		{
		}
		ModuleRet(Compiler::CompilerRet&& OutPut) :
			CompilerRet(std::move(OutPut))
		{
		}
	};
	struct ModuleDependencie
	{
		ModuleIdentifier Identifier;
		Optional<String> WebLink;
	};
	Vector<ModuleDependencie> ModuleDependencies;
	using LogOut = std::function<void(String mgs)>;

	ModuleFile()
	{

	}
	~ModuleFile()
	{

	}
	Compiler::CompilerPathData GetPaths(Compiler& Compiler, bool IsSubModule = false) const;

	static String ToName(const ModuleIdentifier& ID);
	Path GetFullPathName()
	{
		return Path(ThisModuleDir) / (String(FileName) + FileExtWithDot);;
	}
	
	bool DownloadModules(const ModuleIndex& Modules, Optional<LogOut> LogsOut, TaskManger& tasks);	
	ModuleRet BuildModule(Compiler& Compiler, const ModuleIndex& Modules, bool IsSubModule, Optional<LogOut> LogsOut, TaskManger& tasks);
	ModuleRet BuildFile(const String& filestring, Compiler& Compiler, const ModuleIndex& Modules, Optional<LogOut> LogsOut,TaskManger& tasks);

	bool DownloadModules(const ModuleIndex& Modules, Optional<LogOut> LogsOut = {})
	{
		auto tasks = TaskManger();
		tasks.Init();
		return DownloadModules(Modules, LogsOut, tasks);
	}
	ModuleRet BuildModule(Compiler& Compiler, const ModuleIndex& Modules, bool IsSubModule = false, Optional<LogOut> LogsOut = {})
	{
		auto tasks = TaskManger();
		tasks.Init();
		return BuildModule(Compiler, Modules, IsSubModule,LogsOut,tasks);
	}
	ModuleRet BuildFile(const String& filestring, Compiler& Compiler, const ModuleIndex& Modules, Optional<LogOut> LogsOut = {})
	{
		auto tasks = TaskManger();
		tasks.Init();
		return BuildFile(filestring, Compiler, Modules, LogsOut, tasks);
	}

	static bool ToFile(const ModuleFile* Lib, const Path& path);
	static bool FromFile(ModuleFile* Lib, const Path& path);

	static Optional<ModuleFile> OpenFile(const Path& path)
	{
		ModuleFile r;
		if (FromFile(&r, path))
		{
			return r;
		}
		return {};
	}
	bool WriteToFile(const Path& path)
	{
		return ToFile(this, path);
	}

	void NewInit(String ModuleName, String AuthorName);


	static String ToStringBytes(const  ModuleIdentifier* Value);
	static String ToStringBytes(const  Path* Value);

	static String ToStringBytes(const  ModuleFile* Lib);
	static bool FromString(ModuleFile* Lib, const String_view& Data);

	inline static const char* FileName = "ULangModule";
	inline static const char* FileExtWithDot = ".ucm";
	inline static const char* FileExt = "ucm";
	inline static const char* FileNameWithExt = "ULangModule.ucm";


	//all are relative to ModulePath.
	inline static const Path ModuleSourcePath = "src";
	inline static const Path ModuleIntPath = "int";
	inline static const Path ModuleOutPath = "out";
	inline static const Path ModuleBuildfile = "Build.uc";

	inline static const String DefaultSourceFile = "use ULang;\n\n\n|main[]:\n  Fmt::Println(\"Hello World\");";
	inline static const String DefaultBuildFile = "//Build Script Module Imports \n\nimport UCodeLang::StandardLibrary[0:0:0];\nimport UCodeLang::BuildSystem[0:0:0];\n\n//Your code starts here \nuse ULang;\nuse ULang::BuildSystem;\n\n|build[BuildSystem& system] => system.Build();";
private:
	void BuildModuleDependencies(const ModuleIndex& Modules, CompilationErrors& Errs, bool& Err, Compiler& Compiler
		, const Vector<ModuleDependencie>& ModuleDependencies
		, Compiler::ExternalFiles& externfilesoutput, Optional<LogOut> LogsOut,TaskManger& task);

};
UCodeLangEnd

#endif