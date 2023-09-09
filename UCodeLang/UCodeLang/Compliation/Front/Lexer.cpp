#include "Lexer.hpp"
#include "../Helpers/KeyWords.hpp"
UCodeLangFrontStart
void Lexer::Reset()
{
	auto ErrorsOutput = _ErrorsOutput;
	auto Settings = _Settings;

	this->~Lexer();
	new (this)Lexer;
	
	this->_ErrorsOutput = ErrorsOutput;
	this->_Settings = Settings;
}
void Lexer::Lex(const String_view& Text)
{
#define GetNextChar(offset) Text.size() > (TextIndex  + offset) ? Text[TextIndex  + offset] : '\0';
	Reset();
	_Text = Text;
	

	
	for (TextIndex = 0; TextIndex < Text.size(); TextIndex++)
	{
		_Token = Token();
		_Token.OnLine = OnLine;
		_Token.OnPos = TextIndex;
		char Char = Text[TextIndex];

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
					TextIndex++;
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
				NameBufferEnd = TextIndex;

				ReadingState = ReadingNameState::Name;
				_Token.Type = TokenType::String_literal;
				_Token.Value = Get_NameBuffer();
				_Tokens.push_back(_Token);

				ClearNameBuffer();
			}
			continue;
		case ReadingNameState::Char:
			if (Char == '\'')
			{
				NameBufferEnd = TextIndex;

				ReadingState = ReadingNameState::Name;
				_Token.Type = TokenType::Char_literal;

				_Token.Value = Get_NameBuffer();
				_Tokens.push_back(_Token);

				ClearNameBuffer();
			}
			continue;
		default:
			break;
		}

	

		if (LexerHelper::IsNondigitName(Char) || (NameBufferSize() != 0 && LexerHelper::IsNameChar(Char) ))// != 0 for Not geting names as numbers
		{
			if (NameBufferStart == NameBufferNullValue){NameBufferStart = TextIndex;}
			continue;
		}
		else if (LexerHelper::IsDigit(Char))
		{
			ReadingState = ReadingNameState::Number;

			if (NameBufferStart == NameBufferNullValue){NameBufferStart = TextIndex;}
			continue;
		}
		auto OldNodesCount = _Tokens.size();
		NameAndKeyWords(ReadingState,_Token);

		//if added token
		if (OldNodesCount != _Tokens.size()){continue;}

	

		switch (Char)
		{
		case '(':
			_Token.Type = TokenType::Left_Parentheses;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case ')':
			_Token.Type = TokenType::Right_Parentheses;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case '[':
			_Token.Type = TokenType::Left_Bracket;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case ']':
			_Token.Type = TokenType::Right_Bracket;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case '{':
			_Token.Type = TokenType::Left_Brace;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case '}':
			_Token.Type = TokenType::Right_Brace;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case ':':
			NextChar = GetNextChar(1);
			if (NextChar == ':')
			{
				_Token.Type = TokenType::ScopeResolution;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else
			{
				_Token.Type = TokenType::Colon;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '.':
		{
			NextChar = GetNextChar(1);
			bool IsDot = true;
			if (NextChar == '.')
			{
				NextChar = GetNextChar(2);
				if (NextChar == '.')
				{
					IsDot = false;
					_Token.Type = TokenType::ellipses;
					_Token.Value = nullptr;
					_Tokens.push_back(_Token);
					TextIndex+=2;
				}
				else
				{
					IsDot = false;
					_Token.Type = TokenType::KeyWord_RangeOperator;
					_Token.Value = nullptr;
					_Tokens.push_back(_Token);
					TextIndex += 1;
				}
			}

			if (IsDot) 
			{
				_Token.Type = TokenType::Dot;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
		}
		break;
		case '^':
			_Token.Type = TokenType::bitwise_XOr;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case '~':
			NextChar = GetNextChar(1);
			if (NextChar == '=') 
			{
				TextIndex++;
				_Token.Type = TokenType::approximate_Comparison;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '>')
			{
				TextIndex++;
				_Token.Type = TokenType::IndirectMember;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::bitwise_not;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '$':
			NextChar = GetNextChar(1);
			if (LexerHelper::IsNondigitName(NextChar))
			{
				ReadingState = ReadingNameState::Class;
			}
			else
			{
				_Token.Type = TokenType::DollarSign;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '\"':
			ReadingState = ReadingNameState::String;
			NameBufferStart = TextIndex +1;
			break;
		case '\'':
			ReadingState = ReadingNameState::Char;
			NameBufferStart = TextIndex + 1;
			break;
		case ';':
			_Token.Type = TokenType::Semicolon;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case ',':
			_Token.Type = TokenType::Comma;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case '/':
			NextChar = GetNextChar(1);
			if (NextChar == '*')
			{
				CommentState = CommentState::MultLine;
				TextIndex++;
			}
			else if (NextChar == '/')
			{
				CommentState = CommentState::SingleLine;
				TextIndex++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundMult;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else
			{
				_Token.Type = TokenType::forwardslash;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '>':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				TextIndex++;
				_Token.Type = TokenType::greater_than_or_equalto;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '>')
			{
				TextIndex++;
				_Token.Type = TokenType::bitwise_RightShift;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::greaterthan;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '<':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				TextIndex++;
				_Token.Type = TokenType::less_than_or_equalto;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if(NextChar == '<')
			{
				TextIndex++;
				_Token.Type = TokenType::bitwise_LeftShift;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '-')
			{
				TextIndex++;
				_Token.Type = TokenType::leftArrow;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::lessthan;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '&':
			NextChar = GetNextChar(1);
			if (NextChar == '&')
			{
				TextIndex++;
				_Token.Type = TokenType::logical_and;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::bitwise_and;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '|':
			NextChar = GetNextChar(1);
			if (NextChar == '|')
			{
				TextIndex++;
				_Token.Type = TokenType::logical_or;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::bitwise_or;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '!':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				TextIndex++;
				_Token.Type = TokenType::Notequal_Comparison;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '.')
			{
				TextIndex++;
				_Token.Type = TokenType::ExclamationDot;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::Not;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '?':
			NextChar = GetNextChar(1);
			if (NextChar == '.')
			{
				TextIndex++;
				_Token.Type = TokenType::OptionalDot;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::QuestionMark;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '=':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				TextIndex++;
				_Token.Type = TokenType::equal_Comparison;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '>')
			{
				TextIndex++;
				_Token.Type = TokenType::RightAssignArrow;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else
			{
				_Token.Type = TokenType::equal;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '+':
			NextChar = GetNextChar(1);
			if (NextChar == '+')
			{
				_Token.Type = TokenType::increment;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundAdd;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else
			{
				_Token.Type = TokenType::plus;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '-':
			NextChar = GetNextChar(1);
			if (NextChar == '>')
			{
				TextIndex++;

				_Token.Type = TokenType::RightArrow;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			else if (NextChar == '-')
			{
				_Token.Type = TokenType::decrement;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundSub;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else if (LexerHelper::IsDigit(NextChar))
			{
				ReadingState = ReadingNameState::Number;
				if (NameBufferStart == NameBufferNullValue) { NameBufferStart = TextIndex; }
			}
			else
			{
				_Token.Type = TokenType::minus;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
			}
			break;
		case '*':
			NextChar = GetNextChar(1);
			if (NextChar == '=')
			{
				_Token.Type = TokenType::CompoundMult;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
				TextIndex++;
			}
			else
			{
				_Token.Type = TokenType::star;
				_Token.Value = nullptr;
				_Tokens.push_back(_Token);
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
				_Tokens.push_back(_Token);
			}
			break;
		case '#':
			_Token.Type = TokenType::hash;
			_Token.Value = nullptr;
			_Tokens.push_back(_Token);
			break;
		case ' ':
			break;
		case '\t':
			//break;
		default:
			if (_ErrorsOutput)
			{
				auto& Error = _ErrorsOutput->AddError(ErrorCodes::UnknownChar, OnLine, TextIndex);

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


	_Token = Token();
	_Token.Type = TokenType::EndofFile;
	_Token.OnLine = OnLine;
	_Token.OnPos = TextIndex;
	_Tokens.push_back(_Token);

	_LexerSuccess = !_ErrorsOutput->Has_Errors();
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
					_Tokens.push_back(_Token);
					
					_Token = Token();
					_Token.OnLine = OnLine;
					_Token.OnPos = TextIndex;
					LastIndentationLevel = IndentationLevel;

					Indentations.push_back(_Tokens.size() - 1);
				}
				else
				{

					for (auto it = Indentations.rbegin(); it != Indentations.rend();)
					{
						auto Item_ = *it;
						Token& Item = _Tokens[Item_];
						size_t IndentationLvl = Item.Value._Size_t;
						if (IndentationLvl > IndentationLevel)
						{
							_Token.Type = TokenType::EndTab;
							_Token.Value = nullptr;
							_Tokens.push_back(_Token);
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
			if (ReadingState == ReadingNameState::Class)
			{
				if (KeyWord == TokenType::KeyWord_If)
				{
					KeyWord = TokenType::KeyWord_ClassIf;
					ReadingState = ReadingNameState::Name;
				}
				if (KeyWord == TokenType::KeyWord_Else)
				{
					KeyWord = TokenType::KeyWord_ClassElse;
					ReadingState = ReadingNameState::Name;
				}
				if (KeyWord == TokenType::KeyWord_for)
				{
					KeyWord = TokenType::KeyWord_ClassFor;
					ReadingState = ReadingNameState::Name;
				}
			}
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
			{
				size_t NexIndex = TextIndex;
				char NextChar = _Text.size() > NexIndex ? _Text[NexIndex] : '\0';
				if (NextChar == '.')
				{
					TextIndex++;

					bool IsReadingfloat = false;
					while (TextIndex < _Text.size())
					{

						char LookingAtChar = _Text[TextIndex];
						if (!LexerHelper::IsDigit(LookingAtChar)) { break; }
						TextIndex++;
						IsReadingfloat = true;
					}
					if (IsReadingfloat)
					{
						Type = TokenType::Float_literal;
						NameBuffer = String_view(NameBuffer.data(), NameBuffer.size() + (TextIndex - NexIndex));
					}
					else
					{
						NameBuffer = String_view(NameBuffer.data(), TextIndex - NexIndex);
						Type = TokenType::Number_literal;
						TextIndex = NexIndex;
					}
				}
				else
				{
					Type = TokenType::Number_literal;
				}
			}
			break;
			default:
				Type = TokenType::Null;
				break;
			}


			_Token.Type = Type;
			_Token.Value = NameBuffer;
			ReadingState = ReadingNameState::Name;
		}

		TextIndex--;

		_Tokens.push_back(_Token);
		ClearNameBuffer();
		_Token = Token();
		_Token.OnLine = OnLine;
		_Token.OnPos = TextIndex;
	}
}
UCodeLangFrontEnd