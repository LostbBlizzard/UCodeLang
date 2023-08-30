#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"

//C includes
#include  <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//A C API that targets C89

#ifdef __cplusplus
#define UCodeLangCStruct 
#else 
#define UCodeLangCStruct struct
#endif


//UCodeLangCAPIStart
#ifdef __cplusplus
extern "C"
{
#endif

#define UCodeLangCAPI_KeepInHFile inline
#define UCodeLangCAPI_Nullptr ((void*)0)
#define UCodeLangCAPI_NullptrT(T) ((T*)0)

#define UCodeLangCAPI_Hardtypedef(OldName,NewName) typedef struct \
{ \
    OldName Base; \
} NewName; \

#define UCodeLangCAPI_HardtypedefVoid(OldName,NewName) typedef struct \
{ \
    void* Base_Dont_Touch; \
} NewName; \

    //translation convention

    //type defs

    typedef uint8_t UCodeLangCAPI_Byte;
    typedef char UCodeLangCAPI_PathChar;

    typedef uintptr_t UCodeLangCAPI_UAddress;
    typedef void* UCodeLangCAPI_PtrType;

    typedef int8_t UCodeLangCAPI_Int8;
    typedef int16_t UCodeLangCAPI_Int16;
    typedef int32_t UCodeLangCAPI_Int32;
    typedef int64_t UCodeLangCAPI_Int64;

    typedef uint8_t UCodeLangCAPI_UInt8;
    typedef uint16_t UCodeLangCAPI_UInt16;
    typedef uint32_t  UCodeLangCAPI_UInt32;
    typedef uint64_t UCodeLangCAPI_UInt64;

    typedef intptr_t UCodeLangCAPI_SIntNative;
    typedef uintptr_t UCodeLangCAPI_UIntNative;

    typedef float UCodeLangCAPI_float32;
    typedef double UCodeLangCAPI_float64;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_Compiler);
    typedef const UCodeLangCAPI_Compiler UCodeLangCAPI_Const_Compiler;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_Interpreter);
    typedef const UCodeLangCAPI_Interpreter UCodeLangCAPI_Const_Interpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_JitInterpreter);
    typedef const UCodeLangCAPI_Interpreter UCodeLangCAPI_Const_JitInterpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_NativeInterpreter);
    typedef const UCodeLangCAPI_Interpreter UCodeLangCAPI_Const_NativeInterpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_AnyInterpreter);
    typedef const UCodeLangCAPI_Interpreter UCodeLangCAPI_Const_AnyInterpreter;


    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_RunTimeLangState);
    typedef const UCodeLangCAPI_RunTimeLangState UCodeLangCAPI_Const_RunTimeLangState;




    struct UCodeLangCAPI_AnyInterpreterPtr
    {
        void* Base;
    };
    typedef const UCodeLangCStruct UCodeLangCAPI_AnyInterpreterPtr UCodeLangCAPI_Const_AnyInterpreterPtr;

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
        UCodeLangCAPI_Byte* pointer;
        size_t size;
    };
    struct UCodeLangCAPI_Const_ByteSpan
    {
        const UCodeLangCAPI_Byte* pointer;
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
        UCodeLangCAPI_Byte* data;
        size_t size;
    };

    struct UCodeLangCAPI_CompilerOutput
    {
        bool Success;
    };
    typedef UCodeLangCAPI_Byte UCodeLangCAPI_RegisterID_t;
    struct UCodeLangCAPI_RegisterID
    {
        enum
        {
            A, B, C, D, E, F,

            //
            StartRegister = (UCodeLangCAPI_RegisterID_t)A,
            EndRegister = (UCodeLangCAPI_RegisterID_t)F,


            ThisRegister = (UCodeLangCAPI_RegisterID_t)D,
            InPutRegister = (UCodeLangCAPI_RegisterID_t)E,
            OuPutRegister = (UCodeLangCAPI_RegisterID_t)F,

            MathOuPutRegister = OuPutRegister,
            BoolRegister = OuPutRegister,
            BitwiseRegister = OuPutRegister,

            StartParameterRegister = (UCodeLangCAPI_RegisterID_t)D,//the range the runtime will pass funcion Parameters into Registers
            EndParameterRegister = (UCodeLangCAPI_RegisterID_t)F + 1,


        };
    };
    union UCodeLangCAPI_AnyInt64
    {
        UCodeLangCAPI_UInt64 Value;

        bool Asbool;
        UCodeLangCAPI_Int8  AsInt8;
        UCodeLangCAPI_Int16 AsInt16;
        UCodeLangCAPI_Int32 AsInt32;
        UCodeLangCAPI_Int64 AsInt64;

        UCodeLangCAPI_UInt8  AsUInt8;
        UCodeLangCAPI_UInt16 AsUInt16;
        UCodeLangCAPI_UInt32 AsUInt32;
        UCodeLangCAPI_UInt64 AsUInt64;

        UCodeLangCAPI_float32 Asfloat32;
        UCodeLangCAPI_float64 Asfloat64;

        UCodeLangCStruct UCodeLangCAPI_RegisterID AsRegister;

        UCodeLangCAPI_UIntNative AsUIntNative;
        UCodeLangCAPI_PtrType AsPtr;
        UCodeLangCAPI_UAddress AsAddress;
    };


    struct UCodeLangCAPI_Interpreter_RetState
    {
        enum
        {
            Null,
            Success,
            Error,
            Error_Function_doesnt_exist,
        } type;
    };

    struct UCodeLangCAPI_Interpreter_Register
    {
        union UCodeLangCAPI_AnyInt64 Value;
    };
    struct UCodeLangCAPI_Interpreter_Return_t
    {
        UCodeLangCStruct UCodeLangCAPI_Interpreter_RetState _Succeed;
        UCodeLangCStruct UCodeLangCAPI_Interpreter_Register ReturnValue;
    };


    //funcions

    void UCodeLangAPIExport UCodeLangCAPI_Init();
    void UCodeLangAPIExport UCodeLangCAPI_DeInit();


    UCodeLangCStruct UCodeLangCAPI_Compiler* UCodeLangAPIExport UCodeLangCAPI_New_Compiler();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_Compiler(UCodeLangCStruct UCodeLangCAPI_Compiler* Value);
    void UCodeLangAPIExport UCodeLangCAPI_Compiler_Build(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_CharSpan String, UCodeLangCStruct UCodeLangCAPI_CompilerOutput* Output);

    UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* UCodeLangAPIExport UCodeLangCAPI_New_RunTimeState();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_RunTimeState(UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* Value);

    UCodeLangCStruct UCodeLangCAPI_Interpreter* UCodeLangAPIExport UCodeLangCAPI_New_Interpreter();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_Interpreter(UCodeLangCStruct UCodeLangCAPI_Interpreter* Value);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_Call(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCStruct UCodeLangCAPI_CharSpan* FuncName);

    //types
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpan(UCodeLangCStruct UCodeLangCAPI_CharSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(char);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpanV(UCodeLangCStruct UCodeLangCAPI_CharSpan* This, char* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_CharSpanV2(UCodeLangCStruct UCodeLangCAPI_CharSpan* This, char* CString)
    {
        UCodeLangCAPI_New_CharSpanV(This, CString, strlen(CString));
    }

    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_ByteSpan(UCodeLangCStruct UCodeLangCAPI_ByteSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_Byte);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_ByteSpanV(UCodeLangCStruct UCodeLangCAPI_ByteSpan* This, UCodeLangCAPI_Byte* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }


    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpan(UCodeLangCStruct UCodeLangCAPI_PathSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpanV(UCodeLangCStruct UCodeLangCAPI_PathSpan* This, UCodeLangCAPI_PathChar* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_New_PathSpanV2(UCodeLangCStruct UCodeLangCAPI_PathSpan* This, UCodeLangCAPI_PathChar* CString)
    {
        UCodeLangCAPI_New_PathSpanV(This, CString, strlen(CString));
    }


    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpan(UCodeLangCStruct UCodeLangCAPI_Const_PathSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpanV(UCodeLangCStruct UCodeLangCAPI_Const_PathSpan* This, const UCodeLangCAPI_PathChar* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_PathSpanV2(UCodeLangCStruct UCodeLangCAPI_Const_PathSpan* This, const UCodeLangCAPI_PathChar* CString)
    {
        UCodeLangCAPI_Const_New_PathSpanV(This, CString, strlen(CString));
    }

    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpan(UCodeLangCStruct UCodeLangCAPI_Const_CharSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_PathChar);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpanV(UCodeLangCStruct UCodeLangCAPI_Const_CharSpan* This, const char* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_CharSpanV2(UCodeLangCStruct UCodeLangCAPI_Const_CharSpan* This, const char* CString)
    {
        UCodeLangCAPI_Const_New_CharSpanV(This, CString, strlen(CString));
    }


    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_ByteSpan(UCodeLangCStruct UCodeLangCAPI_Const_ByteSpan* This)
    {
        This->pointer = UCodeLangCAPI_NullptrT(UCodeLangCAPI_Byte);
        This->size = 0;
    }
    void UCodeLangCAPI_KeepInHFile UCodeLangCAPI_Const_New_ByteSpanV(UCodeLangCStruct UCodeLangCAPI_Const_ByteSpan* This, const UCodeLangCAPI_Byte* Buffer, size_t Size)
    {
        This->pointer = Buffer;
        This->size = Size;
    }

    UCodeLangCStruct UCodeLangCAPI_Const_PathSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_PathSpan_ToConst(UCodeLangCStruct UCodeLangCAPI_PathSpan* This)
    {
        UCodeLangCStruct UCodeLangCAPI_Const_PathSpan r;
        UCodeLangCAPI_Const_New_PathSpanV(&r, This->pointer, This->size);
        return r;
    }
    UCodeLangCStruct UCodeLangCAPI_Const_PathSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_ByteSpan_ToConst(UCodeLangCStruct UCodeLangCAPI_PathSpan* This)
    {
        UCodeLangCStruct UCodeLangCAPI_Const_PathSpan r;
        UCodeLangCAPI_Const_New_PathSpanV(&r, This->pointer, This->size);
        return r;
    }
    UCodeLangCStruct UCodeLangCAPI_Const_CharSpan UCodeLangCAPI_KeepInHFile UCodeLangCAPI_CharSpan_ToConst(UCodeLangCStruct UCodeLangCAPI_CharSpan* This)
    {
        UCodeLangCStruct UCodeLangCAPI_Const_CharSpan r;
        UCodeLangCAPI_Const_New_CharSpanV(&r, This->pointer, This->size);
        return r;
    }


    //UCodeLangCAPIEnd
#ifdef __cplusplus
}
#endif