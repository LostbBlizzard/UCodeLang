#pragma once
#include "Typedef.hpp"
#include "UCodeLang/Compilation/Front/Lexer.hpp"
#include "UCodeLang/Compilation/Front/Parser.hpp"
UCodeAnalyzerStart


#define UCodeLangLanguageId ""

using Fileidentifier = Path::string_type;

class LanguageAnalyzer
{
public:
	LanguageAnalyzer() {};
	~LanguageAnalyzer() {};


private:
};
struct UCFile
{
	UCFile()
	{

	}
	~UCFile()
	{

	}
	UCFile(UCFile&& source) = default;
	UCFile& operator=(UCFile&& source) = default;

	UCFile(const UCFile& source) = default;
	UCFile& operator=(const UCFile& source) = default;


	//not an Real file path just for errors
	Path FileName; 
	Fileidentifier _Fileidentifier;
	String filetext;

	//Call this ather updateing filetext
	void UpdatedFileText()
	{
		oldfile = filetext;
	}


	UCodeLang::CompilationSettings* _CompilationSettings = nullptr;

	String oldfile;

	
	UCodeLang::Vector<UCodeLang::CompilationErrors::Error> lexingErrors;
	bool lexingpassing = false;
	UCodeLang::Vector<UCodeLang::CompilationErrors::Error> parseErrors;
	bool parsepassing = false;

	//May return null if has Errors.
	UCodeLang::FileNode* Get_Tree()
	{
		if (analyzedPaseTree.get())
		{
			return analyzedPaseTree.get();
		}
		return nullptr;
	}
	
	void Step()
	{

		AnalyzedParseAndLex();

	}
private:
	String analyzedlex;
	String analyzedpaser;
	UCodeLang::CompilationErrors _CompilationErrors;
	UCodeLang::FrontEnd::Lexer _oldLexer;
	UCodeLang::Vector<UCodeLang::Token> parsertokens;

	UCodeLang::Vector<UCodeLang::Token> analyzedtokens;

	//Shared for this to be copy able for type Vector
	Shared_ptr<UCodeLang::FrontEnd::Parser> _oldParser;

	//Shared for this to be copy able for type Vector
	Shared_ptr<UCodeLang::FileNode> analyzedPaseTree;

	void AnalyzedParseAndLex();
};

struct GoToItem
{
	Path OnRelativeFilePath;
};

class LanguageServer
{
public:
	LanguageServer(){}
	~LanguageServer(){}

	void init();

	void Step();

	void deinit();

	void Goto(StringView Name)
	{

	}

	void AddFile(UCFile&& file)
	{
		file._CompilationSettings = &Settings;
		_Files.AddValue(file._Fileidentifier, file);
	}
	void RemoveFile(const Fileidentifier& file)
	{
		_Files.erase(file);
	}
	UCFile& GetFile(const Fileidentifier& file)
	{
		return _Files.GetValue(file);
	}
	UnorderedMap<Fileidentifier, UCFile>& GetFiles()
	{
		return _Files;
	}

	UCodeLang::Vector<UCodeLang::CompilationErrors::Error> ErrorList;
private:
	UnorderedMap<Fileidentifier, UCFile> _Files;

	size_t OldErrorListCount = 0;
	UCodeLang::CompilationSettings Settings;
	void UpdateErrorList();
};




UCodeAnalyzerEnd