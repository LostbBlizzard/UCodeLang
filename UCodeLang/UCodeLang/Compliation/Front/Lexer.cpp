#include "Lexer.hpp"
#include "../Helpers/KeyWords.hpp"
UCodeLangStart
void Lexer::Reset()
{
	_Nodes.clear();
	_Token = Token();
	ClearNameBuffer();

	LastIndentationLevel = 0;
	IndentationLevel = 0;
	IsIndentationing = true;

	OnLine = 1;
	TextIndex = 0;
	OnLinePos = 0;
	CommentState = CommentState::NoComment;
	NextChar = '\n';
	ReadingState = ReadingNameState::Name;
}
void Lexer::Lex(const String_view& Text)
{
#define GetNextChar(offset) Text.size() > (i + offset) ? Text[i + offset] : '\0';
	
	_Text = Text;
	Reset();

	
	for (size_t i = 0; i < Text.size(); i++)
	{
		_Token = Token();
		_Token.OnLine = OnLine;
		_Token.OnPos = i;
		TextIndex = i;
		char Char = Text[i];

		if (CommentState == CommentState::SingleLine)
		{
			if (Char == '\n')
			{
				CommentState = CommentState::NoComment;
				goto NextLine;
			}
			continue;
		}
		else if (CommentState == CommentState::MultLine)
		{
			if (Char == '\n')
			{
				goto NextLine;
			}
			else if (Char == '*')
			{
				NextChar = GetNextChar(1);
				if (NextChar == '/')
				{
					CommentState = CommentState::NoComment;
					i++;
				}
			}
			continue;
		}
		if (Char == '\n')
		{
			NextLine:
			IsIndentationing = true;
			IndentationLevel = 0;
			OnLinePos = 0;
			OnLine++;

			NextChar = GetNextChar(1);
			
		
			NameAndKeyWords(ReadingState, _Token);
			continue;
		}
		else
		{
			OnLinePos++;
		}

		if (DoIndentation(IsIndentationing, Char, IndentationLevel, LastIndentationLevel, _Token))
		{
			continue;
		}
		


		switch (ReadingState)
		{
		case ReadingNameState::String:
			if (Char == '\"')
			{
				NameBufferEnd = i;

				ReadingState = ReadingNameState::Name;
				_Token.Type = TokenType::String_literal;
				_Token.Value = Get_NameBuffer();
				_Nodes.push_back(_Token);
				ClearNameBuffer();
			}
			continue;
			break;
		default:
			break;
		}

	

		if (LexerHelper::IsLetter(Char) || (NameBufferSize() != 0 && LexerHelper::IsNameChar(Char) ))// != 0 for Not geting names as numbers
		{
			if (NameBufferStart == NameBufferNullValue){NameBufferStart = i;}
			continue;
		}
		else if (LexerHelper::IsDigit(Char))
		{
			ReadingState = ReadingNameState::Number;

			if (NameBufferStart == NameBufferNullValue){NameBufferStart = i;}
			continue;
		}
		NameAndKeyWords(ReadingState,_Token);

		

	

		switch (Char)
		{
		case '(':
			_Token.Type = TokenType::Left_Parentheses;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case ')':
			_Token.Type = TokenType::Right_Parentheses;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case '[':
			_Token.Type = TokenType::Left_Bracket;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case ']':
			_Token.Type = TokenType::Right_Bracket;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case ':':
			NextChar = GetNextChar(1);
			if (NextChar == ':')
			{
				_Token.Type = TokenType::ScopeResolution;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else
			{
				_Token.Type = TokenType::Colon;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '.':
			_Token.Type = TokenType::Dot;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case '^':
			_Token.Type = TokenType::bitwise_XOr;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case '~':
			_Token.Type = TokenType::bitwise_not;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case '$':
			ReadingState = ReadingNameState::Class;
			break;
		case '\"':
			ReadingState = ReadingNameState::String;
			NameBufferStart = i+1;
			break;
		case ';':
			_Token.Type = TokenType::Semicolon;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case ',':
			_Token.Type = TokenType::Comma;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case '/':
			NextChar = GetNextChar(1);
			if (NextChar == '*')
			{
				CommentState = CommentState::MultLine;
				i++;
			}
			else if (NextChar == '/')
			{
				CommentState = CommentState::SingleLine;
				i++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundMult;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else
			{
				_Token.Type = TokenType::forwardslash;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '>':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				i++;
				_Token.Type = TokenType::greater_than_or_equalto;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else if (NextChar == '>')
			{
				i++;
				_Token.Type = TokenType::bitwise_RightShift;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::greaterthan;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '<':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				i++;
				_Token.Type = TokenType::less_than_or_equalto;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else if(NextChar == '<')
			{
				i++;
				_Token.Type = TokenType::bitwise_LeftShift;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::lessthan;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '&':
			NextChar = GetNextChar(1);
			if (NextChar == '&')
			{
				i++;
				_Token.Type = TokenType::logical_and;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::bitwise_and;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '|':
			NextChar = GetNextChar(1);
			if (NextChar == '|')
			{
				i++;
				_Token.Type = TokenType::logical_or;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::bitwise_or;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '!':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				i++;
				_Token.Type = TokenType::Notequal_Comparison;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::Not;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '=':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				i++;
				_Token.Type = TokenType::equal_Comparison;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::equal;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '+':
			NextChar = GetNextChar(1);
			if (NextChar == '+')
			{
				_Token.Type = TokenType::increment;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundAdd;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else
			{
				_Token.Type = TokenType::plus;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '-':
			NextChar = GetNextChar(1);
			if (NextChar == '>')
			{
				_Token.Type = TokenType::RightArrow;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else if (NextChar == '-')
			{
				_Token.Type = TokenType::decrement;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundSub;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else
			{
				_Token.Type = TokenType::minus;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '*':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundMult;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
				i++;
			}
			else
			{
				_Token.Type = TokenType::star;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;		
		case '%':
			NextChar = GetNextChar(1);
			if (LexerHelper::IsLetter(NextChar))
			{
				ReadingState = ReadingNameState::Namespace;
			}
			else
			{
				_Token.Type = TokenType::modulo;
				_Token.Value = nullptr;
				_Nodes.push_back(_Token);
			}
			break;
		case '#':
			_Token.Type = TokenType::hash;
			_Token.Value = nullptr;
			_Nodes.push_back(_Token);
			break;
		case ' ':
			break;
		case '\t':
			//break;
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
	
	IndentationLevel = 0;
	DoIndentation(IsIndentationing,'\0', IndentationLevel, LastIndentationLevel, _Token);

	if (CommentState == CommentState::NoComment) 
	{
		NameAndKeyWords(ReadingState, _Token);
	}
	else if (CommentState == CommentState::MultLine)
	{
		if (_ErrorsOutput)
		{
			auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, OnLine, Text.size());

			Error._Msg = "ExpectingSequence \' */ \' To End MultLine Comment";
		};
	}
}
bool Lexer::DoIndentation(bool& IsIndentationing, char Char, size_t& IndentationLevel, size_t& LastIndentationLevel, UCodeLang::Token& _Token)
{
	if (IsIndentationing)
	{
		if (Char == ' ')
		{
			IndentationLevel++;
			return true;
		}
		//else if (Char == '\t')
		//{
			//IndentationLevel += 4;
			//return true;
		//}
		else
		{
			IsIndentationing = false;
			if (LastIndentationLevel != IndentationLevel)
			{
				if (LastIndentationLevel < IndentationLevel)
				{
					_Token.Type = TokenType::StartTab;
					_Token.Value = IndentationLevel;
					_Nodes.push_back(_Token);
					
					_Token = Token();
					_Token.OnLine = OnLine;
					_Token.OnPos = TextIndex;
					LastIndentationLevel = IndentationLevel;

					Indentations.push_back(_Nodes.size() - 1);
				}
				else
				{

					for (auto it = Indentations.rbegin(); it != Indentations.rend();)
					{
						auto Item_ = *it;
						Token& Item = _Nodes[Item_];
						size_t IndentationLvl = Item.Value._Size_t;
						if (IndentationLvl > IndentationLevel)
						{
							_Token.Type = TokenType::EndTab;
							_Token.Value = nullptr;
							_Nodes.push_back(_Token);
							_Token = Token();
							_Token.OnLine = OnLine;
							_Token.OnPos = TextIndex;
						
							it = std::reverse_iterator(Indentations.erase(std::next(it).base()));
							LastIndentationLevel = IndentationLevel;
							continue;
						}
						++it;	
					}


				}
			}
		}
	}
	return false;
}
void Lexer::NameAndKeyWords(ReadingNameState& ReadingState, Token& _Token)
{
	
	if (NameBufferStart != NameBufferNullValue)
	{
		NameBufferEnd = TextIndex;
		auto NameBuffer = Get_NameBuffer();
		auto KeyWord = KeyWordHelper::GetToken(NameBuffer);
		if (KeyWord != TokenType::Null)
		{
			_Token.Type = KeyWord;
		}
		else
		{
			TokenType Type;
			switch (ReadingState)
			{
			case ReadingNameState::Name:
				Type = TokenType::Name;
				break;
			case ReadingNameState::Class:
				Type = TokenType::Class;
				break;
			case ReadingNameState::Namespace:
				Type = TokenType::Namespace;
				break;
			case ReadingNameState::Number:
				Type = TokenType::Number_literal;
				break;
			default:
				Type = TokenType::Null;
				break;
			}
			_Token.Type = Type;
			_Token.Value = NameBuffer;
			ReadingState = ReadingNameState::Name;
		}



		_Nodes.push_back(_Token);
		ClearNameBuffer();
		_Token = Token();
		_Token.OnLine = OnLine;
		_Token.OnPos = TextIndex;
	}
}
UCodeLangEnd