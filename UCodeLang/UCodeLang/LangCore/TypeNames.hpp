#pragma once
//#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
#define EnumBaseTypeName "base_t";

#define CharTypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_Char)
#define boolTypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_Bool)

#define Uint8TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_UInt8)
#define Sint8TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_SInt8)

#define Uint16TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_UInt16)
#define Sint16TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_SInt16)

#define Uint32TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_UInt32)
#define Sint32TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_SInt32)

#define Uint64TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_UInt64)
#define Sint64TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_SInt64)

#define UintPtrTypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_uintptr)
#define SintPtrTypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_sintptr)

#define float32TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_float32)
#define float64TypeName UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_float64)

#define VoidPtrTypeName "VoidPtr"
#define CStringTypeName "CString"

#define CompilerGeneratedStart "(&"
#define CompilerGeneratedEnd "&)"
#define CompilerGenerated(x) CompilerGeneratedStart x CompilerGeneratedEnd

#define ClassInitializefuncName CompilerGenerated("_init")
#define ClassConstructorfunc CompilerGenerated("_new")
#define ClassDestructorFunc CompilerGenerated("_drop")
#define ClassCopyFunc CompilerGenerated("_copy")
#define ClassMoveFunc CompilerGenerated("_move")
#define ClassSwapFunc CompilerGenerated("_swap")

#define ThisSymbolName CompilerGenerated("this")

#define StaticVariablesInitializeFunc CompilerGenerated("StaticInit")
#define StaticVariablesUnLoadFunc CompilerGenerated("StaticUnload")

#define StandardLibraryName "UCode"

#define StandardLibraryLibName StandardLibraryName ".lib"

#define DLLCallAttributeName "DLL"

//Op
#define Overload_Plus_Func CompilerGenerated("Plus")
#define Overload_minus_Func CompilerGenerated("minus")


//Postfix
#define Overload_increment_Func CompilerGenerated("increment")
#define Overload_decrement_Func CompilerGenerated("decrement")

//Compound
#define Overload_Compoundplus_Func CompilerGenerated("Compound_Plus")
#define Overload_CompoundSub_Func CompilerGenerated("Compound_Sub")

//others Op
#define Overload_Index_Func CompilerGenerated("Index")
#define Overload_Invoke_Func CompilerGenerated("Invoke")

//standard Library
#define UCode_OptionalType "Optional"//int?
#define UCode_ArrayType "Array"//int[10]
#define UCode_VectorType "Vector"//int[]
#define UCode_ViewType "View"//int[:]

#define UCode_Shared_ptr "Unique_ptr"//int^ 
#define Ucode_Unique_ptr "Shared_ptr"//int%