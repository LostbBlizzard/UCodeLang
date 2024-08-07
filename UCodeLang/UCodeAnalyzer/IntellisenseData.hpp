#pragma once

#ifndef UCodeLangNoCompiler
#include "Typedef.hpp"

#include "UCodeLang/Compilation/Front/Lexer.hpp"
#include "UCodeLang/Compilation/Front/Parser.hpp"
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeAnalyzerStart
class IntellisenseData
{
public:
	IntellisenseData(){}
	~IntellisenseData(){}

	auto& Get_Lexer(){return _Lexer;}
	auto& Get_Parser() { return _Parser; }
	auto& Get_SystematicAnalysis() { return _SystematicAnalysis; }
	auto& Get_Errors() { return _Errors; }
	auto& Get_Settings() { return _Settings; }
private:

	UCodeLang::CompilationErrors _Errors;
	UCodeLang::CompilationSettings _Settings;

	UCodeLang::FrontEnd::Lexer _Lexer;
	UCodeLang::FrontEnd::Parser _Parser;
	UCodeLang::FrontEnd::SystematicAnalysis _SystematicAnalysis;
	
};
UCodeAnalyzerEnd

#endif