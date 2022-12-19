#pragma once
#include "../../LangCore.hpp"
UCodeLangStart





inline const char* S_KeyWord_Int8 = "int8";
inline const char* S_KeyWord_sInt8 = "Sint8"; 
inline const char* S_KeyWord_sbyte = "Sbyte";
inline const char* S_KeyWord_byte = "byte";
inline const char* S_KeyWord_uInt8 = "Uint8";
inline const char* S_KeyWord_char = "char";
inline const char* S_KeyWord_bool = "bool";

inline const char* S_KeyWord_int = "int";
inline const char* S_KeyWord_float = "float";
inline const char* S_KeyWord_float32 = "float32";
inline const char* S_KeyWord_float64 = "float64";

inline const char* S_KeyWord_ret = "ret";
inline const char* S_KeyWord_void = "void";

inline const char* S_KeyWord_sizeof = "sizeof";
inline const char* S_KeyWord_nameof = "nameof";
inline const char* S_KeyWord_typeof = "typeof";

inline const char* S_KeyWord_Log = "log";

inline const char* S_KeyWord_true = "true";
inline const char* S_KeyWord_false = "false";
inline const char* S_KeyWord_var = "var";

inline const char* S_KeyWord_while = "while";
inline const char* S_KeyWord_for = "for";
inline const char* S_KeyWord_do = "do";

inline const char* S_KeyWord_if = "if";
inline const char* S_KeyWord_else = "else";
inline const char* S_KeyWord_invalid = "invalid";
inline const char* S_KeyWord_break = "break";
inline const char* S_KeyWord_use = "use";
inline const char* S_KeyWord_this = "this";
inline const char* S_KeyWord_drop = "drop";
inline const char* S_KeyWord_new = "new";
inline const char* S_KeyWord_copy = "copy";
inline const char* S_KeyWord_move = "move";
inline const char* S_KeyWord_swap = "swap";


inline const char* S_KeyWord_static = "static";

inline const char* S_KeyWord_uintptr = "uintptr";
inline const char* S_KeyWord_sintptr = "sintptr";

inline const char* S_KeyWord_pub = "pub";
inline const char* S_KeyWord_get = "get";
inline const char* S_KeyWord_set = "set";

inline const char* S_KeyWord_asm = "asm";
struct KeyWord
{
	const char* keyWord;
	TokenType token;
	constexpr KeyWord(const char* word, TokenType T):keyWord(word),token(T)
	{

	}
};
inline KeyWord KeyWords[] =
{


	//Types	
	
	KeyWord(S_KeyWord_Int8,TokenType::KeyWorld_SInt8),
	KeyWord(S_KeyWord_sInt8,TokenType::KeyWorld_SInt8),
	KeyWord(S_KeyWord_sbyte,TokenType::KeyWorld_SInt8),
	KeyWord(S_KeyWord_byte,TokenType::KeyWorld_UInt8),
	KeyWord(S_KeyWord_uInt8,TokenType::KeyWorld_UInt8),
	KeyWord(S_KeyWord_char,TokenType::KeyWorld_Char),
	KeyWord(S_KeyWord_bool,TokenType::KeyWorld_Bool),
	
	KeyWord(S_KeyWord_int,TokenType::KeyWorld_UInt8),
	KeyWord(S_KeyWord_float32,TokenType::KeyWorld_float32),
	KeyWord(S_KeyWord_float,TokenType::KeyWorld_float32),
	KeyWord(S_KeyWord_float64,TokenType::KeyWorld_float64),

	KeyWord(S_KeyWord_uintptr,TokenType::KeyWorld_uintptr),
	KeyWord(S_KeyWord_sintptr,TokenType::KeyWorld_sintptr),

	//
	KeyWord(S_KeyWord_ret,TokenType::KeyWorld_Ret),
	KeyWord(S_KeyWord_void,TokenType::Void),
	KeyWord(S_KeyWord_sizeof,TokenType::KeyWorld_Sizeof),
	KeyWord(S_KeyWord_nameof,TokenType::KeyWorld_Nameof),
	//
	KeyWord(S_KeyWord_true,TokenType::KeyWorld_True),
	KeyWord(S_KeyWord_false,TokenType::KeyWorld_False),
	KeyWord(S_KeyWord_var,TokenType::KeyWorld_var),

	KeyWord(S_KeyWord_invalid,TokenType::KeyWord_invalid),
	KeyWord(S_KeyWord_break,TokenType::KeyWord_break),
	KeyWord(S_KeyWord_use,TokenType::KeyWorld_use),

	KeyWord(S_KeyWord_this,TokenType::KeyWorld_This),
	KeyWord(S_KeyWord_drop,TokenType::KeyWorld_Drop),
	KeyWord(S_KeyWord_new,TokenType::KeyWorld_new),
	KeyWord(S_KeyWord_static,TokenType::KeyWorld_static),

	KeyWord(S_KeyWord_pub,TokenType::KeyWorld_pub),
	KeyWord(S_KeyWord_typeof,TokenType::KeyWorld_typeof),
	KeyWord(S_KeyWord_get,TokenType::KeyWorld_get),
	KeyWord(S_KeyWord_set,TokenType::KeyWorld_set),

	KeyWord(S_KeyWord_asm,TokenType::KeyWorld_asm),
};
constexpr size_t KeyWords_Size = sizeof(KeyWords) / sizeof(KeyWords[0]);
class KeyWordHelper
{
public:
	static bool IsKeyWord(const String_view& Str)
	{
		for (size_t i = 0; i < KeyWords_Size; i++)
		{
			if (Str == KeyWords[i].keyWord) { return true; }
		}
		return false;
	}
	static TokenType GetToken(const String_view& Str)
	{
		for (size_t i = 0; i < KeyWords_Size; i++)
		{
			const auto& Item = KeyWords[i];
			if (Str == Item.keyWord) { return  Item.token; }
		}
		return TokenType::Null;
	}
};


UCodeLangEnd