#pragma once

#include "UCodeLang/LangCore/UClib.hpp"
#include "UAssembly_NameSpace.hpp"
#include "UCodeLang/Compliation/LexerDefs.h"
#include "CompilerTypes.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
UAssemblyStart

class Lexer
{
public:
	Lexer() {}
	~Lexer() {}
	void Reset();
	void Lex(const String_view& Text);
	UCodeLangForceinline auto& Get_Output() { return Tokens; }
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	Vector<Token> Tokens;
	bool _LexerSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
};
UAssemblyEnd