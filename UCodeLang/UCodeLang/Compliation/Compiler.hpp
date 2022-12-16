#pragma once
#include "../LangCore.hpp"
#include "Front/Lexer.hpp"
#include "Front/Parser.hpp"
#include "Front/SemanticAnalysis.hpp"
#include "Front/Optimizer.hpp"
#include "Front/IntermediateAssembler.hpp"
#include "Middle/Linker.hpp"
#include "Middle/Assembler.hpp"
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
	
	
	CompilerRet Compile(const String& Text);
	CompilerRet CompilePath(const String& Path);
	CompilerRet CompilePathToObj(const String& Path, const String& OutLib);
	CompilerRet CompileFiles(const CompilerPathData& Data);
	CompilerRet LinkFiles(const CompilerPathData& Data);
	CompilerRet LinkFilesToFile(const CompilerPathData& Data);
	

	inline void FreeLinkFilesLibOut(UClib* Lib)
	{
		delete Lib;
	}
	inline UCodeLang::CompliationErrors& Get_Errors()
	{
		return _Errors;
	}
	inline void ReMove_Errors()
	{
		_Errors.Remove_Errors();
	}
	inline CompliationSettings& Get_Settings()
	{
		return _Settings;
	}
	
private:
	CompliationSettings _Settings;
	UCodeLang::CompliationErrors _Errors;
	UCodeLang::Lexer _Lexer;
	UCodeLang::Parser _Parser;
	UCodeLang::SemanticAnalysis _SemanticAnalysis;
	UCodeLang::Optimizer _Optimizer;
	UCodeLang::IntermediateAssembler _IntermediateAssembler;
	UCodeLang::Linker _Linker;
	UCodeLang::Assembler _Assembler;
};
UCodeLangEnd
