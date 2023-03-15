#pragma once
#include "../../LangCore.hpp"
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
UCodeLangFrontStart
//Int types
inline const char* S_KeyWord_Int8 = "int8";
inline const char* S_KeyWord_Int16 = "int16";
inline const char* S_KeyWord_Int32 = "int32";
inline const char* S_KeyWord_Int64 = "int64";

inline const char* S_KeyWord_Int = "int";

inline const char* S_KeyWord_uInt8 = "uint8";
inline const char* S_KeyWord_uInt16 = "uint16";
inline const char* S_KeyWord_uInt32 = "uint32";
inline const char* S_KeyWord_uInt64 = "uint64";

inline const char* S_KeyWord_byte = "byte";
inline const char* S_KeyWord_sbyte = "Sbyte";

//
inline const char* S_KeyWord_char = "char";
inline const char* S_KeyWord_bool = "bool";

//floats
inline const char* S_KeyWord_float = "float";
inline const char* S_KeyWord_float32 = "float32";
inline const char* S_KeyWord_float64 = "float64";

inline const char* S_KeyWord_uintptr = "uintptr";
inline const char* S_KeyWord_sintptr = "sintptr";

inline const char* S_KeyWord_ret = "ret";
inline const char* S_KeyWord_void = "void";
inline const char* S_KeyWord_sizeof = "sizeof";
inline const char* S_KeyWord_nameof = "nameof";
inline const char* S_KeyWord_typeof = "typeof";
inline const char* S_KeyWord_true = "true";
inline const char* S_KeyWord_false = "false";
inline const char* S_KeyWord_var = "var";
inline const char* S_KeyWord_invalid = "invalid";
inline const char* S_KeyWord_break = "break";
inline const char* S_KeyWord_use = "use";
inline const char* S_KeyWord_this = "this";
inline const char* S_KeyWord_drop = "drop";
inline const char* S_KeyWord_new = "new";
inline const char* S_KeyWord_static = "static";
inline const char* S_KeyWord_thread = "thread";

inline const char* S_KeyWord_if = "if";
inline const char* S_KeyWord_else = "else";
inline const char* S_KeyWord_while  = "while";
inline const char* S_KeyWord_for = "for";
inline const char* S_KeyWord_do = "do";

inline const char* S_KeyWord_pub = "pub";
inline const char* S_KeyWord_enum = "enum";
inline const char* S_KeyWord_tag = "tag";

inline const char* S_KeyWord_umut = "umut";
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
	KeyWord(S_KeyWord_Int16,TokenType::KeyWorld_SInt16),
	KeyWord(S_KeyWord_Int32,TokenType::KeyWorld_SInt32),
	KeyWord(S_KeyWord_Int64,TokenType::KeyWorld_SInt64),

	KeyWord(S_KeyWord_uInt8,TokenType::KeyWorld_UInt8),
	KeyWord(S_KeyWord_uInt16,TokenType::KeyWorld_UInt16),
	KeyWord(S_KeyWord_uInt32,TokenType::KeyWorld_UInt32),
	KeyWord(S_KeyWord_uInt64,TokenType::KeyWorld_UInt64),

	KeyWord(S_KeyWord_Int,TokenType::KeyWorld_SInt32),

	KeyWord(S_KeyWord_byte,TokenType::KeyWorld_UInt8),
	KeyWord(S_KeyWord_sbyte,TokenType::KeyWorld_SInt8),
	
	KeyWord(S_KeyWord_char,TokenType::KeyWorld_Char),
	KeyWord(S_KeyWord_bool,TokenType::KeyWorld_Bool),
	
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
	KeyWord(S_KeyWord_thread,TokenType::KeyWorld_Thread),

	KeyWord(S_KeyWord_if,TokenType::KeyWorld_If),
	KeyWord(S_KeyWord_else,TokenType::KeyWorld_Else),
	KeyWord(S_KeyWord_while,TokenType::KeyWorld_while),
	KeyWord(S_KeyWord_for,TokenType::KeyWorld_for),
	KeyWord(S_KeyWord_do,TokenType::KeyWorld_do),

	KeyWord(S_KeyWord_pub,TokenType::KeyWorld_pub),
	KeyWord(S_KeyWord_typeof,TokenType::KeyWorld_typeof),

	KeyWord(S_KeyWord_enum,TokenType::KeyWorld_Enum),
	KeyWord(S_KeyWord_tag,TokenType::KeyWorld_Tag),
	KeyWord(S_KeyWord_umut,TokenType::KeyWorld_umut),
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
UCodeLangFrontEnd