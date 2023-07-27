#pragma once
#include "LangCore/LangDef.hpp"
#include  <inttypes.h>


//A CAPI that targets C89

#define UCodeLangCAPIStart 
#define UCodeLangCAPIEnd


UCodeLangCAPIStart


#define UCodeLangCAPI_KeepInHFile inline
#define UCodeLangCAPI_Nullptr ((void*)0)
#define UCodeLangCAPI_NullptrT(T) ((T*)0)


//translation convention

//type defs

typedef uint8_t UCodeLangCAPI_byte;
typedef char UCodeLangCAPI_PathChar;

typedef void  UCodeLangCAPI_Compiler;
typedef const UCodeLangCAPI_Compiler UCodeLangCAPI_Const_Compiler;

//structs
struct UCodeLangCAPI_CharSpan
{
    char* pointer;
    size_t size;
};
struct UCodeLangCAPI_Const_CharSpan
{
    const char* pointer;
    size_t size;
};

struct UCodeLangCAPI_PathSpan
{
    UCodeLangCAPI_PathChar* pointer;
    size_t size;
};
struct UCodeLangCAPI_Const_PathSpan
{
    const UCodeLangCAPI_PathChar* pointer;
    size_t size;
};

struct UCodeLangCAPI_ByteSpan
{
    UCodeLangCAPI_byte* pointer;
    size_t size;
};
struct UCodeLangCAPI_Const_ByteSpan
{
    const UCodeLangCAPI_byte* pointer;
    size_t size;
};

struct UCodeLangCAPI_String
{
    char* data;
    size_t size;
};
struct UCodeLangCAPI_Path
{
    UCodeLangCAPI_PathChar* data;
    size_t size;
};
struct UCodeLangCAPI_BytesPtr
{
    UCodeLangCAPI_byte* data;
    size_t size;
};

struct UCodeLangCAPI_CompilerOutput
{
    bool Success;
};

//funcions

void UCodeLangAPIExport UCodeLangCAPI_Init();
void UCodeLangAPIExport UCodeLangCAPI_DeInit();


UCodeLangCAPI_Compiler* UCodeLangAPIExport UCodeLangCAPI_New_Compiler();
void UCodeLangAPIExport UCodeLangCAPI_Destroy_Compiler(UCodeLangCAPI_Compiler* Value);
void UCodeLangAPIExport UCodeLangCAPI_Compiler_Build(UCodeLangCAPI_Compiler* This, UCodeLangCAPI_CharSpan String, UCodeLangCAPI_CompilerOutput* Output);


//types
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpan(UCodeLangCAPI_CharSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(char);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpanV(UCodeLangCAPI_CharSpan* This,char* Buffer,size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpanV2(UCodeLangCAPI_CharSpan* This, char* CString)
{
    UCodeLangCAPI_New_CharSpanV(This, CString, strlen(CString));
}

void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_ByteSpan(UCodeLangCAPI_ByteSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_byte);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_ByteSpanV(UCodeLangCAPI_ByteSpan* This,UCodeLangCAPI_byte* Buffer, size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}


void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpan(UCodeLangCAPI_PathSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpanV(UCodeLangCAPI_PathSpan* This, UCodeLangCAPI_PathChar* Buffer, size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpanV2(UCodeLangCAPI_PathSpan* This, UCodeLangCAPI_PathChar* CString)
{
    UCodeLangCAPI_New_PathSpanV(This, CString, strlen(CString));
}


void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpan(UCodeLangCAPI_Const_PathSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpanV(UCodeLangCAPI_Const_PathSpan* This,const UCodeLangCAPI_PathChar* Buffer, size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpanV2(UCodeLangCAPI_Const_PathSpan* This,const UCodeLangCAPI_PathChar* CString)
{
    UCodeLangCAPI_Const_New_PathSpanV(This, CString, strlen(CString));
}

void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpan(UCodeLangCAPI_Const_CharSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpanV(UCodeLangCAPI_Const_CharSpan* This, const char* Buffer, size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpanV2(UCodeLangCAPI_Const_CharSpan* This, const char* CString)
{
    UCodeLangCAPI_Const_New_CharSpanV(This, CString, strlen(CString));
}


void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_ByteSpan(UCodeLangCAPI_Const_ByteSpan* This)
{
    This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_byte);
    This->size = 0;
}
void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_ByteSpanV(UCodeLangCAPI_Const_ByteSpan* This, const UCodeLangCAPI_byte* Buffer, size_t Size)
{
    This->pointer = Buffer;
    This->size = Size;
}

UCodeLangCAPI_Const_PathSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_PathSpan_ToConst(UCodeLangCAPI_PathSpan* This)
{
    UCodeLangCAPI_Const_PathSpan r;
    UCodeLangCAPI_Const_New_PathSpanV(&r,This->pointer, This->size);
    return r;
}
UCodeLangCAPI_Const_PathSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_ByteSpan_ToConst(UCodeLangCAPI_PathSpan* This)
{
    UCodeLangCAPI_Const_PathSpan r;
    UCodeLangCAPI_Const_New_PathSpanV(&r, This->pointer, This->size);
    return r;
}
UCodeLangCAPI_Const_CharSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_CharSpan_ToConst(UCodeLangCAPI_CharSpan* This)
{
    UCodeLangCAPI_Const_CharSpan r;
    UCodeLangCAPI_Const_New_CharSpanV(&r, This->pointer, This->size);
    return r;
}


UCodeLangCAPIEnd