#pragma once
#include "Typedef.hpp"
UCodeAnalyzerStart


#define UCodeLangLanguageId ""

using Fileidentifier = Path;

class LanguageAnalyzer
{
public:
	LanguageAnalyzer() {};
	~LanguageAnalyzer() {};


private:
};
struct UCFile
{
	//not an Real file path just for errors
	Path FileName; 
	Fileidentifier Fileidentifier;
	String filetext;

	//Call this befor updateing filetext
	void IsUpdateingFile()
	{
		oldfile = filetext;
	}

	
	String oldfile;
};

struct GoToItem
{
	Path OnRelativeFilePath;
};

class Language_Server
{
public:
	Language_Server(){}
	~Language_Server(){}

	void init();

	void Step();

	void deinit();

	void Goto(StringView Name)
	{

	}

	void AddFile(UCFile&& file)
	{
		_Files.AddValue(file.Fileidentifier, file);
	}
	void RemoveFile(const Fileidentifier& file)
	{
		_Files.erase(file);
	}
	UCFile& GetFile(const Fileidentifier& file)
	{
		return _Files.at(file);
	}
private:
	BinaryVectorMap<Fileidentifier, UCFile> _Files;
};




UCodeAnalyzerEnd