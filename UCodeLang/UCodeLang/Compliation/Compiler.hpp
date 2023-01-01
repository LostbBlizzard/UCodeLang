#pragma once
#include "../LangCore.hpp"
#include "Front/Lexer.hpp"
#include "Front/Parser.hpp"
#include "Middle/SystematicAnalysis.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "Back/UCodeBackEnd/UCodeBackEnd.hpp"
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
		CompilerState _State= CompilerState::Null;
		UCodeLang::UClib* OutPut =nullptr;
	};
	struct CompilerPathData
	{
		String FileDir;
		String OutFile;
	};
	
	
	CompilerRet CompileText(const String_view& Text);
	static String GetTextFromFile(const Path& path);
	UCodeLangForceinline CompilerRet CompileFileToLib(const Path& path)
	{
		return  CompileText(GetTextFromFile(path));
	}
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
	UCodeLangForceinline void Set_BackEnd(const BackEndInterface* Value)
	{
		_BackEnd = Value;
	}
	
private:
	CompliationSettings _Settings;
	CompliationErrors _Errors;
	//Front
	Lexer _Lexer;
	Parser _Parser;
	//Middle
	SystematicAnalysis _Analyzer;
	//Back
	const BackEndInterface* _BackEnd = UCodeBackEnd::Get();
};
UCodeLangEnd
