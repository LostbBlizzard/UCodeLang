#pragma once
//#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
#define EnumBaseTypeName "base_t";

#define CharTypeName UCodeLang::S_KeyWord_char
#define boolTypeName UCodeLang::S_KeyWord_bool

#define Uint8TypeName UCodeLang::S_KeyWord_uInt8
#define Sint8TypeName UCodeLang::S_KeyWord_Int8

#define Uint16TypeName UCodeLang::S_KeyWord_uInt16
#define Sint16TypeName UCodeLang::S_KeyWord_Int16

#define Uint32TypeName UCodeLang::S_KeyWord_uInt32
#define Sint32TypeName UCodeLang::S_KeyWord_Int32

#define Uint64TypeName UCodeLang::S_KeyWord_uInt64
#define Sint64TypeName UCodeLang::S_KeyWord_Int64

#define UintPtrTypeName UCodeLang::S_KeyWord_uintptr
#define SintPtrTypeName UCodeLang::S_KeyWord_sintptr

#define VoidPtrTypeName "VoidPtr"
#define CStringTypeName "CString"

#define CompilerGeneratedStart "(&"
#define CompilerGeneratedEnd "&)"
#define CompilerGenerated(x) CompilerGeneratedStart x CompilerGeneratedEnd

#define ClassInitializefuncName CompilerGenerated("init")
#define ClassInitializefuncNameC(ClassName) ClassName + "::" ClassInitializefuncName

#define ThisSymbolName CompilerGenerated("this")

#define StaticVariablesInitializeFunc CompilerGenerated("StaticInit")
#define StaticVariablesUnLoadFunc CompilerGenerated("StaticUnload")

#define StandardLibraryName "UCode"

#define StandardLibraryLibName StandardLibraryName ".lib"

#define DLLCallAttributeName "DLL"