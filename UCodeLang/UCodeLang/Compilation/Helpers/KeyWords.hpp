#pragma once
#ifndef UCodeLangNoCompiler
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

inline const char* S_KeyWord_true = "true";
inline const char* S_KeyWord_false = "false";
inline const char* S_KeyWord_var = "var";
inline const char* S_KeyWord_invalid = "invalid";
inline const char* S_KeyWord_valid = "valid";
inline const char* S_KeyWord_continue = "continue";
inline const char* S_KeyWord_break = "break";
inline const char* S_KeyWord_use = "use";
inline const char* S_KeyWord_this = "this";
inline const char* S_KeyWord_drop = "drop";
inline const char* S_KeyWord_new = "new";
inline const char* S_KeyWord_static = "static";
inline const char* S_KeyWord_thread = "thread";

inline const char* S_KeyWord_moved = "moved";
inline const char* S_KeyWord_move = "move";

inline const char* S_KeyWord_if = "if";
inline const char* S_KeyWord_else = "else";
inline const char* S_KeyWord_while = "while";
inline const char* S_KeyWord_for = "for";
inline const char* S_KeyWord_do = "do";

inline const char* S_KeyWord_public = "public";
inline const char* S_KeyWord_private = "private";

inline const char* S_KeyWord_enum = "enum";
inline const char* S_KeyWord_tag = "tag";

inline const char* S_KeyWord_imut = "imut";

inline const char* S_KeyWord_trait = "trait";
inline const char* S_KeyWord_dynamic = "dynamic";
inline const char* S_KeyWord_out = "out";

inline const char* S_KeyWord_bitcast = "bitcast";
inline const char* S_KeyWord_panic = "panic";

inline const char* S_KeyWord_typeof = "typeof";
inline const char* S_KeyWord_type = "type";
inline const char* S_KeyWord_bind = "bind";

inline const char* S_KeyWord_match = "match";

inline const char* S_KeyWord_eval = "eval";

inline const char* S_KeyWord_import = "import";

inline const char* S_KeyWord_typeinfo = "typeinfo";


inline const char* S_KeyWord_unq = "unq";
inline const char* S_KeyWord_shr = "shr";

inline const char* S_KeyWord_null = "null";
inline const char* S_KeyWord_nullptr = "nullptr";

inline const char* S_KeyWord_extern = "extern";


inline const char* S_KeyWord_await = "await";
inline const char* S_KeyWord_async = "async";
inline const char* S_KeyWord_yield = "yield";

inline const char* S_KeyWord_utf8 = "utf8";
inline const char* S_KeyWord_utf16 = "utf16";
inline const char* S_KeyWord_utf32 = "utf32";
inline const char* S_KeyWord_utf64 = "utf64";

inline const char* S_KeyWord_unsafe = "unsafe";

inline const char* S_KeyWord_block = "block";

inline const char* S_KeyWord_defer = "defer";

inline const char* S_KeyWord_mut = "mut";

inline const char* S_KeyWord_compiler = "compiler";

inline const char* S_KeyWord_export = "export";

inline const char* S_KeyWord_functor = "functor";

struct KeyWord
{
	const char* keyWord;
	TokenType token;
	constexpr KeyWord(const char* word, TokenType T) :keyWord(word), token(T)
	{

	}
};
inline KeyWord KeyWords[] =
{


	//Types	

	KeyWord(S_KeyWord_Int8,TokenType::KeyWord_SInt8),
	KeyWord(S_KeyWord_Int16,TokenType::KeyWord_SInt16),
	KeyWord(S_KeyWord_Int32,TokenType::KeyWord_SInt32),
	KeyWord(S_KeyWord_Int64,TokenType::KeyWord_SInt64),

	KeyWord(S_KeyWord_uInt8,TokenType::KeyWord_UInt8),
	KeyWord(S_KeyWord_uInt16,TokenType::KeyWord_UInt16),
	KeyWord(S_KeyWord_uInt32,TokenType::KeyWord_UInt32),
	KeyWord(S_KeyWord_uInt64,TokenType::KeyWord_UInt64),

	KeyWord(S_KeyWord_Int,TokenType::KeyWord_SInt32),

	KeyWord(S_KeyWord_byte,TokenType::KeyWord_UInt8),
	KeyWord(S_KeyWord_sbyte,TokenType::KeyWord_SInt8),

	KeyWord(S_KeyWord_char,TokenType::KeyWord_Char),
	KeyWord(S_KeyWord_bool,TokenType::KeyWord_Bool),

	KeyWord(S_KeyWord_float32,TokenType::KeyWord_float32),
	KeyWord(S_KeyWord_float,TokenType::KeyWord_float32),
	KeyWord(S_KeyWord_float64,TokenType::KeyWord_float64),

	KeyWord(S_KeyWord_uintptr,TokenType::KeyWord_uintptr),
	KeyWord(S_KeyWord_sintptr,TokenType::KeyWord_sintptr),

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
	KeyWord(S_KeyWord_valid,TokenType::KeyWord_valid),
	KeyWord(S_KeyWord_continue,TokenType::KeyWord_continue),
	KeyWord(S_KeyWord_break,TokenType::KeyWord_break),
	KeyWord(S_KeyWord_use,TokenType::KeyWord_use),

	KeyWord(S_KeyWord_this,TokenType::KeyWord_This),
	KeyWord(S_KeyWord_drop,TokenType::KeyWord_Drop),
	KeyWord(S_KeyWord_new,TokenType::KeyWord_new),
	KeyWord(S_KeyWord_static,TokenType::KeyWord_static),
	KeyWord(S_KeyWord_thread,TokenType::KeyWord_Thread),

	KeyWord(S_KeyWord_moved,TokenType::KeyWord_moved),
	KeyWord(S_KeyWord_move,TokenType::KeyWord_move),

	KeyWord(S_KeyWord_if,TokenType::KeyWord_If),
	KeyWord(S_KeyWord_else,TokenType::KeyWord_Else),
	KeyWord(S_KeyWord_while,TokenType::KeyWord_while),
	KeyWord(S_KeyWord_for,TokenType::KeyWord_for),
	KeyWord(S_KeyWord_do,TokenType::KeyWord_do),
	KeyWord(S_KeyWord_match,TokenType::KeyWord_match),

	KeyWord(S_KeyWord_public,TokenType::KeyWorld_public),
	KeyWord(S_KeyWord_private,TokenType::KeyWorld_private),

	KeyWord(S_KeyWord_enum,TokenType::KeyWord_Enum),
	KeyWord(S_KeyWord_tag,TokenType::KeyWord_Tag),
	KeyWord(S_KeyWord_imut,TokenType::KeyWord_imut),
	KeyWord(S_KeyWord_trait,TokenType::KeyWord_trait),
	KeyWord(S_KeyWord_dynamic,TokenType::KeyWord_dynamic),
	KeyWord(S_KeyWord_out,TokenType::KeyWord_out),
	KeyWord(S_KeyWord_bitcast,TokenType::KeyWord_bitcast),
	KeyWord(S_KeyWord_panic,TokenType::KeyWord_panic),

	KeyWord(S_KeyWord_typeof,TokenType::KeyWorld_typeof),
	KeyWord(S_KeyWord_type,TokenType::KeyWord_type),
	KeyWord(S_KeyWord_bind,TokenType::KeyWord_bind),
	KeyWord(S_KeyWord_eval,TokenType::KeyWord_eval),
	KeyWord(S_KeyWord_import,TokenType::KeyWord_Import),

	KeyWord(S_KeyWord_typeinfo,TokenType::KeyWord_TypeInfo),

	KeyWord(S_KeyWord_unq,TokenType::KeyWord_unq),
	KeyWord(S_KeyWord_shr,TokenType::KeyWord_shr),

	KeyWord(S_KeyWord_null,TokenType::Null),
	KeyWord(S_KeyWord_nullptr,TokenType::Null),

	KeyWord(S_KeyWord_extern,TokenType::KeyWord_extern),

	KeyWord(S_KeyWord_await,TokenType::KeyWord_await),
	KeyWord(S_KeyWord_async,TokenType::KeyWord_async),
	KeyWord(S_KeyWord_yield,TokenType::KeyWord_yield),

	KeyWord(S_KeyWord_utf8,TokenType::KeyWord_uft8),
	KeyWord(S_KeyWord_utf16,TokenType::KeyWord_uft16),
	KeyWord(S_KeyWord_utf32,TokenType::KeyWord_uft32),
	KeyWord(S_KeyWord_utf64,TokenType::Null),

	KeyWord(S_KeyWord_unsafe,TokenType::KeyWord_unsafe),
	KeyWord(S_KeyWord_block, TokenType::KeyWord_block),
	KeyWord(S_KeyWord_defer, TokenType::KeyWord_defer),

	KeyWord(S_KeyWord_mut,TokenType::Null),

	KeyWord(S_KeyWord_compiler, TokenType::KeyWord_compiler),

	KeyWord(S_KeyWord_export, TokenType::KeyWord_export),
	
	KeyWord(S_KeyWord_functor, TokenType::KeyWord_functor),
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

#endif