#include "ModuleFile.hpp"
#include <sstream> 
#include <fstream>
#include "UCodeLang/Compliation/Front/Lexer.hpp"
#include <filesystem>

#include <thread>
#include <mutex>
#include <future>
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

	BytesPtr CompilerRet;
	CompilerRet.Bytes = std::make_unique<Byte[]>(bit.size());
	memcpy(CompilerRet.Bytes.get(), bit.data(), bit.size());
	CompilerRet.Size = bit.size();
	return CompilerRet;
}

bool ModuleIndex::FromBytes(ModuleIndex* Lib, const BytesView Bytes)
{
	Lib->_IndexedFiles.clear();

	BitReader bits;
	bits.SetBytes(Bytes.Bytes, Bytes.Size);


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
		File.write((const char*)bits.Bytes.get(),bits.Size);
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
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Bytes = std::make_unique<Byte[]>(Bits.Size);

		File.read((char*)Bits.Bytes.get(), Bits.Size);
		File.close();
		auto V = FromBytes(Lib, { Bits.Bytes.get(),Bits.Size });

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

void ModuleIndex::FromType(BitReader& bit, Path& Value)
{
	String Out;
	bit.ReadType(Out, Out);
	Value = Out;;
}
Compiler::CompilerPathData ModuleFile::GetPaths(Compiler& Compiler) const
{
	Compiler::CompilerPathData paths;
	auto B = Path(ThisModuleDir);

	String OutputName = Compiler.GetBackEndName();

	paths.FileDir = Path((B.native() + Path::preferred_separator + Path(ModuleSourcePath).native())).generic_string();
	paths.IntDir = Path((B.native()  + Path::preferred_separator + Path(ModuleIntPath).native() + Path::preferred_separator + Path(OutputName).native())).generic_string();
	paths.OutFile = Path((B.native() + Path::preferred_separator + Path(ModuleOutPath).native() + Path::preferred_separator + Path(OutputName).native() + Path::preferred_separator)).generic_string();
	
	
	if (!std::filesystem::exists(paths.IntDir))
	{
		std::filesystem::create_directories(paths.IntDir);
	}


	if (!std::filesystem::exists(paths.OutFile))
	{
		std::filesystem::create_directories(paths.OutFile);
	}
	paths.OutFile += ModuleName.ModuleName + Compiler.GetOutputExtWithDot();

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
ModuleFile::ModuleRet ModuleFile::BuildModule(Compiler& Compiler, const ModuleIndex& Modules)
{
	Compiler::CompilerPathData paths = GetPaths(Compiler);
	Compiler::ExternalFiles ExternFiles;
	auto& Errs = Compiler.Get_Errors();

	Errs.FilePath = GetFullPathName();
	bool Err = false;
	for (auto& Item : ModuleDependencies)
	{
		auto V = Modules.FindFile(Item);
		if (!V.has_value())
		{
			Errs.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Find Mudule Named " + ToName(Item));
			Err = true;
		}
		else
		{
			const ModuleIndex::IndexModuleFile& Index = Modules._IndexedFiles[V.value()];
			ModuleFile MFile;
			if (ModuleFile::FromFile(&MFile, Index._ModuleFullPath))
			{
				auto BuildData = MFile.BuildModule(Compiler, Modules);
				if (BuildData.CompilerRet._State == Compiler::CompilerState::Fail)
				{
					Errs.FilePath = GetFullPathName();

					Errs.AddError(ErrorCodes::ExpectingSequence, 0, 0, "Cant Build Mudule " + ToName(ModuleName) + " because of an Err in " + ToName(Item));
					Err = true;
				}
				else
				{
					ExternFiles.Files.push_back(MFile.GetPaths(Compiler).OutFile);
				}

			}

		}
	}
	
	if (Err == false) 
	{
		ModuleRet CompilerRet;

		//

		//
		CompilerRet.OutputItemPath = paths.OutFile;
		CompilerRet.CompilerRet = Compiler.CompileFiles_UseIntDir(paths, ExternFiles);
		return CompilerRet;
	}
	else
	{
		ModuleRet CompilerRet;
		CompilerRet.CompilerRet._State = Compiler::CompilerState::Fail;
		return CompilerRet;
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

	ModuleSourcePath = "src";
	ModuleIntPath = "int";
	ModuleOutPath = "out";
}
String ModuleFile::ToStringBytes(const ModuleIdentifier* Value)
{
	String out;

	out += "AuthorName:" + Value->AuthorName;
	out += "ModuleName:" + Value->ModuleName;

	out += "Version:" + std::to_string(Value->MajorVersion) 
		+ ":" + std::to_string(Value->MinorVersion) 
		+ ":" + std::to_string(Value->RevisionVersion);
	return out;
}
String ModuleFile::ToStringBytes(const Path* Value)
{
	String out =Value->generic_string();


	return out;
}
String ModuleFile::ToStringBytes(const ModuleFile* Lib)
{
	String out;
	out += ToStringBytes(&Lib->ModuleName);

	out += "Src:" + ToStringBytes(&Lib->ModuleSourcePath);
	out += "Obj:" + ToStringBytes(&Lib->ModuleIntPath);
	out += "Out:" + ToStringBytes(&Lib->ModuleOutPath);

	out += "\n";

	for (auto& Item : Lib->ModuleDependencies)
	{
		out += "import ";
		out += Item.AuthorName;
		out += "::" + Item.ModuleName;
		out += + "[" + std::to_string(Item.MajorVersion) 
			+ ':' + std::to_string(Item.MinorVersion)
			+ ':' + std::to_string(Item.RevisionVersion) + "];";
	}
	

	return out;
}

bool ModuleFile::FromString(ModuleFile* Lib, const String_view& Data)
{
	CompliationErrors tep;
	UCodeLang::FrontEnd::Lexer lex;
	lex.Set_ErrorsOutput(&tep);
	lex.Lex(Data);
	

	auto& V = lex.Get_Tokens();

	if (tep.Has_Errors())
	{
		return false;
	}

	{
		ModuleIdentifier Tep;


#define IsGood(x) if (i > V.size()) {return false;}

		for (size_t i = 0; i < V.size(); i++)
		{
			auto& Item = V[i];
			if (Item.Type == TokenType::Name)
			{
				i++; IsGood(i);
				if (V[i].Type == TokenType::Colon)
				{
					i++; IsGood(i);
					if (Item.Value._String == "AuthorName")
					{
						Tep.AuthorName = V[i].Value._String;
					}
					else if (Item.Value._String == "ModuleName")
					{
						Tep.ModuleName = V[i].Value._String;
					}
					else if (Item.Value._String == "ModuleSourcePath")
					{
						Lib->ModuleSourcePath = V[i].Value._String;
					}
					else if (Item.Value._String == "Files")
					{
						Lib->ModuleSourcePath = V[i].Value._String;
					}
					else if (Item.Value._String == "obj")
					{
						Lib->ModuleIntPath = V[i].Value._String;
					}
					else if (Item.Value._String == "out")
					{
						Lib->ModuleOutPath = V[i].Value._String;
					}
					else if (Item.Value._String == "Version")
					{
						Lib->ModuleName = std::move(Tep);
					}
				}

			}
			else if (Item.Type == TokenType::KeyWord_Import)
			{
				i++; IsGood(i);
				if (V[i].Type == TokenType::Name)
				{
					auto& AuthorNameName = V[i];

					i++; IsGood(i);
					if (V[i].Type != TokenType::ScopeResolution)
					{
						return false;
					}

					i++; IsGood(i);
					if (V[i].Type != TokenType::Name)
					{
						return false;
					}	
					auto& ModNameName = V[i];
				
					i++; IsGood(i);
					if (V[i].Type != TokenType::Left_Bracket)
					{
						return false;
					}

					i++; IsGood(i);
					if (V[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num0 = V[i];

					i++;

					i++; IsGood(i);
					if (V[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num1 = V[i];

					i++;

					i++; IsGood(i);
					if (V[i].Type != TokenType::Number_literal)
					{
						return false;
					}
					auto& Num2 = V[i];


					Tep.AuthorName = AuthorNameName.Value._String;
					Tep.ModuleName = ModNameName.Value._String;
					
					Tep.MajorVersion = std::stoi((String)Num0.Value._String);
					Tep.MinorVersion = std::stoi((String)Num1.Value._String);
					Tep.RevisionVersion = std::stoi((String)Num2.Value._String);

					Lib->ModuleDependencies.push_back(std::move(Tep));
				}
			}
		}

	}
	return true;
}

UCodeLangEnd

