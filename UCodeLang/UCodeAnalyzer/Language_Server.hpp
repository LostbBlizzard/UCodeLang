#pragma once
#include "Typedef.hpp"
UCodeAnalyzerStart


class LanguageAnalyzer
{
public:
	LanguageAnalyzer() {};
	~LanguageAnalyzer() {};


private:
};
struct UCFile
{
	Path FullFilePath;
	Path RelativePath;
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

	void Step()
	{

	}

	void Goto(StringView Name)
	{

	}
private:
	
public:
};




UCodeAnalyzerEnd