#pragma once

#include "../../LangCore.hpp"
UCodeLangStart

using TokenType_t = UInt8;
enum class TokenType : TokenType_t
{
	Null,
	EndofFile,
	Error,
	Name,
	Class,
	Namespace,
	Left_Parentheses,
	Right_Parentheses,
	Left_Bracket,
	Right_Bracket,
	Colon,
	
	StartTab,
	EndTab,

	Semicolon,
	Dot,
	forwardslash,
	equal,
	plus,
	minus,
	star,
	modulo,
	bitwise_XOr, //^,
	bitwise_not, //~,
	Type,
	KeyWorld_UInt8,
	KeyWorld_SInt8,
	KeyWorld_Char,
	KeyWorld_Bool,
	String_literal,
	Number_literal,

	Return,
	RightArrow,
	Void,
	Comma,

	KeyWorld_Sizeof,
	KeyWorld_Nameof,
	KeyWorld_True,
	KeyWorld_False,
	KeyWorld_var,

	Not,
	equal_Comparison,
	Notequal_Comparison,
	greaterthan,
	lessthan,
	less_than_or_equalto,
	greater_than_or_equalto,
	logical_and,
	logical_or,
	bitwise_and,
	bitwise_or,
	bitwise_LeftShift,
	bitwise_RightShift,
	increment,
	decrement,

	KeyWorld_while,
	KeyWorld_for,
	KeyWorld_do,
	KeyWorld_if,
	KeyWorld_else,

	CompoundAdd,
	CompoundSub,
	CompoundMult,
	CompoundDiv,

	KeyWord_invalid,
	KeyWord_break,
	ScopeResolution,
	KeyWorld_use,
	KeyWorld_This,
	KeyWorld_Drop,
	KeyWorld_new,
	KeyWorld_static,

	KeyWorld_uintptr,
	KeyWorld_sintptr,

	KeyWorld_copy,
	KeyWorld_move,
	KeyWorld_swap,
	KeyWorld_pub,
	KeyWorld_typeof,
	KeyWorld_get,
	KeyWorld_set,
	hash,
	KeyWorld_float32,
	KeyWorld_float64,
	KeyWorld_asm,
};
class StringHelper
{
public:
	static void PushString(String& out,TokenType V)
	{
		const char* str = ToString(V);
		out += str;
	}
	constexpr static  const char* ToString(TokenType V)
	{
		switch (V)
		{
		case UCodeLang::TokenType::Null:return "null";
		case UCodeLang::TokenType::EndofFile:return "End of File";
		case UCodeLang::TokenType::Error:return "Error";
		case UCodeLang::TokenType::Name:return "identifier";
		case UCodeLang::TokenType::Namespace:return "%";
		case UCodeLang::TokenType::Class:return "$";
		case UCodeLang::TokenType::Left_Parentheses:return "(";
		case UCodeLang::TokenType::Right_Parentheses:return ")";
		case UCodeLang::TokenType::Left_Bracket:return "[";
		case UCodeLang::TokenType::Right_Bracket:return "]";
		case UCodeLang::TokenType::Colon:return ":";
		case UCodeLang::TokenType::StartTab:return "StartTab";
		case UCodeLang::TokenType::EndTab:return "EndTab";
		case UCodeLang::TokenType::Semicolon:return ";";
		case UCodeLang::TokenType::Dot:return ".";
		case UCodeLang::TokenType::forwardslash:return "/";
		case UCodeLang::TokenType::equal:return "=";
		case UCodeLang::TokenType::plus:return "+";
		case UCodeLang::TokenType::minus:return "-";
		case UCodeLang::TokenType::star:return "*";

		case UCodeLang::TokenType::Type:return "Type";
		case UCodeLang::TokenType::KeyWorld_UInt8:return "uint8";
		case UCodeLang::TokenType::KeyWorld_SInt8:return "sint8";
		case UCodeLang::TokenType::KeyWorld_Char:return "char";
		case UCodeLang::TokenType::KeyWorld_Bool:return "bool";
		case UCodeLang::TokenType::String_literal:return "String literal";
		case UCodeLang::TokenType::Number_literal:return "Number literal";

		case UCodeLang::TokenType::Return:return "ret";
		case UCodeLang::TokenType::RightArrow:return "->";
		case UCodeLang::TokenType::Void:return "void";

		case UCodeLang::TokenType::Comma:return ",";

		case UCodeLang::TokenType::KeyWorld_Sizeof:return "sizeof";
		case UCodeLang::TokenType::KeyWorld_Nameof:return "nameof";
		case UCodeLang::TokenType::KeyWorld_True:return "true";
		case UCodeLang::TokenType::KeyWorld_False:return "false";
		case UCodeLang::TokenType::KeyWorld_var:return "var";
		case UCodeLang::TokenType::Not:return "!";
		case UCodeLang::TokenType::equal_Comparison:return "==";
		case UCodeLang::TokenType::Notequal_Comparison:return "!=";
		case UCodeLang::TokenType::lessthan:return "<";
		case UCodeLang::TokenType::greaterthan:return ">";
		case UCodeLang::TokenType::less_than_or_equalto:return "<=";
		case UCodeLang::TokenType::greater_than_or_equalto:return ">=";
		case UCodeLang::TokenType::logical_and:return "&&";
		case UCodeLang::TokenType::logical_or:return "||";
		case UCodeLang::TokenType::bitwise_and:return "&";
		case UCodeLang::TokenType::bitwise_or:return "|";

		case UCodeLang::TokenType::increment:return "++";
		case UCodeLang::TokenType::decrement:return "--";

		case UCodeLang::TokenType::KeyWorld_while:return "while";
		case UCodeLang::TokenType::KeyWorld_for:return "for";
		case UCodeLang::TokenType::KeyWorld_do:return "do";
		case UCodeLang::TokenType::KeyWorld_if:return "if";
		case UCodeLang::TokenType::KeyWorld_else:return "else";

		case UCodeLang::TokenType::CompoundAdd:return "+=";
		case UCodeLang::TokenType::CompoundSub:return "-=";
		case UCodeLang::TokenType::CompoundMult:return "*=";
		case UCodeLang::TokenType::CompoundDiv:return "/=";

		case UCodeLang::TokenType::KeyWord_invalid:return "invalid";
		case UCodeLang::TokenType::KeyWord_break: return "break";
		case UCodeLang::TokenType::ScopeResolution: return "::";
		case UCodeLang::TokenType::KeyWorld_use: return "use";

		case UCodeLang::TokenType::KeyWorld_This: return "this";
		case UCodeLang::TokenType::KeyWorld_Drop: return "drop";
		case UCodeLang::TokenType::KeyWorld_new: return "new(keyword)";
		case UCodeLang::TokenType::KeyWorld_static: return "static";

		case UCodeLang::TokenType::KeyWorld_uintptr: return "uintptr";
		case UCodeLang::TokenType::KeyWorld_sintptr: return "sintptr";

		case UCodeLang::TokenType::KeyWorld_copy: return "copy";
		case UCodeLang::TokenType::KeyWorld_move: return "move";
		case UCodeLang::TokenType::KeyWorld_swap: return "swap";
		case UCodeLang::TokenType::KeyWorld_pub: return "pub";
		case UCodeLang::TokenType::KeyWorld_typeof: return "typeof";

		case UCodeLang::TokenType::KeyWorld_get: return "get";
		case UCodeLang::TokenType::KeyWorld_set: return "set";

		case UCodeLang::TokenType::hash: return "#";

		case UCodeLang::TokenType::KeyWorld_asm: return "asm";

		default:return "Unknown token";
		}
	}
};
struct TokenValue
{
	TokenValue() :_Ptr(nullptr), _String(), _Int8(0), _Size_t(0)
	{

	}


	void* _Ptr;
	String _String;
	Int8 _Int8;
	
	size_t _Size_t;

	TokenValue& operator=(void* ptr)
	{
		_Ptr = ptr;
		return *this;
	}
	TokenValue& operator=(Int8 Value)
	{
		_Int8 = Value;
		return *this;
	}
	TokenValue& operator=(size_t Value)
	{
		_Size_t = Value;
		return *this;
	}
	TokenValue& operator=(String_view String_literal)
	{
		_String = String_literal;
		return *this;
	}
	TokenValue& operator=(String String_literal)
	{
		_String = String_literal;
		return *this;
	}
};
struct Token
{ 
	TokenType Type = TokenType::Null;
	TokenValue Value;

	size_t OnLine =0;
	size_t OnPos = 0;
	static constexpr size_t EndOfFile = -1;

	static void PushString(String& out,const Token& T)
	{
		switch (T.Type)
		{
		case TokenType::Name:
			out += T.Value._String;
			break;
		case TokenType::Number_literal:
			out += T.Value._String;
			break;
		case TokenType::String_literal:
			out += "\"" + T.Value._String + "\"";
			break;
		default:
			StringHelper::PushString(out, T.Type);
			break;
		}
		
	}

}; 
UCodeLangEnd