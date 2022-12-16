#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"
UCodeLangStart
Compiler::CompilerRet Compiler::Compile(const String& Text)
{
	_Lexer.Reset();
	_Parser.Reset();
	_SemanticAnalysis.Reset();
	//
	CompilerRet R;
	R._State = CompilerState::Fail;
	R.OutPut = nullptr;

	UCodeLang::CompliationErrors* Errors= &_Errors;

	_Lexer.Set_Settings(&_Settings);
	_Parser.Set_Settings(&_Settings);
	_SemanticAnalysis.Set_Settings(&_Settings);
	

	_Lexer.Set_ErrorsOutput(Errors);
	_Parser.Set_ErrorsOutput(Errors);
	_SemanticAnalysis.Set_ErrorsOutput(Errors);
	


	_Lexer.Lex(Text);

	if (Errors->Has_Errors()){ return R; }

	_Parser.Parse(_Lexer.Get_Tokens());

	Errors->FixEndOfLine(_Lexer.Get_OnLine(), _Lexer.Get_TextIndex());

	if (Errors->Has_Errors()) { return R; }

	_SemanticAnalysis.DoAnalysis(_Parser.Get_Tree());

	if (Errors->Has_Errors()) { return R; }
	
	

	R._State = CompilerState::Success;
	R.OutPut = &_SemanticAnalysis.Get_SemanticAnalysisRet().Lib;
	return R;
}
Compiler::CompilerRet Compiler::CompilePath(const String& Path)
{
	std::ifstream File(Path);
	if (File.is_open())
	{
		std::string Text;
		std::string line;
		while (std::getline(File, line)){Text += line + '\n';}
		File.close();

		return Compile(Text);
	}
	else
	{
		CompilerRet Data;
		Data._State = CompilerState::Fail;
		return Data;
	}
}
Compiler::CompilerRet Compiler::CompilePathToObj(const String& Path, const String& OutLib)
{
	CompilerRet r = CompilePath(Path);//Cashing Stuff

	if (r._State == CompilerState::Success) {
		UClib::ToFile(r.OutPut, OutLib);
	}

	return r;
}
namespace fs = std::filesystem;
Compiler::CompilerRet Compiler::CompileFiles(const CompilerPathData& Data)
{


	//TODO check if File Int  Dir is in Dir
	for (const auto& dirEntry : fs::recursive_directory_iterator(Data.FileDir))
	{
		if (!dirEntry.is_regular_file()) { continue; }
		
		auto Ext = dirEntry.path().extension();
		if (Ext != FileExt::SourceFileWithDot
		 && Ext != FileExt::IntLibWithDot
		 && Ext != FileExt::LibWithDot
		 && Ext != FileExt::DllWithDot) { continue; }

		String FilePath = dirEntry.path().generic_u8string();

		String RePath = FileHelper::RelativePath(FilePath, Data.FileDir);
		String OutPath = Data.IntDir + RePath;
		
		if (Ext == FileExt::SourceFileWithDot) 
		{
			FileHelper::ChangeExt(OutPath, FileExt::IntLibWithDot);

			String OutPathDir = fs::path(OutPath).parent_path().generic_u8string();

			if (!fs::exists(OutPathDir)) { fs::create_directories(OutPathDir); }

			_Errors.FilePath = RePath;
			CompilePathToObj(FilePath, OutPath);
		}
		else
		{

			String OutPathDir = fs::path(OutPath).parent_path().generic_u8string();

			if (!fs::exists(OutPathDir)) { fs::create_directories(OutPathDir); }

			if (!fs::exists(OutPath))//TODO Copyfile if diff
			{
				fs::copy_file(dirEntry.path(), OutPath);
			}
		}

	}

	CompilerRet r;
	r._State = CompilerState::Success;
	r.OutPut = nullptr;
	return r;
}
Compiler::CompilerRet Compiler::LinkFiles(const CompilerPathData& Data)
{
	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;

	UCodeLang::CompliationErrors* Errors = &_Errors;
	_Linker.Set_ErrorsOutput(Errors);

	Vector<const UClib*> List;
	
	if (!fs::exists(Data.IntDir)) { fs::create_directories(Data.IntDir); }
	for (const auto& dirEntry : fs::recursive_directory_iterator(Data.IntDir))
	{
		if (!dirEntry.is_regular_file()) { continue; }

		auto Ext = dirEntry.path().extension();
		if (Ext != FileExt::IntLibWithDot
			&& Ext != FileExt::LibWithDot
			&& Ext != FileExt::DllWithDot) 
		{
			continue;
		}


		String FilePath = dirEntry.path().generic_u8string();
		auto NewPtr = new UClib();
		if (UClib::FromFile(NewPtr, FilePath))
		{
			List.push_back(NewPtr);
		}

	}

	UClib linkLib = UClib();
	_Linker.Set_ErrorsOutput(&_Errors);
	_Linker.Set_Settings(&_Settings);
	_Assembler.Set_ErrorsOutput(&_Errors);
	_Assembler.Set_Settings(&_Settings);
	_Linker.Link(List, &linkLib);
	
	for (auto Item : List)
	{
		delete Item;
	}
	if (Errors->Has_Errors()) { return r; }

	UClib* outLib =new UClib();
	_Assembler.Assemble(outLib,&linkLib);
	
	if (Errors->Has_Errors()) { delete outLib; return r; }

	r._State = CompilerState::Success;
	r.OutPut = outLib;
	return r;
}
Compiler::CompilerRet Compiler::LinkFilesToFile(const CompilerPathData& Data)
{
	auto r = Compiler::LinkFiles(Data);

	if (r._State == CompilerState::Success)
	{
		UClib::ToFile(r.OutPut, Data.OutFile);

	}
	FreeLinkFilesLibOut(r.OutPut);
	r.OutPut = nullptr;
	return r;
}

UCodeLangEnd