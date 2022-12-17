#include "UAssembly_Lexer.hpp"
UAssemblyStart

void Lexer::Reset()
{

}

void Lexer::Lex(const String_view& Text)
{
	size_t OnLine = 0;
	auto _Token = Token();
	for (size_t i = 0; i < Text.size(); i++)
	{
		char Char = Text[i];
		_Token.OnLine = OnLine;
		_Token.OnPos = i;

		if (Char == '\n')
		{
			OnLine++;
			continue;
		}

		if (LexerHelper::IsNameChar(Char))
		{
			size_t StatrBuffer = i;
			for (size_t i2 = i; i2 < Text.size(); i2++)
			{
				char NameChar = Text[i2];
				if (LexerHelper::IsNameChar(NameChar) || LexerHelper::IsDigit(NameChar))
				{
					i++;
				}
				else
				{
					break;
				}
			}
			size_t BufferSize =  i - StatrBuffer;
			String_view Name = Text.substr(StatrBuffer, BufferSize);

			_Token.Type = TokenType::Name;
			_Token.Value = Name;
			Tokens.push_back(_Token);
			_Token = Token();
			i--;
			continue;
		}
		else if (LexerHelper::IsDigit(Char))
		{
			size_t StatrBuffer = i;

			for (size_t i2 = i; i2 < Text.size(); i2++)
			{
				char NameChar = Text[i2];
				if (LexerHelper::IsDigit(NameChar))
				{
					i++;
				}
				else
				{
					break;
				}
			}

			size_t BufferSize = i - StatrBuffer;
			String_view Name = Text.substr(StatrBuffer,BufferSize);

			_Token.Type = TokenType::Number;
			_Token.Value = Name;
			Tokens.push_back(_Token);
			_Token = Token();
			i--; continue;

		}


		switch (Char)
		{

		case ' ':
			break;
		case '\t':
			break;
		case '[':
			_Token.Type = TokenType::Left_Bracket;
			Tokens.push_back(_Token); _Token = Token();
			break;
		case ']':
			_Token.Type = TokenType::Right_Bracket;
			Tokens.push_back(_Token); _Token = Token();
			break;
		case '.':
			_Token.Type = TokenType::Dot;
			Tokens.push_back(_Token); _Token = Token();
			break;
		case '+':
			_Token.Type = TokenType::plus;
			Tokens.push_back(_Token); _Token = Token();
			break;
		case ',':
			_Token.Type = TokenType::Comma;
			Tokens.push_back(_Token); _Token = Token();
			break;
		case '-':
			_Token.Type = TokenType::minus;
			Tokens.push_back(_Token); _Token = Token();
			break;
		default:
			if (_ErrorsOutput)
			{
				auto& Error = _ErrorsOutput->AddError(ErrorCodes::UnknownChar, OnLine, i);

				String A(1, Char);
				Error._Msg = "UnknownChar \'" + A + "\' ";
			};
			break;
		}
	}
}

UAssemblyEnd