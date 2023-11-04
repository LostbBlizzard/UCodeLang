#pragma once

#include "UCodeLang/LangCore/UClib.hpp"
#include "UAssembly_NameSpace.hpp"
#include "UCodeLang/Compilation/LexerDefs.h"
#include "CompilerTypes.hpp"
#include "../Helpers/CompilationErrors.hpp"
#include "../CompilationSettings.hpp"
UAssemblyStart

class Lexer
{
public:
	Lexer() {}
	~Lexer() {}
	void Reset();
	void Lex(const String_view& Text);
	UCodeLangForceinline auto& Get_Output() { return Tokens; }
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }
private:
	Vector<Token> Tokens;
	bool _LexerSuccess = false;
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
};
UAssemblyEnd