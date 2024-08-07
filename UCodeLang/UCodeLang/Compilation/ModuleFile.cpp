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
#include <UCodeLang/RunTime/SandBoxedIOLink.hpp>

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


	BitReader::SizeAsBits Size = 0;
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
		File.write((const char*)bits.Data(), bits.Size());
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
		if (Item._ModuleFullPath == file)
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
		file._ModuleFullPath = std::filesystem::absolute(path);
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

	this->_IndexedFiles = std::move(newlist);
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
		SettingStr += "-Debug";
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
	paths.IntDir = Path((B.native() + Path::preferred_separator + Path(ModuleIntPath).native() + ValueToAddIfSubModule.native() + Path::preferred_separator + Path(OutputName).native() + Path::preferred_separator + Path(SettingStr).native())).generic_string();
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
void ModuleFile::BuildModuleDependencies(
	const ModuleIndex& Modules,
	CompilationErrors& Errs, bool& Err,
	Compiler& Compiler,
	const Vector<ModuleDependencie>& ModuleDependencies,
	Compiler::ExternalFiles& externfilesoutput, Optional<LogOut> LogsOut, TaskManger& tasks)
{
	for (auto& Item : ModuleDependencies)
	{
		auto V = Modules.FindFile(Item.Identifier);
		if (!V.has_value())
		{
			Errs.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Find Module Named " + ToName(Item.Identifier));
			Err = true;
			return;
		}

	}

	struct TaskR
	{
		CompilationErrors err;
		bool didgood = false;
		Optional<Path> Outpath;
	};
	Vector<TaskManger::Task<TaskR>> modtasks;
	modtasks.reserve(ModuleDependencies.size());

	for (auto& Item : ModuleDependencies)
	{
		std::function<TaskR()> func = [Modules, Item, this, &Compiler, LogsOut, &tasks]() -> TaskR
			{
				auto V = Modules.FindFile(Item.Identifier);


				const ModuleIndex::IndexModuleFile& Index = Modules._IndexedFiles[V.value()];
				ModuleFile MFile;
				if (ModuleFile::FromFile(&MFile, Index._ModuleFullPath))
				{
					auto BuildData = MFile.BuildModule(Compiler, Modules, true, LogsOut, tasks);
					if (BuildData.CompilerRet.IsError())
					{
						TaskR r;
						r.didgood = false;

						r.err.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Build Mudule " + ToName(ModuleName) + " because of an Err in " + ToName(Item.Identifier));
						return r;
					}
					else
					{
						TaskR r;
						r.didgood = true;
						r.Outpath = MFile.GetPaths(Compiler, true).OutFile;

						return r;

					}

				}
				else
				{
					TaskR r;
					r.didgood = false;

					r.err.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Open Mudule At " + Index._ModuleFullPath.generic_string() + ".it may not exist.");
					return r;
				}
			};

		auto t = tasks.AddTask(func, {});
		t.Run();
		modtasks.push_back(TaskManger::Task<TaskR>(std::move(t)));
	}



	bool haveErr = false;
	for (auto& Item : modtasks)
	{
		auto r = Item.Wait();
		if (r.didgood == false)
		{
			haveErr = true;
		}

		for (auto& Item : r.err.Get_Errors())
		{
			Errs.AddError(std::move(Item));
		}

		if (r.Outpath.has_value())
		{
			externfilesoutput.Files.push_back(std::move(r.Outpath.value()));
		}
	}
	Err = haveErr;
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

	this->RemoveUnSafe = true;
	{

		ModuleDependencie f;
		f.Identifier.MajorVersion = 0;
		f.Identifier.MinorVersion = 0;
		f.Identifier.MinorVersion = 0;

		f.Identifier.AuthorName = "UCodeLang";
		f.Identifier.ModuleName = "StandardLibrary";
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

bool ModuleFile::DownloadModules(const ModuleIndex& Modules, Optional<LogOut> LogsOut, TaskManger& tasks)
{
	Vector<TaskManger::Task<bool>> RuningTask;
	RuningTask.reserve(ModuleDependencies.size());


	for (size_t i = 0; i < ModuleDependencies.size(); i++)
	{
		auto& Item = ModuleDependencies[i];

		std::function<bool()> func = [Modules, Item, &tasks, LogsOut]() -> bool
			{
				auto file = Modules.FindFile(Item.Identifier);
				if (!file.has_value() && Item.WebLink)
				{
					const auto& WebLink = Item.WebLink.value();
					auto outdir = LangInfo::GetUCodeGlobalModulesDownloads() / Item.Identifier.AuthorName / Item.Identifier.ModuleName;
					String versionstr = std::to_string(Item.Identifier.MajorVersion) + "." + std::to_string(Item.Identifier.MinorVersion) + "." + std::to_string(Item.Identifier.RevisionVersion);
					outdir /= versionstr;

					String modid = ModuleFile::ToName(Item.Identifier);

					if (!std::filesystem::exists(outdir))
					{
						std::filesystem::create_directories(outdir);


						Vector<String> VaildGitTags;
						{
							VaildGitTags.push_back(versionstr);
							VaildGitTags.push_back("v" + versionstr);
						}

						bool ok = false;
						for (auto& Item : VaildGitTags)
						{
							String Cmd = "git clone ";

							Cmd += WebLink + " --branch " + Item + " " + outdir.generic_string();
							if (LogsOut.has_value())
							{
								(*LogsOut)("trying to download " + modid + " from " + WebLink);
							}

							#if UCodeLang_Platform_IPHONE
							ok = false;
							if (LogsOut.has_value())
							{
								(*LogsOut)("c funcion 'system' is unavailable: not available on iOS");
							}
							#else
							ok = system(Cmd.c_str()) == EXIT_SUCCESS;
							#endif
							if (ok) { break; }
						}


						if (ok)
						{
							//TODO add suport for monorepos. one git with many UCodeModules.
							auto modulefilepath = outdir / ModuleFile::FileNameWithExt;

							if (!std::filesystem::exists(modulefilepath))
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("cant find ") + ModuleFile::FileNameWithExt + " in " + WebLink);
								}

								std::filesystem::remove_all(outdir);
								return false;
							}

							ModuleFile modulef;
							if (!modulef.FromFile(&modulef, modulefilepath))
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("cant open/parse ") + ModuleFile::FileNameWithExt + " in " + WebLink);
								}

								std::filesystem::remove_all(outdir);
								return false;
							}

							bool arenotthesame = false;

							if (modulef.ModuleName.AuthorName != Item.Identifier.AuthorName)
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("AuthorNames do not match in ") + ModuleFile::FileNameWithExt + " from " + WebLink);
								}
								arenotthesame = true;
							}

							if (modulef.ModuleName.ModuleName != Item.Identifier.ModuleName)
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("AuthorNames do not match in ") + ModuleFile::FileNameWithExt + " from " + WebLink);
								}
								arenotthesame = true;
							}

							if (modulef.ModuleName.MajorVersion != Item.Identifier.MajorVersion)
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("MajorVerion do not match in ") + ModuleFile::FileNameWithExt + " from " + WebLink);
								}
								arenotthesame = true;
							}

							if (modulef.ModuleName.MinorVersion != Item.Identifier.MinorVersion)
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("MinorVerion do not match in ") + ModuleFile::FileNameWithExt + " from " + WebLink);
								}
								arenotthesame = true;
							}

							if (modulef.ModuleName.RevisionVersion != Item.Identifier.RevisionVersion)
							{
								if (LogsOut.has_value())
								{
									(*LogsOut)(String("RevisonVersion do not match in ") + ModuleFile::FileNameWithExt + " from " + WebLink);
								}
								arenotthesame = true;
							}

							if (!arenotthesame)
							{
								std::filesystem::remove_all(outdir);
								return false;
							}
							else
							{
								auto mod = ModuleIndex::GetModuleIndex();
								mod.AddModueToList(modulefilepath);
								ModuleIndex::SaveModuleIndex(mod);
								return true;
							}
						}
						else
						{
							if (LogsOut.has_value())
							{
								(*LogsOut)("failed to download " + modid);
							}
						}
						return false;
					}
				}

				return true;
			};

		auto task = tasks.AddTask(func, {});
		task.Run();
		RuningTask.push_back(std::move(task));
	}

	bool r = true;

	for (auto& Item : RuningTask)
	{
		if (!Item.Wait())
		{
			r = false;
		}
	}

	return r;

}
ModuleFile::ModuleRet ModuleFile::BuildModule(Compiler& Compiler, const ModuleIndex& Modules, bool IsSubModule, Optional<LogOut> LogsOut, TaskManger& tasks)
{
	if (DownloadModules(Modules, LogsOut, tasks))
	{

		Compiler::CompilerPathData paths = GetPaths(Compiler, IsSubModule);
		Compiler::ExternalFiles ExternFiles;
		auto& Errs = Compiler.Get_Errors();

		Errs.FilePath = GetFullPathName();

		Path buildfile = ThisModuleDir / ModuleBuildfile.native();

		bool allowdebugsubmodules = false;

		bool Err = false;

		BuildModuleDependencies(Modules, Errs, Err, Compiler, this->ModuleDependencies, ExternFiles, LogsOut, tasks);

		if (Err == false)
		{

			namespace fs = std::filesystem;
			if (fs::exists(buildfile))
			{

				auto OldSettings = Compiler.Get_Settings();

				Compiler.Get_Settings() = CompilationSettings();

				auto oldname = std::move(Errs.FilePath);
				Errs.FilePath = buildfile;
				auto build = BuildFile(Compiler.GetTextFromFile(buildfile), Compiler, Modules, LogsOut, tasks);
				Errs.FilePath = std::move(oldname);

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


						struct APIObject
						{
							std::function<bool()> F;

							static bool Build(APIObject* This)
							{
								return This->F();
							}
						};

						APIObject apiobject;

						ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));

						SandBoxedIOLink::Link(BuildSystemlib);
						//this kind of defeats the point of running code.
						//but also we don't trust people.
						//even more so with recursive dependencies
						//we don't want npm security holes
						//TODO  

						apiobject.F = [&]() -> bool
							{
								if (LogsOut.has_value())
								{
									Compiler.SetLog([&]()
										{
											(*LogsOut)("Building:" + this->ModuleName.ModuleName);
										});
								}
								CompilerRet.CompilerRet = Compiler.CompileFiles_UseIntDir(paths, ExternFiles, tasks);
								return CompilerRet.CompilerRet.IsValue();
							};

						BuildSystemlib.Add_CPPCall("ULang:BuildSystem:Internal:BuildSys_Build", [](InterpreterCPPinterface& Input)
							{
								auto par = Input.GetParameter<APIObject*>();
								Input.Set_Return(APIObject::Build(par));
							}, APIObject::Build);

						RunTimeLangState state;
						state.AddLib(&buildscriptrlib);
						state.AddLib(&BuildSystemlib);



						state.LinkLibs();


						BuildRuner runer;
						runer.Init(&state);


						auto itworked = runer.RCall<bool>(buildfunction, &apiobject);


						if (itworked)
						{
							return CompilerRet;
						}
						else
						{
							Compiler.Get_Errors().AddError(ErrorCodes::CouldNotFindFunc, 0, 0, "build funcion returned false");
							ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
							return CompilerRet;
						}
					}
					else
					{
						Compiler.Get_Errors().AddError(ErrorCodes::ExpectingToken, 0, 0, "Cant find function |build[BuildSystem& system] for build script.");
						ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
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
				auto OldSettings = Compiler.Get_Settings();


				if (LogsOut.has_value())
				{
					Compiler.SetLog([&]()
					{
						(*LogsOut)("Building:" + this->ModuleName.ModuleName);
					});
				}
				else
				{
					Compiler.RemoveLog();
				}

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

					CompilerRet.CompilerRet = Compiler.CompileFiles_UseIntDir(paths, ExternFiles, tasks);
				}

				Compiler.Get_Settings() = OldSettings;
				return CompilerRet;

			}

			{
				ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
				return CompilerRet;
			}

		}
		else
		{
			ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
			return CompilerRet;
		}
	}
	ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
	return CompilerRet;
}

ModuleFile::ModuleRet ModuleFile::BuildFile(const String& filestring, Compiler& Compiler, const ModuleIndex& Modules, Optional<LogOut> LogsOut, TaskManger& tasks)
{
	auto& Errs = Compiler.Get_Errors();
	String fileimports = "";
	String filetext = "";

	{

		size_t ImportEnd = 0;
		String Line;
		size_t StartIndex = 0;
		size_t LineIndex = 0;
		for (size_t i = 0; i < filestring.size(); i++)
		{
			auto item = filestring[i];

			if (item == '\n')
			{
				Line = filestring.substr(StartIndex, i - StartIndex);
				StartIndex = i + 1;
				if (StringHelper::StartWith(Line, "import"))
				{
					fileimports += Line;
					fileimports += '\n';
					ImportEnd = LineIndex;
				}
				LineIndex++;
			}
		}

		size_t LinePassed = 0;

		for (size_t i = 0; i < ImportEnd; i++)
		{
			filetext += "\n";//so line numbers line up in errors
		}

		for (size_t i = 0; i < filestring.size(); i++)
		{
			auto item = filestring[i];

			if (item == '\n')
			{
				LinePassed++;
			}

			if (LinePassed > ImportEnd)
			{
				filetext += item;
			}
		}
	}


	Vector<ModuleDependencie> buildfileDependencies;
	{
		String modstr = R"(AuthorName: "UCodeLang"
ModuleName: "BuildSystem"
Version: 0:0:0)";

		modstr += "\n";
		modstr += fileimports;

		ModuleFile mod;

		auto v = FromString(&mod, modstr);

		buildfileDependencies = std::move(mod.ModuleDependencies);

		if (!mod.DownloadModules(Modules, LogsOut, tasks))
		{
			ModuleRet CompilerRet = Compiler::CompilerRet(NeverNullptr(&Compiler.Get_Errors()));
			return CompilerRet;
		}
	}

	Compiler::ExternalFiles buildExternFiles;

	bool Err = false;

	auto oldname = Errs.FilePath;
	BuildModuleDependencies(Modules, Errs, Err, Compiler, buildfileDependencies, buildExternFiles, LogsOut, tasks);
	Errs.FilePath = oldname;


	if (Err == false)
	{
		auto buildscriptinfo = Compiler.CompileText(filetext, buildExternFiles, tasks);//TODO cash this file to avoid full builds

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
	String out = Value->generic_string();


	return out;
}
String ModuleFile::ToStringBytes(const ModuleFile* Lib)
{
	String out;
	out += ToStringBytes(&Lib->ModuleName) + "\n" + '\n';

	out += (String)"ForceImport:" + (Lib->ForceImport ? "true" : "false") + "\n";
	out += (String)"RemoveUnSafe:" + (Lib->RemoveUnSafe ? "true" : "false") + "\n";
	out += "NameSpace:" + Lib->ModuleNameSpace;

	out += "\n\n";

	for (auto& Item : Lib->ModuleDependencies)
	{
		out += "import ";
		out += Item.Identifier.AuthorName;
		out += "::" + Item.Identifier.ModuleName;
		out += +"[" + std::to_string(Item.Identifier.MajorVersion)
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
						if (tokens[i].Type == TokenType::Name)
						{
							while (tokens[i].Type == TokenType::Name)
							{
								Lib->ModuleNameSpace += tokens[i].Value._String;
								if (i + 2 < tokens.size()
									&& tokens[i + 1].Type == TokenType::ScopeResolution
									&& tokens[i + 2].Type == TokenType::Name)
								{
									i += 2;
									Lib->ModuleNameSpace += ScopeHelper::_ScopeSep;
								}
								else
								{
									break;
								}
							}
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
