#pragma once

#define UpLettersCharSet "QWERTYUIOPASDFGHJKLZXCVBNM"
#define LowLettersCharSet "qwertyuiopasdfghjklzxcvbnm"
#define LettersCharSet UpLettersCharSet LowLettersCharSet
#define digitCharSet "0123456789"
#define NameCharSet LettersCharSet digitCharSet "_"

struct LexerHelper
{
	static constexpr const char* const Letters = LettersCharSet;
	static constexpr size_t Letters_Size = sizeof(LettersCharSet);

	static constexpr const char* const Digits = digitCharSet;
	static constexpr size_t Digits_Size = sizeof(digitCharSet);

	static constexpr const char* const NameChars = NameCharSet;
	static constexpr size_t NameChars_Size = sizeof(NameCharSet);
	static bool IsNameChar(char Char)
	{
		for (size_t i = 0; i < LexerHelper::NameChars_Size; i++)
		{
			if (Char == LexerHelper::NameChars[i]) { return true; }
		}
		return false;
	}
	static bool IsLetter(char Char)
	{
		for (size_t i = 0; i < LexerHelper::Letters_Size; i++)
		{
			if (Char == LexerHelper::Letters[i]) { return true; }
		}
		return false;
	}
	static bool IsDigit(char Char)
	{
		for (size_t i = 0; i < LexerHelper::Digits_Size; i++)
		{
			if (Char == LexerHelper::Digits[i]) { return true; }
		}
		return false;
	}
};