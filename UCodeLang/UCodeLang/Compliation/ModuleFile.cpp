#include "ModuleFile.hpp"
#include <sstream> 
#include <fstream>
#include "UCodeLang/Compliation/Front/Lexer.hpp"
#include <filesystem>
UCodeLangStart

Compiler::CompilerRet ModuleFile::BuildModule(Compiler& Compiler)
{
	Compiler::CompilerPathData paths;

	auto B = Path(ThisModuleDir);

	paths.FileDir = Path((B.native() + Path::preferred_separator + Path(ModuleSourcePath).native())).generic_string();
	paths.IntDir = Path((B.native() + Path::preferred_separator + Path(ModuleIntPath).native() )).generic_string();

	paths.OutFile = Path((B.native() + Path(ModuleOutPath).native())).generic_string();
return Compiler.CompileFiles_UseIntDir(paths);
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
		+ "." + std::to_string(Value->MinorVersion) 
		+ "." + std::to_string(Value->RevisionVersion);
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

	out += "Files:" + ToStringBytes(&Lib->ModuleSourcePath);
	out += "obj:" + ToStringBytes(&Lib->ModuleIntPath);
	out += "out:" + ToStringBytes(&Lib->ModuleOutPath);

	out += "Dependencies: [\n";
	for (auto& Item : Lib->ModuleDependencies)
	{
		out += "[\n";
		out += ToStringBytes(&Item);
		out += "\n]";
	}
	out += "]";

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
		enum class FieldMode
		{
			ModuleName,
			Dependencies,
		};

		FieldMode mode = FieldMode::ModuleName;

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

						//
						if (mode == FieldMode::ModuleName)
						{
							Lib->ModuleName = std::move(Tep);
						}
						else
						{
							Lib->ModuleDependencies.push_back(std::move(Tep));
						}
					}
				}

			}
		}
	}
	return true;
}

UCodeLangEnd

