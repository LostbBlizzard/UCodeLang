#include "SyntaxHighlighter.hpp"
UCodeAnalyzerStart

using UToken = UCodeLang::Token;
using UTokenType = UCodeLang::TokenType;
bool IsKeyWord(UTokenType T)
{
	return false;
}

void SyntaxHighlighter::Parse(StringView Text)
{
	if (_Data == nullptr) { return; }
	auto& Lexer = _Data->Get_Lexer();

	_Tokens.clear();
	

	Lexer.Lex(Text);
	const auto& Output = Lexer.Get_Tokens();

	for (auto& Item : Output)
	{
		
	}
}
UCodeAnalyzerEnd
