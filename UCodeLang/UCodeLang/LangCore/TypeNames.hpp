#pragma once
//#include "UCodeLang/Compilation/Helpers/KeyWords.hpp"
#define EnumBaseTypeName "base_t";

#define CharTypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_Char)
#define boolTypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_Bool)

#define Uint8TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_UInt8)
#define Sint8TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_SInt8)

#define Uint16TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_UInt16)
#define Sint16TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_SInt16)

#define Uint32TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_UInt32)
#define Sint32TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_SInt32)

#define Uint64TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_UInt64)
#define Sint64TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_SInt64)

#define UintPtrTypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_uintptr)
#define SintPtrTypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_sintptr)

#define float32TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_float32)
#define float64TypeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_float64)

#define Uft8typeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_uft8)
#define Uft16typeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_uft16)
#define Uft32typeName UCodeLang::TokenStringHelper::ToString(UCodeLang::TokenType::KeyWord_uft32)

#define VoidPtrTypeName "VoidPtr"
#define CStringTypeName "CString"

#define CompilerGeneratedStart "(&"
#define CompilerGeneratedEnd "&)"
#define CompilerGenerated(x) CompilerGeneratedStart x CompilerGeneratedEnd



#define ClassConstructorfunc CompilerGenerated("_new")
#define ClassDestructorFunc CompilerGenerated("_drop")

#define ClassInitializefuncName CompilerGenerated("_init")
#define ClassDeInitializefuncName CompilerGenerated("_de-init")

#define ThisSymbolName CompilerGenerated("this")

#define StaticVariablesInitializeFunc CompilerGenerated("StaticInit")
#define StaticVariablesUnLoadFunc CompilerGenerated("StaticUnload")

#define ThreadVariablesInitializeFunc CompilerGenerated("threadInit")
#define ThreadVariablesUnLoadFunc CompilerGenerated("threadUnload")

#define StandardLibraryName "UCode"

#define StandardLibraryLibName StandardLibraryName ".lib"

#define DLLCallAttributeName "DLL"

//Op
#define Overload_Plus_Func CompilerGenerated("Plus")
#define Overload_minus_Func CompilerGenerated("Minus")


#define Overload_equal_Func CompilerGenerated("equal")
#define Overload_notequal_Func CompilerGenerated("notequal")

//Postfix
#define Overload_increment_Func CompilerGenerated("Increment")
#define Overload_decrement_Func CompilerGenerated("Decrement")
//Urinary
#define Overload_Not_Func CompilerGenerated("Not")
#define Overload_Bitwise_Not CompilerGenerated("BitwiseNot")
#define Overload_exist_Func CompilerGenerated("Exist")

//Compound
#define Overload_Compoundplus_Func CompilerGenerated("Compound_Plus")
#define Overload_CompoundSub_Func CompilerGenerated("Compound_Sub")

//Member
#define Overload_IndirectMember_Func CompilerGenerated("IndirectMember")
#define Overload_OptionalDot_Func CompilerGenerated("OptionalDot")
#define Overload_ExclamationDot_Func CompilerGenerated("ExclamationDot")

//others Op
#define Overload_Index_Func CompilerGenerated("Index")
#define Overload_Invoke_Func CompilerGenerated("Invoke")
#define Overload_For_Func CompilerGenerated("For")
#define Overload_Cast_Func CompilerGenerated("Cast")

//standard Library
#define UCode_OptionalType "Optional"//int?
#define UCode_ArrayType "Array"//int[10]
#define UCode_VectorType "Vector"//int[]
#define UCode_SpanType "Span"//int[:]
#define UCode_MapType "Map"//int[String]
#define UCode_ResultType "Result"//int!
#define UCode_RangeFunction "Range"//0..10 Range(0,10)
#define UCode_RangeInclusiveFunction "RangeInclusive"//0..=10 RangeInclusive(0,10)

#define UCode_RangeType "Range_t"//0..10 Range_t<int>(0,10)
#define UCode_RangeInclusiveType "RangeInclusive_t"//0..=10 RangeInclusive<int>(0,10)


#define UCodeLangStringType "String"
#define UCodeLangString8Type  "String8"
#define UCodeLangString16Type  "String16"
#define UCodeLangString32Type  "String32"

#define UCodeLangStringSpanType "StringSpan"
#define UCodeLangStringSpan8Type  "String8Span"
#define UCodeLangStringSpan16Type  "String16Span"
#define UCodeLangStringSpan32Type  "String32Span"


#define UCode_Unique_ptr "Unique_ptr"//int^ 
#define UCode_Shared_ptr "Shared_ptr"//int$ 

#define UCode_Unique_Ptr_MakeFunction "Make"//int[^]::Make(...)
#define UCode_Shared_Ptr_MakeFunction "Make"//int[$]::Make(...)

#define UCode_Unique_Array "Unique_Array"//int[^] 
#define UCode_Shared_Array "Shared_Array"//int[$] 

#define UCode_FutureType "Future"//async<int> 

#define UCode_Unique_Array_MakeFunction "Make"//int[^]::Make(...)
#define UCode_Shared_Array_MakeFunction "Make"//int[$]::Make(...)

//
#define UCode_CodeLayer_UCodeVM_Name "UCodeVm"
#define UCode_CodeLayer_IR_Name "IR"
#define UCode_CodeLayer_Text_Name "Text"
#define UCode_CodeLayer_X86_Name "x86"
#define UCode_CodeLayer_X86_64_Name "x86_64"
#define UCode_CodeLayer_AMD_Name "AMD"
#define UCode_CodeLayer_AMD_64_Name "AMD64"

#define UCode_CodeLayer_X86_UCodeVM_Name "UCodeVm-x86"
#define UCode_CodeLayer_X86_64_UCodeVM_Name "UCodeVm-x86_64"
#define UCode_CodeLayer_AMD_UCodeVM_Name "UCodeVm-AMD"
#define UCode_CodeLayer_AMD_64_UCodeVM_Name "UCodeVm-AMD64"


#define UCode_LangType_UCodeLang ".uc"
