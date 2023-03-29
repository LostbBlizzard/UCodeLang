#pragma once

#include "../../LangCore.hpp"
UCodeLangStart

using TokenType_t = UInt8;
enum class TokenType : TokenType_t
{
	Null,
	EndofFile, //End?
	Error,// Error
	Name,// Name
	Class,//$
	Namespace,//%
	Left_Parentheses,//(
	Right_Parentheses,//)
	Left_Bracket,//[
	Right_Bracket,//]
	Colon,//:
	
	StartTab,//
	EndTab,//

	Semicolon,//;
	Dot,//.
	forwardslash,// /
	equal,//=
	plus,//+
	minus,//-
	star,//*
	modulo,//%
	bitwise_XOr, //^,
	bitwise_not, //~,
	Type,
	
	String_literal,// "String"
	Number_literal, // 100
	Float_literal,
	Char_literal,

	KeyWorld_Ret,//ret
	leftArrow,//<-
	RightArrow,//->
	RightAssignArrow,//=>

	OptionalDot,//?.
	IndirectMember,//~>
	ExclamationDot,//!.

	Void,//void
	Comma,//,

	KeyWorld_Sizeof,//sizeof
	KeyWorld_Nameof,//nameof
	KeyWorld_True,//true
	KeyWorld_False,//false
	KeyWorld_var,//var

	Not,//!
	QuestionMark,//?
	equal_Comparison,//==
	Notequal_Comparison,//!=
	greaterthan,//>
	lessthan,//<
	less_than_or_equalto,//<=
	greater_than_or_equalto,//>=
	logical_and,//&&
	logical_or,//||
	bitwise_and,//&
	bitwise_or,//|
	bitwise_LeftShift,//<<
	bitwise_RightShift,//>>
	increment,//++
	decrement,//--

	CompoundAdd,//+=
	CompoundSub,//-=
	CompoundMult,//*=
	CompoundDiv,// /=

	KeyWord_invalid,//invaild
	KeyWord_continue,//continue
	KeyWord_break,//break
	ScopeResolution,//::
	KeyWord_new,//new
	KeyWord_This,//this
	KeyWord_use,//use
	KeyWord_Drop,//drop
	KeyWord_static,// static
	KeyWord_Thread,// thread

	KeyWord_If,
	KeyWord_Else,
	KeyWord_while,
	KeyWord_for,
	KeyWord_do,


	KeyWorld_pub,//pub
	KeyWorld_typeof,//typeof
	hash,//#

	approximate_Comparison,//~=

	KeyWorld_Enum,//enum
	KeyWorld_Tag,//tag
	KeyWorld_umut,//umut

	//Types
	KeyWord_UInt8,
	KeyWord_UInt16,
	KeyWord_UInt32,
	KeyWord_UInt64,

	KeyWord_SInt8,
	KeyWord_SInt16,
	KeyWord_SInt32,
	KeyWord_SInt64,

	KeyWord_Char,
	KeyWord_Bool,

	KeyWord_float32,
	KeyWord_float64,

	KeyWord_uintptr,
	KeyWord_sintptr,
	//used internally
	internal_Constant_expression
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
		case UCodeLang::TokenType::Name:return "Name";
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
		case UCodeLang::TokenType::KeyWord_UInt8:return "uint8";
		case UCodeLang::TokenType::KeyWord_SInt8:return "sint8";
		case UCodeLang::TokenType::KeyWord_UInt16:return "uint16";
		case UCodeLang::TokenType::KeyWord_SInt16:return "sint16";
		case UCodeLang::TokenType::KeyWord_UInt32:return "uint32";
		case UCodeLang::TokenType::KeyWord_SInt32:return "sint32";
		case UCodeLang::TokenType::KeyWord_UInt64:return "uint64";
		case UCodeLang::TokenType::KeyWord_SInt64:return "sint64";

		case UCodeLang::TokenType::KeyWord_Char:return "char";
		case UCodeLang::TokenType::KeyWord_Bool:return "bool";

		case UCodeLang::TokenType::KeyWord_float32:return "float32";
		case UCodeLang::TokenType::KeyWord_float64:return "float64";

		case UCodeLang::TokenType::String_literal:return "String literal";
		case UCodeLang::TokenType::Number_literal:return "Number literal";
		case UCodeLang::TokenType::Char_literal:return "Char_literal";
		case UCodeLang::TokenType::Float_literal:return "Float_literal";

		case UCodeLang::TokenType::KeyWorld_Ret:return "ret";
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

		case UCodeLang::TokenType::CompoundAdd:return "+=";
		case UCodeLang::TokenType::CompoundSub:return "-=";
		case UCodeLang::TokenType::CompoundMult:return "*=";
		case UCodeLang::TokenType::CompoundDiv:return "/=";

		case UCodeLang::TokenType::KeyWord_invalid:return "invalid";
		case UCodeLang::TokenType::KeyWord_break: return "break";
		case UCodeLang::TokenType::ScopeResolution: return "::";
		case UCodeLang::TokenType::KeyWord_use: return "use";

		case UCodeLang::TokenType::KeyWord_This: return "this";
		case UCodeLang::TokenType::KeyWord_Drop: return "drop";
		case UCodeLang::TokenType::KeyWord_new: return "new";
		case UCodeLang::TokenType::KeyWord_static: return "static";
		case UCodeLang::TokenType::KeyWord_Thread: return "staticforthread";

		case UCodeLang::TokenType::KeyWord_uintptr: return "uintptr";
		case UCodeLang::TokenType::KeyWord_sintptr: return "sintptr";

		case UCodeLang::TokenType::KeyWorld_pub: return "pub";
		case UCodeLang::TokenType::KeyWorld_typeof: return "typeof";

		case UCodeLang::TokenType::hash: return "#";

		case UCodeLang::TokenType::KeyWorld_Enum:return "enum";
		case UCodeLang::TokenType::KeyWorld_umut:return "umut";
		case UCodeLang::TokenType::KeyWorld_Tag: return "tag";

		case UCodeLang::TokenType::QuestionMark:return "?";

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
	String_view _String;
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
};
struct Token
{ 
	TokenType Type = TokenType::Null;
	TokenValue Value;

	size_t OnLine =0;
	size_t OnPos = 0;

	static void PushString(String& out,const Token& T)
	{
		switch (T.Type)
		{
		case TokenType::Class:
		case TokenType::Namespace:
		case TokenType::Name:
			out += T.Value._String;
			break;
		case TokenType::Number_literal:
			out += T.Value._String;
			break;
		case TokenType::String_literal:
			out += "\"";
			out += T.Value._String;
			out += +"\"";
			break;
		default:
			StringHelper::PushString(out, T.Type);
			break;
		}
		
	}

}; 
UCodeLangEnd