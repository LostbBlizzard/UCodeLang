#pragma once
#ifndef UCodeLangNoCompiler
#include "../../LangCore.hpp"
#include "../Helpers/CompilerTypes.hpp"
#include "../LexerDefs.h"

#include "UCodeFrontEndNameSpace.hpp"

UCodeLangStart

class CompilationErrors;
struct CompilationSettings;

UCodeLangEnd

UCodeLangFrontStart

class Lexer
{
	enum class ReadingNameState :UInt8
	{
		Name,
		Namespace,
		Class,
		String,
		Number,
		Char
	};
	enum class CommentState : Int8
	{
		NoComment,
		SingleLine,
		MultLine,
	};
public:
	Lexer() {}
	~Lexer() {}
	void Reset();
	void Lex(const String_view& Text);

	UCodeLangForceinline Vector<Token>& Get_Tokens() { return _Tokens; }
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }
	UCodeLangForceinline bool Get_LexerSuccess() { return _LexerSuccess; };
	UCodeLangForceinline size_t Get_OnLine()
	{
		return OnLine;
	}
	UCodeLangForceinline size_t Get_TextIndex()
	{
		return TextIndex;
	}
private:
	String_view _Text;
	Vector<Token> _Tokens;
	bool _LexerSuccess = false;
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;


	Token _Token;
	size_t NameBufferStart = NameBufferNullValue;
	size_t NameBufferEnd = 0;
	static constexpr size_t NameBufferNullValue = -1;
	UCodeLangForceinline size_t NameBufferSize()
	{
		if (NameBufferStart == NameBufferNullValue) { return 0; }
		return NameBufferEnd - NameBufferStart;
	}
	UCodeLangForceinline String_view Get_NameBuffer()
	{
		return _Text.substr(NameBufferStart, NameBufferSize());
	}
	UCodeLangForceinline void ClearNameBuffer()
	{
		NameBufferStart = NameBufferNullValue;
		NameBufferEnd = 0;
	}

	size_t LastIndentationLevel = 0;
	size_t IndentationLevel = 0;
	bool IsIndentationing = true;

	size_t OnLine = 1;
	size_t TextIndex = 0;
	size_t OnLinePos = 0;
	CommentState  CommentState = CommentState::NoComment;
	char NextChar = '\0';
	ReadingNameState ReadingState = ReadingNameState::Name;

	using TokenInedex = size_t;
	Vector<TokenInedex> Indentations;
	bool IsInStringConcatenate = false;


	bool DoIndentation(bool& IsIndentationing, char Char, size_t& IndentationLevel, size_t& LastIndentationLevel, UCodeLang::Token& _Token);



	void NameAndKeyWords(ReadingNameState& ReadingState, Token& _Token);
};
UCodeLangFrontEnd

#endif