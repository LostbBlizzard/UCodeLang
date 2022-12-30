#pragma once
#include "../LangCore.hpp"
#include "Front/Lexer.hpp"
#include "Front/Parser.hpp"


#include "UCodeLang/LangCore/UClib.hpp"
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
		UCodeLang::UClib* OutPut =nullptr;
	};
	struct CompilerPathData
	{
		String FileDir;
		String OutFile;
	};
	
	
	CompilerRet Compile(const String_view& Text);
	static String GetTextFromFile(const Path& path);
	CompilerRet CompilePathToObj(const Path& path, const Path& OutLib);
	CompilerRet CompileFiles(const CompilerPathData& Data);
	
	

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
	//Middle

};
UCodeLangEnd
