#include "LanguageServer.hpp"
UCodeAnalyzerStart


void LanguageServer::init()
{

}

void LanguageServer::Step()
{
	UpdateErrorList();
	for (auto& Item : _Files)
	{
		Item._Value.Step();
	}
}

void LanguageServer::deinit()
{

}
void LanguageServer::UpdateErrorList()
{
	ErrorList.clear();
	for (auto& Item : _Files)//this is a bit slow should be cashed.
	{
		for (auto& Error : Item._Value.lexingErrors)
		{
			ErrorList.push_back(Error);
		}
		for (auto& Error : Item._Value.parseErrors)
		{
			ErrorList.push_back(Error);
		}
	}
}

void UCFile::AnalyzedParseAndLex()
{
	if (oldfile == analyzedlex) { return; }
	analyzedlex = oldfile;

	_oldLexer.Reset();
	_CompliationErrors.FilePath = this->FileName;

	_oldLexer.Set_Settings(_CompliationSettings);
	_oldLexer.Set_ErrorsOutput(&_CompliationErrors);


	_oldLexer.Lex(oldfile);


	lexingpassing = !_CompliationErrors.Has_Errors();
	lexingErrors = std::move(_CompliationErrors.Get_Errors());
	if (lexingpassing)
	{
		String old = std::move(analyzedpaser);
		
		analyzedpaser = std::move(analyzedlex);
		analyzedlex = analyzedpaser;//keep copy
		
		parsertokens = std::move(_oldLexer.Get_Tokens());

		_oldParser = std::make_shared<UCodeLang::FrontEnd::Parser>();
		_oldParser->Reset();
		_oldParser->Set_Settings(_CompliationSettings);
		_oldParser->Set_ErrorsOutput(&_CompliationErrors);

		_oldParser->Parse(parsertokens);

		parsepassing = !_CompliationErrors.Has_Errors();
		parseErrors = std::move(_CompliationErrors.Get_Errors());
		if (parsepassing) 
		{
			analyzedtokens = std::move(parsertokens);
			analyzedPaseTree = std::make_shared<UCodeLang::FileNode>(std::move(_oldParser->Get_Tree()));
		}
		else
		{
			analyzedpaser = std::move(old);
		}
	}
}


UCodeAnalyzerEnd

