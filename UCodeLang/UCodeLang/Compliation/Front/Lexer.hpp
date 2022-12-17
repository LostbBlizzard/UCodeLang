#pragma once

#include "../../LangCore.hpp"
#include "../Helpers/CompilerTypes.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"


#define UpLettersCharSet "QWERTYUIOPASDFGHJKLZXCVBNM"
#define LowLettersCharSet "qwertyuiopasdfghjklzxcvbnm"
#define LettersCharSet UpLettersCharSet LowLettersCharSet
#define digitCharSet "0123456789"
#define NameCharSet LettersCharSet digitCharSet "_"


UCodeLangStart
class Lexer
{
	enum class ReadingNameState :UInt8
	{
		Name,
		Namespace,
		Class,
		String,
		Number,
	};
	enum class CommentState : Int8
	{
		NoComment,
		SingleLine,
		MultLine,
	};
public:
	static constexpr const char* const Letters = LettersCharSet;
	static constexpr size_t Letters_Size = sizeof(LettersCharSet);
	
	static constexpr const char* const Digits = digitCharSet;
	static constexpr size_t Digits_Size = sizeof(digitCharSet);

	static constexpr const char* const NameChars = NameCharSet;
	static constexpr size_t NameChars_Size = sizeof(NameCharSet);

	Lexer(){}
	~Lexer(){}
	void Reset();
	void Lex(const String_view& Text);

	UCodeLangForceinline Vector<Token>& Get_Tokens() { return _Nodes; }
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
	UCodeLangForceinline bool Get_LexerSuccess() {return _LexerSuccess;};
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
	Vector<Token> _Nodes;
	bool _LexerSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	
	
	Token _Token;
	size_t NameBufferStart=0;
	size_t NameBufferEnd=0;
	static constexpr size_t NameBufferNullValue = -1;
	UCodeLangForceinline size_t NameBufferSize()
	{
		return NameBufferEnd- NameBufferStart;
	}
	UCodeLangForceinline String_view Get_NameBuffer()
	{
		intptr_t V = (intptr_t)_Text.data() + (intptr_t)NameBufferStart;
		const char* Charptr = (const char*)V;
		return String_view(Charptr, NameBufferSize());
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
	char NextChar ='\0';
	ReadingNameState ReadingState = ReadingNameState::Name;

	using TokenInedex = size_t;
	Vector<TokenInedex> Indentations;
	
	
	bool DoIndentation(bool& IsIndentationing, char Char, size_t& IndentationLevel, size_t& LastIndentationLevel, UCodeLang::Token& _Token);

	
	static bool IsNameChar(char Char)
	{
		for (size_t i = 0; i < NameChars_Size; i++)
		{
			if (Char == NameChars[i]) { return true; }
		}
		return false;
	}
	static bool IsLetter(char Char)
	{
		for (size_t i = 0; i < Letters_Size; i++)
		{
			if (Char == Letters[i]) { return true; }
		}
		return false;
	}
	static bool IsDigit(char Char)
	{
		for (size_t i = 0; i < Digits_Size; i++)
		{
			if (Char == Digits[i]) { return true; }
		}
		return false;
	}
	void NameAndKeyWords(ReadingNameState& ReadingState, Token& _Token);
};
UCodeLangEnd

