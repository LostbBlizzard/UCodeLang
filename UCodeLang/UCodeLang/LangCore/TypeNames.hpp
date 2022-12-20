#pragma once
#define EnumBaseTypeName "base_t";
#define Uint8TypeName "uint8"
#define Sint8TypeName "sint8"
#define CharTypeName "char"
#define boolTypeName "bool"


#define Uint8TypeName32 Uint8TypeName
#define Uint8TypeName64 Uint8TypeName

#define UintPtrTypeName "uintptr"
#define SintPtrTypeName "sintptr"

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