#ifndef UCodeLangNoCompiler

#include "ModuleFile.hpp"
#include <sstream> 
#include <fstream>
#include "UCodeLang/Compilation/Front/Lexer.hpp"
#include <filesystem>

#include <thread>
#include <mutex>
#include <future>

#include <UCodeLang/RunTime/Interpreters/Interpreter.hpp>
#include <UCodeLang/RunTime/RunTimeLangState.hpp>

#include "CompilationSettings.hpp"
UCodeLangStart


BytesPtr ModuleIndex::ToBytes(const ModuleIndex* Lib)
{
	BitMaker bit;
	{
		bit.WriteType((BitMaker::SizeAsBits)Lib->_IndexedFiles.size());
		for (auto& Item : Lib->_IndexedFiles)
		{
			WriteType(bit, Item._ModuleFullPath.generic_string());

			bit.WriteType(Item._ModuleData.AuthorName);
			bit.WriteType(Item._ModuleData.ModuleName);

			bit.WriteType(Item._ModuleData.MajorVersion);
			bit.WriteType(Item._ModuleData.MinorVersion);
			bit.WriteType(Item._ModuleData.RevisionVersion);
		}
	}

	return bit.AsBytePtrAndMove();
}

bool ModuleIndex::FromBytes(ModuleIndex* Lib, const BytesView Bytes)
{
	Lib->_IndexedFiles.clear();

	BitReader bits;
	bits.SetBytes(Bytes.Data(), Bytes.Size());


	BitReader::SizeAsBits Size=0;
	bits.ReadType(Size, Size);

	for (size_t i = 0; i < Size; i++)
	{
		IndexModuleFile Item;

		FromType(bits, Item._ModuleFullPath);

		bits.ReadType(Item._ModuleData.AuthorName, Item._ModuleData.AuthorName);
		bits.ReadType(Item._ModuleData.ModuleName, Item._ModuleData.ModuleName);

		bits.ReadType(Item._ModuleData.MajorVersion, Item._ModuleData.MajorVersion);
		bits.ReadType(Item._ModuleData.MinorVersion, Item._ModuleData.MinorVersion);
		bits.ReadType(Item._ModuleData.RevisionVersion, Item._ModuleData.RevisionVersion);

		Lib->_IndexedFiles.push_back(std::move(Item));
	}

	return true;
}

bool ModuleIndex::ToFile(const ModuleIndex* Lib, const Path& path)
{
	
	std::ofstream File(path);

	if (File.is_open()) 
	{
		BytesPtr bits = ToBytes(Lib);
		File.write((const char*)bits.Data(),bits.Size());
		File.close();
		return true;
	}
	else
	{
		return false;
	}
}
bool ModuleIndex::FromFile(ModuleIndex* Lib, const Path& path)
{
	std::ifstream File(path);

	if (File.is_open())
	{
		BytesPtr Bits;
		File.seekg(0, File.end);
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);

		File.read((char*)Bits.Data(), Bits.Size());
		File.close();
		auto V = FromBytes(Lib, Bits.AsSpan());

		return V;
	}
	else
	{
		return false;
	}
}

Optional<size_t> ModuleIndex::FindFile(const ModuleIdentifier& ID) const
{
	for (size_t i = 0; i < _IndexedFiles.size(); i++)
	{
		auto& Item = _IndexedFiles[i];
		if (Item._ModuleData.AuthorName == ID.AuthorName
			&& Item._ModuleData.ModuleName == ID.ModuleName)
		{
			return i;
		}
	}
	return {};
}
Optional<size_t> ModuleIndex::FindFileFullPath(const Path& file) const
{
	for (size_t i = 0; i < _IndexedFiles.size(); i++)
	{
		auto& Item = _IndexedFiles[i];
		if (Item._ModuleFullPath== file)
		{
			return i;
		}
	}
	return {};
}
void ModuleIndex::AddModueToList(const Path& path)
{

	auto V = FindFileFullPath(path);
	if (V.has_value())
	{
		return;
	}

	ModuleFile modfile;
	if (ModuleFile::FromFile(&modfile, path))
	{
		IndexModuleFile file;
		file._ModuleData = modfile.ModuleName;
		file._ModuleFullPath = path;
		_IndexedFiles.push_back(std::move(file));
	}
}
Path ModuleIndex::GetModuleIndexFilePath()
{
	return (LangInfo::GetUCodeGlobalDirectory() / FileName).concat(FileExtWithDot);
}

void ModuleIndex::WriteType(BitMaker& bit, const Path& Value)
{
	bit.WriteType(Value.generic_string());
}
ModuleIndex ModuleIndex::GetModuleIndex()
{
	ModuleIndex	LangIndex;
	auto Path = ModuleIndex::GetModuleIndexFilePath();
	if (!std::filesystem::exists(Path))
	{
		ModuleIndex::ToFile(&LangIndex, Path);

		return LangIndex;
	}

	if (ModuleIndex::FromFile(&LangIndex, Path))
	{
		return LangIndex;
	}
	else
	{
		return LangIndex;
	}
}
bool ModuleIndex::SaveModuleIndex(ModuleIndex& Lib)
{
	return Lib.ToFile(&Lib, ModuleIndex::GetModuleIndexFilePath());
}
void ModuleIndex::RemoveDeletedModules()
{
	namespace fs = std::filesystem;
	Vector<IndexModuleFile> newlist;
	for (auto& Item : _IndexedFiles)
	{
		if (fs::exists(Item._ModuleFullPath))
		{
			newlist.push_back(std::move(Item));
		}
	}

	this->_IndexedFiles =std::move(newlist);
}
void ModuleIndex::FromType(BitReader& bit, Path& Value)
{
	String Out;
	bit.ReadType(Out, Out);
	Value = Out;;
}
Compiler::CompilerPathData ModuleFile::GetPaths(Compiler& Compiler, bool IsSubModule) const
{
	Compiler::CompilerPathData paths;
	auto B = Path(ThisModuleDir);

	String OutputName = Compiler.GetBackEndName();

	Path ValueToAddIfSubModule;
	if (IsSubModule)
	{
		ValueToAddIfSubModule += Path::preferred_separator;
		ValueToAddIfSubModule += "SubModule";
	}

	auto& Settings = Compiler.Get_Settings();

	String SettingStr;

	if (Settings.PtrSize == IntSizes::Int32)
	{
		SettingStr += "32";
	}
	else if (Settings.PtrSize == IntSizes::Int64)
	{
		SettingStr += "64";
	}

	if ((OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug)
	{
		SettingStr  += "-Debug";
	}
	
	if ((OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::O_1)
	{
		SettingStr += "-O1";
	}
	if ((OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::O_2)
	{
		SettingStr += "-O2";
	}
	if ((OptimizationFlags_t)Settings._Flags & (OptimizationFlags_t)OptimizationFlags::O_3)
	{
		SettingStr += "-O3";
	}

	paths.FileDir = Path((B.native() + Path::preferred_separator + Path(ModuleSourcePath).native())).generic_string();
	paths.IntDir = Path((B.native()  + Path::preferred_separator + Path(ModuleIntPath).native() + ValueToAddIfSubModule.native() + Path::preferred_separator + Path(OutputName).native() + Path::preferred_separator + Path(SettingStr).native())).generic_string();
	paths.OutFile = Path((B.native() + Path::preferred_separator + Path(ModuleOutPath).native() + ValueToAddIfSubModule.native() + Path::preferred_separator + Path(OutputName).native() + Path::preferred_separator + Path(SettingStr).native() + Path::preferred_separator)).generic_string();
	
	
	if (!std::filesystem::exists(paths.IntDir))
	{
		std::filesystem::create_directories(paths.IntDir);
	}


	if (!std::filesystem::exists(paths.OutFile))
	{
		std::filesystem::create_directories(paths.OutFile);
	}

	paths.OutFile += ModuleName.ModuleName;
	if (IsSubModule) 
	{
		paths.OutFile += FileExt::LibWithDot;
	}
	else {
		paths.OutFile += Compiler.GetOutputExtWithDot();
	}
	return paths;
}
String ModuleFile::ToName(const ModuleIdentifier& ID)
{
	String R;
	R += ID.ModuleName;
	R += "[" + std::to_string(ID.MajorVersion) + "." + std::to_string(ID.MinorVersion) + "." + std::to_string(ID.RevisionVersion) + "]:";
	R += ID.AuthorName;
	return R;
}
bool ModuleFile::DownloadModules(const ModuleIndex& Modules,OptionalRef<String> LogsOut)
{

	for (auto& Item : ModuleDependencies)
	{
		auto file = Modules.FindFile(Item.Identifier);
		if (!file.has_value() && Item.WebLink)
		{
			const auto& WebLink = Item.WebLink.value();
		
			bool isgithub = StringHelper::StartWith(WebLink,"https://github.com");

			if (LogsOut.has_value())
			{
				LogsOut.value() += "Download Modules is not added yet";
			}
			return false;
		}
	}

	return true;
}
ModuleFile::ModuleRet ModuleFile::BuildModule(Compiler& Compiler, const ModuleIndex& Modules, bool IsSubModule, OptionalRef<String> LogsOut)
{
	String out;
	if (DownloadModules(Modules,Optionalref(out)))
	{
		
		if (LogsOut.has_value())
		{
			LogsOut.value() += out;
		}
		Compiler::CompilerPathData paths = GetPaths(Compiler, IsSubModule);
		Compiler::ExternalFiles ExternFiles;
		auto& Errs = Compiler.Get_Errors();

		Errs.FilePath = GetFullPathName();

		Path buildfile = ThisModuleDir / ModuleBuildfile.native();

		bool allowdebugsubmodules = false;

		{

			namespace fs = std::filesystem;
			if (fs::exists(buildfile))
			{
				auto OldSettings = Compiler.Get_Settings();

				Compiler.Get_Settings() = CompilationSettings();

				
				auto build = BuildFile(Compiler.GetTextFromFile(buildfile),Compiler, Modules);

				Compiler.Get_Settings() = OldSettings;
				
				if (build.CompilerRet.IsValue())
				{
					UClib& buildscriptlib = *build.CompilerRet.GetValue().OutPut;

					ClassMethod* buildfunction = buildscriptlib.Get_Assembly().Find_Func("build");

					if (buildfunction)
					{
						using BuildRuner = Interpreter;

						RunTimeLib buildscriptrlib;
						buildscriptrlib.Init(&buildscriptlib);

						RunTimeLib BuildSystemlib;
						//BuildSystemlib.Add_CPPCall("BuildSystem::Build",nullptr, nullptr);

						RunTimeLangState state;
						state.AddLib(&buildscriptrlib);

						state.LinkLibs();


						BuildRuner runer;
						runer.Init(&state);


						auto itworked = runer.RCall<int>(buildfunction, this);


						if (itworked == 0)
						{
							ModuleRet CompilerRet =Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
							return CompilerRet;
						}
						else
						{
							ModuleRet CompilerRet =Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
							return CompilerRet;
						}
					}
					else
					{
						Compiler.Get_Errors().AddError(ErrorCodes::ExpectingToken, 0, 0, "Cant find function |build[BuildSystem& system] for build script.");
						ModuleRet CompilerRet =Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
						return CompilerRet;
					}
				}
				else
				{
					ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
					return CompilerRet;
				}
			}
			else
			{
				bool Err = false;

				BuildModuleDependencies(Modules, Errs, Err, Compiler, this->ModuleDependencies, ExternFiles);

				if (Err == false)
				{
					auto OldSettings = Compiler.Get_Settings();

					#if UCodeLangDebug
					std::cout << "Building:" << this->ModuleName.ModuleName;
					std::cout << '\n';
					#endif

					ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
					{
						Compiler.Get_Settings()._Type = IsSubModule ? OutPutType::IRAndSymbols : OldSettings._Type;
						CompilerRet.OutputItemPath = GetPaths(Compiler, IsSubModule).OutFile;

						if (!allowdebugsubmodules)
						{
							//tell the IROptimizer ignored debug symbols so optimizations can take place since people dont debug submodules often
							Compiler.Get_Settings().AddArgFlag("IgnoreDebug");
						}

						if (ForceImport) {
							Compiler.Get_Settings().AddArgFlag("ForceImport");
						}

						if (ModuleNameSpace.size())
						{
							Compiler.Get_Settings().AddArgValue("StartingNameSpace", ModuleNameSpace);
						}

						CompilerRet.CompilerRet = Compiler.CompileFiles_UseIntDir(paths, ExternFiles);
					}

					Compiler.Get_Settings() = OldSettings;
					return CompilerRet;
				}
				else
				{
					ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
					return CompilerRet;
				}
			}
		}
	}
	else
	{
		if (LogsOut.has_value())
		{
			LogsOut.value() += out;
		}
		Compiler.Get_Errors().AddError(ErrorCodes::CouldNotFindFunc, 0, 0, "Download Error:" + out);


		ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
		return CompilerRet;
	}
}

void ModuleFile::BuildModuleDependencies(
	const ModuleIndex& Modules, 
	CompilationErrors& Errs, bool& Err,
	Compiler& Compiler,
	const Vector<ModuleDependencie>& ModuleDependencies,
	Compiler::ExternalFiles& externfilesoutput)
{
	for (auto& Item : ModuleDependencies)
	{
		auto V = Modules.FindFile(Item.Identifier);
		if (!V.has_value())
		{
			Errs.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Find Mudule Named " + ToName(Item.Identifier));
			Err = true;
		}
		else
		{
			const ModuleIndex::IndexModuleFile& Index = Modules._IndexedFiles[V.value()];
			ModuleFile MFile;
			if (ModuleFile::FromFile(&MFile, Index._ModuleFullPath))
			{
				auto BuildData = MFile.BuildModule(Compiler, Modules, true);
				if (BuildData.CompilerRet.IsError())
				{
					Errs.FilePath = GetFullPathName();

					Errs.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Build Mudule " + ToName(ModuleName) + " because of an Err in " + ToName(Item.Identifier));
					Err = true;
				}
				else
				{
					externfilesoutput.Files.push_back(MFile.GetPaths(Compiler, true).OutFile);
				}

			}

		}
	}

}

bool ModuleFile::ToFile(const ModuleFile* Lib, const Path& path)
{
	namespace fs = std::filesystem;
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{
		String V = ToStringBytes(Lib);
		File.write((const char*)V.c_str(), V.size());

		
		File.close();
		return true;
	}
	return false;
}

bool ModuleFile::FromFile(ModuleFile* Lib, const Path& path)
{
	std::ifstream inFile(path);
	if (inFile.is_open())
	{
		Lib->ThisModuleDir = path.parent_path();

		std::stringstream strStream;
		strStream << inFile.rdbuf();
		std::string str = strStream.str();
		inFile.close();


		return FromString(Lib, str);
	}
	else
	{
		return false;
	}

}
void ModuleFile::NewInit(String ModuleName, String AuthorName)
{
	this->ModuleName.ModuleName = ModuleName;
	this->ModuleName.AuthorName = AuthorName;
	this->ModuleNameSpace = ModuleName;
	{
		
		ModuleDependencie f;
		f.Identifier.MajorVersion = 0;
		f.Identifier.MinorVersion = 0;
		f.Identifier.MinorVersion = 0;

		f.Identifier.AuthorName = "UCodeLang";
		f.Identifier.ModuleName = "StandardLibarary";
		ModuleDependencies.push_back(std::move(f));
	}
}
String ModuleFile::ToStringBytes(const ModuleIdentifier* Value)
{
	String out;

	out += "AuthorName:" + Value->AuthorName + '\n';
	out += "ModuleName:" + Value->ModuleName + '\n';

	out += "Version:" + std::to_string(Value->MajorVersion) 
		+ ":" + std::to_string(Value->MinorVersion) 
		+ ":" + std::to_string(Value->RevisionVersion);
	return out;
}
ModuleFile::ModuleRet ModuleFile::BuildFile(const String& filestring, Compiler& Compiler, const ModuleIndex& Modules)
{
	auto& Errs = Compiler.Get_Errors();
	String fileimports = "";
	String filetext = "$BuildSystem; \n|build[BuildSystem& system] => 0;";

	Vector<ModuleDependencie> buildfileDependencies;
	Compiler::ExternalFiles buildExternFiles;

	bool Err = false;
	BuildModuleDependencies(Modules, Errs, Err, Compiler, buildfileDependencies, buildExternFiles);

	auto buildscriptinfo = Compiler.CompileText(filetext);
	if (Err == false)
	{
		if (buildscriptinfo.IsValue())
		{
			ModuleRet CompilerRet = std::move(buildscriptinfo);
			CompilerRet.OutputItemPath = "";
			return CompilerRet;
		}
		else
		{
			ModuleRet CompilerRet = std::move(buildscriptinfo);
			CompilerRet.OutputItemPath = "";
			return CompilerRet;
		}
	}
	else
	{
		ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
		return CompilerRet;
	}
}
String ModuleFile::ToStringBytes(const Path* Value)
{
	String out =Value->generic_string();


	return out;
}
String ModuleFile::ToStringBytes(const ModuleFile* Lib)
{
	String out;
	out += ToStringBytes(&Lib->ModuleName) + "\n" + '\n';

	out += (String)"ForceImport:" + (Lib->ForceImport ? "true" : "false") + "\n";
	out += (String)"RemoveUnSafe:" + (Lib->RemoveUnSafe ? "true" : "false") + "\n";
	out += "NameSpace:" + Lib->ModuleNameSpace;

	out += "\n";

	for (auto& Item : Lib->ModuleDependencies)
	{
		out += "import ";
		out += Item.Identifier.AuthorName;
		out += "::" + Item.Identifier.ModuleName;
		out += + "[" + std::to_string(Item.Identifier.MajorVersion)
			+ ':' + std::to_string(Item.Identifier.MinorVersion)
			+ ':' + std::to_string(Item.Identifier.RevisionVersion) + "]";

		if (Item.WebLink.has_value())
		{
			out += "(";
			out += Item.WebLink.value();
			out += ")";
		}
		out += ";\n";
	}
	

	return out;
}

bool ModuleFile::FromString(ModuleFile* Lib, const String_view& Data)
{
	CompilationErrors tep;
	UCodeLang::FrontEnd::Lexer lex;
	lex.Set_ErrorsOutput(&tep);
	lex.Lex(Data);
	

	auto& tokens = lex.Get_Tokens();

	if (tep.Has_Errors())
	{
		return false;
	}

	{
		ModuleIdentifier Tep;
		Optional<String> WebLink;

#define IsGood(x) if (i > tokens.size()) {return false;}

		for (size_t i = 0; i < tokens.size(); i++)
		{
			auto& Item = tokens[i];
			if (Item.Type == TokenType::Name)
			{
				i++; IsGood(i);
				if (tokens[i].Type == TokenType::Colon)
				{
					i++; IsGood(i);
					if (Item.Value._String == "AuthorName")
					{
						Tep.AuthorName = tokens[i].Value._String;
					}
					else if (Item.Value._String == "ModuleName")
					{
						Tep.ModuleName = tokens[i].Value._String;
					}
					else if (Item.Value._String == "Version")
					{
						Lib->ModuleName = std::move(Tep);
					}
					else if (Item.Value._String == "ForceImport")
					{
						Lib->ForceImport = tokens[i].Type == TokenType::KeyWorld_True;
					}
					else if (Item.Value._String == "RemoveUnSafe")
					{
						Lib->RemoveUnSafe = tokens[i].Type == TokenType::KeyWorld_True;
					}
					else if (Item.Value._String == "NameSpace")
					{
						if (tokens[i].Type == TokenType::Name) {
							Lib->ModuleNameSpace = tokens[i].Value._String;
						}
						else
						{
							i--;
						}
					}
				}

			}
			else if (Item.Type == TokenType::KeyWord_Import)
			{
				i++; IsGood(i);
				if (tokens[i].Type == TokenType::Name)
				{
					auto& AuthorNameName = tokens[i];

					i++; IsGood(i);
					if (tokens[i].Type != TokenType::ScopeResolution)
					{
						return false;
					}

					i++; IsGood(i);
					if (tokens[i].Type != TokenType::Name)
					{
						return false;
					}	
					auto& ModNameName = tokens[i];
				
					i++; IsGood(i);
					if (tokens[i].Type != TokenType::Left_Bracket)
					{
						return false;
					}

					i++; IsGood(i);
					if (tokens[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num0 = tokens[i];

					i++;

					i++; IsGood(i);
					if (tokens[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num1 = tokens[i];

					i++;

					i++; IsGood(i);
					if (tokens[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num2 = tokens[i];

					i++; IsGood(i);
					i++;
					if (i < tokens.size()) 
					{
						if (tokens[i].Type == TokenType::Left_Parentheses)
						{
							i++; IsGood(i);
							if (tokens[i].Type == TokenType::String_literal)
							{
								WebLink = tokens[i].Value._String;
							}
						}
					}


					ModuleDependencie moddep;
					
					moddep.Identifier.AuthorName = AuthorNameName.Value._String;
					moddep.Identifier.ModuleName = ModNameName.Value._String;
					
					moddep.Identifier.MajorVersion = std::stoi((String)Num0.Value._String);
					moddep.Identifier.MinorVersion = std::stoi((String)Num1.Value._String);
					moddep.Identifier.RevisionVersion = std::stoi((String)Num2.Value._String);

					moddep.WebLink = std::move(WebLink);
					Lib->ModuleDependencies.push_back(std::move(moddep));
				}
			}
		}

	}
	return true;
}

UCodeLangEnd

#endif