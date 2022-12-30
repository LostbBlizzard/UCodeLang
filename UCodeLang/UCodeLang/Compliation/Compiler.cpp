#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"
UCodeLangStart
Compiler::CompilerRet Compiler::Compile(const String_view& Text)
{
	_Lexer.Reset();
	_Parser.Reset();
	//
	CompilerRet R;
	R._State = CompilerState::Fail;
	R.OutPut = nullptr;

	_Errors.FilePath = _Errors.FilePath;
	UCodeLang::CompliationErrors* Errors= &_Errors;

	_Lexer.Set_Settings(&_Settings);
	_Parser.Set_Settings(&_Settings);
	
	_Lexer.Set_ErrorsOutput(Errors);
	_Parser.Set_ErrorsOutput(Errors);

	_Lexer.Lex(Text);

	if (Errors->Has_Errors()){ return R; }

	Parser::FileData FileData = {Text,_Errors.FilePath };
	_Parser.Parse(FileData,_Lexer.Get_Tokens());

	Errors->FixEndOfLine(_Lexer.Get_OnLine(), _Lexer.Get_TextIndex());

	if (Errors->Has_Errors()) { return R; }

	R._State = CompilerState::Success;
	R.OutPut = nullptr;
	return R;
}
String Compiler::GetTextFromFile(const Path& path)
{
	std::ifstream File(path);
	if (File.is_open())
	{
		std::string Text;
		std::string line;
		while (std::getline(File, line)){Text += line + '\n';}
		File.close();

		return Text;
	}
	else
	{
		return "";
	}
}
Compiler::CompilerRet Compiler::CompilePathToObj(const Path& path, const Path& OutLib)
{
	auto Text = GetTextFromFile(path);
	CompilerRet r = Compile(Text);

	if (r._State == CompilerState::Success) {
		UClib::ToFile(r.OutPut, OutLib);
	}

	return r;
}
namespace fs = std::filesystem;
Compiler::CompilerRet Compiler::CompileFiles(const CompilerPathData& Data)
{


	//TODO check if File Int  Dir is in Dir

	Vector<FileNode> Files;
	Vector<UClib*> Libs;


	_Lexer.Set_ErrorsOutput(&_Errors);
	_Parser.Set_ErrorsOutput(&_Errors);
	_Lexer.Set_Settings(&_Settings);
	_Parser.Set_Settings(&_Settings);
	
	_Lexer.Reset();
	_Parser.Reset();
	for (const auto& dirEntry : fs::recursive_directory_iterator(Data.FileDir))
	{
		
		if (!dirEntry.is_regular_file()) { continue; }
		
		auto Ext = dirEntry.path().extension();
		if (Ext != FileExt::SourceFileWithDot
		 && Ext != FileExt::LibWithDot
		 && Ext != FileExt::DllWithDot) { continue; }

		String FilePath = dirEntry.path().generic_u8string();

		String RePath = FileHelper::RelativePath(FilePath, Data.FileDir);
		
		if (Ext == FileExt::SourceFileWithDot)
		{
			_Errors.FilePath = RePath;

			auto Text = GetTextFromFile(dirEntry.path());
			_Lexer.Lex(Text);
			Parser::FileData Data;
			if (!_Lexer.Get_LexerSuccess()) { continue; }

			_Parser.Parse(Data, _Lexer.Get_Tokens());
			if (!_Parser.Get_ParseSucces()) { continue; }
			_Errors.FixEndOfLine(_Lexer.Get_OnLine(), _Lexer.Get_TextIndex());

			Files.push_back(_Parser.Get_Tree());

			_Lexer.Reset();
			_Parser.Reset();
		}
		else
		{
			UClib* lib =new UClib();
			if (UClib::FromFile(lib, dirEntry.path()))
			{
				Libs.push_back(lib);
			}
			else
			{
				delete lib;
			}

		}

	}




	
for (auto Item : Libs)
	{
		delete Item;
	}
	CompilerRet r;
	r._State = CompilerState::Success;
	r.OutPut = nullptr;
	return r;
}

UCodeLangEnd