#pragma once
#include "../LangCore.hpp"
#include "Front/Lexer.hpp"
#include "Front/Parser.hpp"
#include "Front/SemanticAnalysis.hpp"
#include "Middle/Linker.hpp"
#include "Middle/Assembler.hpp"
#include "Middle/Optimizer.hpp"
UCodeLangStart
class Compiler
{

public:
	enum class CompilerState : UInt8
	{
		Null,Fail,Success, CompilerError
	};
	struct CompilerRet
	{
		CompilerState _State;
		UCodeLang::UClib* OutPut;
	};
	struct CompilerPathData
	{
		String FileDir;
		String IntDir;
		String OutFile;
	};
	
	
	CompilerRet Compile(const String_view& Text);
	CompilerRet CompilePath(const String& Path);
	CompilerRet CompilePathToObj(const String& Path, const String& OutLib);
	CompilerRet CompileFiles(const CompilerPathData& Data);
	CompilerRet LinkFiles(const CompilerPathData& Data);
	CompilerRet LinkFilesToFile(const CompilerPathData& Data);
	

	UCodeLangForceinline void FreeLinkFilesLibOut(UClib* Lib)
	{
		delete Lib;
	}
	UCodeLangForceinline UCodeLang::CompliationErrors& Get_Errors()
	{
		return _Errors;
	}
	UCodeLangForceinline void ReMove_Errors()
	{
		_Errors.Remove_Errors();
	}
	UCodeLangForceinline CompliationSettings& Get_Settings()
	{
		return _Settings;
	}
	
private:
	CompliationSettings _Settings;
	UCodeLang::CompliationErrors _Errors;
	//Front
	UCodeLang::Lexer _Lexer;
	UCodeLang::Parser _Parser;
	UCodeLang::SemanticAnalysis _SemanticAnalysis;
	//Middle
	UCodeLang::Linker _Linker;
	UCodeLang::Assembler _Assembler;
	UCodeLang::Optimizer _Optimizer;
};
UCodeLangEnd
