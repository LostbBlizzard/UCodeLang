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
	KeyWord_move,// move
	KeyWord_moved,// moved

	KeyWord_If,//if
	KeyWord_Else,//else
	KeyWord_while,//while
	KeyWord_for,//for
	KeyWord_do,//do


	KeyWorld_public,//public
	KeyWorld_private,//private
	KeyWorld_typeof,//typeof
	hash,//#

	approximate_Comparison,//~=

	KeyWord_Enum,//enum
	KeyWord_Tag,//tag
	KeyWord_umut,//umut

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
	KeyWord_trait,
	KeyWord_dynamic,
	KeyWord_out,
	KeyWord_bitcast,
	KeyWord_panic,
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
		case TokenType::Null:return "null";
		case TokenType::EndofFile:return "End of File";
		case TokenType::Error:return "Error";
		case TokenType::Name:return "Name";
		case TokenType::Namespace:return "%";
		case TokenType::Class:return "$";
		case TokenType::Left_Parentheses:return "(";
		case TokenType::Right_Parentheses:return ")";
		case TokenType::Left_Bracket:return "[";
		case TokenType::Right_Bracket:return "]";
		case TokenType::Colon:return ":";
		case TokenType::StartTab:return "StartTab";
		case TokenType::EndTab:return "EndTab";
		case TokenType::Semicolon:return ";";
		case TokenType::Dot:return ".";
		case TokenType::forwardslash:return "/";
		case TokenType::equal:return "=";
		case TokenType::plus:return "+";
		case TokenType::minus:return "-";
		case TokenType::star:return "*";

		case TokenType::Type:return "Type";
		case TokenType::KeyWord_UInt8:return "uint8";
		case TokenType::KeyWord_SInt8:return "sint8";
		case TokenType::KeyWord_UInt16:return "uint16";
		case TokenType::KeyWord_SInt16:return "sint16";
		case TokenType::KeyWord_UInt32:return "uint32";
		case TokenType::KeyWord_SInt32:return "sint32";
		case TokenType::KeyWord_UInt64:return "uint64";
		case TokenType::KeyWord_SInt64:return "sint64";

		case TokenType::KeyWord_Char:return "char";
		case TokenType::KeyWord_Bool:return "bool";

		case TokenType::KeyWord_float32:return "float32";
		case TokenType::KeyWord_float64:return "float64";

		case TokenType::String_literal:return "String literal";
		case TokenType::Number_literal:return "Number literal";
		case TokenType::Char_literal:return "Char_literal";
		case TokenType::Float_literal:return "Float_literal";

		case TokenType::KeyWorld_Ret:return "ret";
		case TokenType::RightArrow:return "->";
		case TokenType::Void:return "void";

		case TokenType::Comma:return ",";

		case TokenType::KeyWorld_Sizeof:return "sizeof";
		case TokenType::KeyWorld_Nameof:return "nameof";
		case TokenType::KeyWorld_True:return "true";
		case TokenType::KeyWorld_False:return "false";
		case TokenType::KeyWorld_var:return "var";
		case TokenType::Not:return "!";
		case TokenType::equal_Comparison:return "==";
		case TokenType::Notequal_Comparison:return "!=";
		case TokenType::lessthan:return "<";
		case TokenType::greaterthan:return ">";
		case TokenType::less_than_or_equalto:return "<=";
		case TokenType::greater_than_or_equalto:return ">=";
		case TokenType::logical_and:return "&&";
		case TokenType::logical_or:return "||";
		case TokenType::bitwise_and:return "&";
		case TokenType::bitwise_or:return "|";

		case TokenType::increment:return "++";
		case TokenType::decrement:return "--";

		case TokenType::CompoundAdd:return "+=";
		case TokenType::CompoundSub:return "-=";
		case TokenType::CompoundMult:return "*=";
		case TokenType::CompoundDiv:return "/=";

		case TokenType::KeyWord_invalid:return "invalid";
		case TokenType::KeyWord_break: return "break";
		case TokenType::ScopeResolution: return "::";
		case TokenType::KeyWord_use: return "use";

		case TokenType::KeyWord_This: return "this";
		case TokenType::KeyWord_Drop: return "drop";
		case TokenType::KeyWord_new: return "new";
		case TokenType::KeyWord_static: return "static";
		case TokenType::KeyWord_Thread: return "staticforthread";

		case TokenType::KeyWord_uintptr: return "uintptr";
		case TokenType::KeyWord_sintptr: return "sintptr";

		case TokenType::KeyWorld_public: return "public";
		case TokenType::KeyWorld_private: return "private";

		case TokenType::KeyWorld_typeof: return "typeof";

		case TokenType::hash: return "#";

		case TokenType::KeyWord_Enum:return "enum";
		case TokenType::KeyWord_umut:return "umut";
		case TokenType::KeyWord_Tag: return "tag";

		case TokenType::QuestionMark:return "?";
		case TokenType::modulo:return "%";

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