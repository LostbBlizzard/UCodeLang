#pragma once
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

class ModuleFile
{
public:
	ModuleIdentifier ModuleName;
	Path ThisModuleDir;
	//all are relative to ModulePath.
	Path ModuleSourcePath = "src";
	Path ModuleIntPath = "int";
	Path ModuleOutPath = "out";
	
	

	Vector<ModuleIdentifier> ModuleDependencies;

	ModuleFile()
	{

	}
	~ModuleFile()
	{

	}
	Compiler::CompilerRet BuildModule(Compiler& Compiler);;
	static bool ToFile(const ModuleFile* Lib, const Path& path);
	static bool FromFile(ModuleFile* Lib, const Path& path);

	void NewInit(String ModuleName, String AuthorName);


	static String ToStringBytes(const  ModuleIdentifier* Value);
	static String ToStringBytes(const  Path* Value);

	static String ToStringBytes(const  ModuleFile* Lib);
	static bool FromString(ModuleFile* Lib, const String_view& Data);

	inline static const char* FileExtWithDot = ".ucm";
	inline static const char* FileExt = "ucm";
};
UCodeLangEnd
