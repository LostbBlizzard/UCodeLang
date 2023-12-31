#pragma once


#define UCodeLangAPIExport

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

#define UCodeLangCAPI_Hardtypedef(OldName,NewName) struct NewName\
{ \
    OldName Base; \
}; \

#define UCodeLangCAPI_HardtypedefVoid(OldName,NewName) struct NewName \
{ \
    void* Base_Dont_Touch; \
}; \

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
    typedef const UCodeLangCStruct UCodeLangCAPI_Compiler UCodeLangCAPI_Const_Compiler;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_Interpreter);
    typedef const UCodeLangCStruct UCodeLangCAPI_Interpreter UCodeLangCAPI_Const_Interpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_JitInterpreter);
    typedef const UCodeLangCStruct UCodeLangCAPI_JitInterpreter UCodeLangCAPI_Const_JitInterpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_NativeInterpreter);
    typedef const UCodeLangCStruct UCodeLangCAPI_NativeInterpreter UCodeLangCAPI_Const_NativeInterpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_AnyInterpreter);
    typedef const UCodeLangCStruct UCodeLangCAPI_AnyInterpreter UCodeLangCAPI_Const_AnyInterpreter;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_AnyInterpreterPtr);
    typedef const UCodeLangCStruct UCodeLangCAPI_AnyInterpreterPtr UCodeLangCAPI_Const_AnyInterpreterPtr;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_RunTimeLangState);
    typedef const UCodeLangCStruct UCodeLangCAPI_RunTimeLangState UCodeLangCAPI_Const_RunTimeLangState;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_UClib);
    typedef const UCodeLangCStruct UCodeLangCAPI_UClib UCodeLangCAPI_Const_UClib;

    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_RunTimeUClib);
    typedef const UCodeLangCStruct UCodeLangCAPI_RunTimeUClib UCodeLangCAPI_Const_RunTimeUClib;

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
    
    enum UCodeLangCAPI_RegisterID : UCodeLangCAPI_RegisterID_t
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

            StartParameterRegister = (UCodeLangCAPI_RegisterID_t)D,//the range the runtime will pass function Parameters into Registers
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

        enum UCodeLangCAPI_RegisterID AsRegister;

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


    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_ClassMethod);
    typedef const UCodeLangCStruct UCodeLangCAPI_ClassMethod UCodeLangCAPI_Const_ClassMethod;


    UCodeLangCAPI_HardtypedefVoid(void, UCodeLangCAPI_InterpreterCPPinterface);
    typedef const UCodeLangCStruct UCodeLangCAPI_InterpreterCPPinterface UCodeLangCAPI_Const_InterpreterCPPinterface;

    typedef void(*UCodeLangCAPI_CppCall)(UCodeLangCStruct UCodeLangCAPI_InterpreterCPPinterface* cppinterface);


    //functions 
    UCodeLangAPIExport uint32_t UCodeLangCAPI_VersionMajor();
    UCodeLangAPIExport uint32_t UCodeLangCAPI_VersionMinor();
    UCodeLangAPIExport uint32_t UCodeLangCAPI_VersionPatch();
    UCodeLangAPIExport uint32_t UCodeLangCAPI_VersionNumber();
#ifndef UCodeLangNoCompiler
    //Compile
    UCodeLangCStruct UCodeLangCAPI_Compiler* UCodeLangAPIExport UCodeLangCAPI_New_Compiler();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_Compiler(UCodeLangCStruct UCodeLangCAPI_Compiler* Value);
    UCodeLangCStruct UCodeLangCAPI_CompilerOutput UCodeLangAPIExport UCodeLangCAPI_Compiler_CompileText(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_CharSpan String);
    UCodeLangCStruct UCodeLangCAPI_CompilerOutput UCodeLangAPIExport UCodeLangCAPI_Compiler_CompileFile(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan path);
    UCodeLangCStruct UCodeLangCAPI_CompilerOutput UCodeLangAPIExport UCodeLangCAPI_Compiler_CompileFileToLib(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan path, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan outpath);
    UCodeLangCStruct UCodeLangCAPI_CompilerOutput UCodeLangAPIExport UCodeLangCAPI_Compiler_CompileDir(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan dirpath, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan outpath);
    UCodeLangCStruct UCodeLangCAPI_CompilerOutput UCodeLangAPIExport UCodeLangCAPI_Compiler_CompileDirWithInt(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan dirpath, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan intpath, UCodeLangCStruct UCodeLangCAPI_Const_PathSpan outpath);

    UCodeLangCStruct UCodeLangCAPI_Const_CharSpan UCodeLangAPIExport UCodeLangCAPI_Compiler_OutputFileExt(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    void UCodeLangAPIExport UCodeLangCAPI_ResetCompilerSetings(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    void UCodeLangAPIExport UCodeLangCAPI_CompilerSetingsAddFlag(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_CharSpan Flag);
    void UCodeLangAPIExport UCodeLangCAPI_CompilerSetingsAddFlag2(UCodeLangCStruct UCodeLangCAPI_Compiler* This, UCodeLangCStruct UCodeLangCAPI_Const_CharSpan Flag, UCodeLangCStruct UCodeLangCAPI_Const_CharSpan FlagValue);


    void UCodeLangAPIExport UCodeLangCAPI_SetCompilerSetingsDebugFlag(UCodeLangCStruct UCodeLangCAPI_Compiler* This, bool value);
    void UCodeLangAPIExport UCodeLangCAPI_SetCompilerSetingsO1Flag(UCodeLangCStruct UCodeLangCAPI_Compiler* This, bool value);
    void UCodeLangAPIExport UCodeLangCAPI_SetCompilerSetingsO2Flag(UCodeLangCStruct UCodeLangCAPI_Compiler* This, bool value);



    void UCodeLangAPIExport UCodeLangCAPI_SetCompilerSetings64Mode(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetCompilerSetings32Mode(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    //Main BackEnds
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndUCode(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndC11(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndWasm(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    //Native Plantfrom BackEnds
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndWindows(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndLinux(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndMacOs(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndToNative(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndToUCodeNative(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    //Native Interpreter
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndToUCodeNativeX86(UCodeLangCStruct UCodeLangCAPI_Compiler* This);
    void UCodeLangAPIExport UCodeLangCAPI_SetBackEndToUCodeNativeArm(UCodeLangCStruct UCodeLangCAPI_Compiler* This);

    //

#endif

//RunTimeState

    UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* UCodeLangAPIExport UCodeLangCAPI_New_RunTimeState();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_RunTimeState(UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* Value);


    void UCodeLangAPIExport UCodeLangCAPI_RunTimeState_AddLib(UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* Value, UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* lib);
    void UCodeLangAPIExport UCodeLangCAPI_RunTimeState_LinkLibs(UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* Value);
    void UCodeLangAPIExport UCodeLangCAPI_RunTimeState_ClearRunTimeState(UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* Value);


    //Interpreter
    UCodeLangCStruct UCodeLangCAPI_Interpreter* UCodeLangAPIExport UCodeLangCAPI_New_Interpreter();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_Interpreter(UCodeLangCStruct UCodeLangCAPI_Interpreter* Value);


    void UCodeLangAPIExport UCodeLangCAPI_Interpreter_Init(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime);
    void UCodeLangAPIExport UCodeLangCAPI_Interpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_Interpreter* This);

    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_Call(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCStruct UCodeLangCAPI_CharSpan* FuncName);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_CallA(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCAPI_UAddress address);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_CallM(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCStruct UCodeLangCAPI_Const_ClassMethod* method);

    void UCodeLangAPIExport UCodeLangCAPI_Interpreter_GetReturn(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, void* Output, size_t Outputsize);

    void UCodeLangAPIExport UCodeLangCAPI_Interpreter_PushParameter(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, const void* object, size_t objectsize);

    //Jit Interpreter
    UCodeLangCStruct UCodeLangCAPI_JitInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_JitInterpreter();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_JitInterpreter(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* Value);


    void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime);
    void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This);

    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_Call(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCStruct UCodeLangCAPI_CharSpan* FuncName);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCAPI_UAddress address);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCStruct UCodeLangCAPI_Const_ClassMethod* method);

    void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_GetReturn(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, void* Output, size_t Outputsize);

    void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_PushParameter(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, const void* object, size_t objectsize);

    //Native Interpreter
    UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_NativeInterpreter();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_NativeInterpreter(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* Value);


    void UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime);
    void UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This);

    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_Call(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCStruct UCodeLangCAPI_CharSpan* FuncName);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCAPI_UAddress address);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCStruct UCodeLangCAPI_Const_ClassMethod* method);

    void UCodeLangAPIExport  UCodeLangCAPI_NativeInterpreter_GetReturn(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, void* Output, size_t Outputsize);

    void UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_PushParameter(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, const void* object, size_t objectsize);

    //Any Interpreter
    UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_AnyInterpreter();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_AnyInterpreter(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* Value);


    void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime);
    void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This);

    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_Call(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCStruct UCodeLangCAPI_CharSpan* FuncName);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCAPI_UAddress address);
    UCodeLangCStruct UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCStruct UCodeLangCAPI_Const_ClassMethod* method);

    void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_GetReturn(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, void* Output, size_t Outputsize);

    void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_PushParameter(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, const void* object, size_t objectsize);


    //RunTimeLib

    UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* UCodeLangAPIExport UCodeLangCAPI_New_RunTimelib();
    void UCodeLangAPIExport UCodeLangCAPI_Destroy_RunTimelib(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value);

    void UCodeLangAPIExport UCodeLangCAPI_RunTimeUClib_Init(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value, UCodeLangCStruct UCodeLangCAPI_UClib* lib);
    void UCodeLangAPIExport UCodeLangCAPI_RunTimeUClib_Unload(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value);

    void UCodeLangAPIExport UCodeLangCAPI_RunTimeUClib_AddCppCall(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value, UCodeLangCStruct UCodeLangCAPI_Const_CharSpan funcname, UCodeLangCAPI_CppCall cppcallfuncion, void* nativefuncion);

    struct IOLinkSettings
    {
        bool AddCoutACin;
    };

    void UCodeLangAPIExport UCodeLangCAPI_RunTimeLib_LinkIO(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value, const UCodeLangCStruct IOLinkSettings* setting);
    void UCodeLangAPIExport UCodeLangCAPI_RunTimeLib_LinkIOSandBox(UCodeLangCStruct UCodeLangCAPI_RunTimeUClib* Value, const UCodeLangCStruct IOLinkSettings* setting);

    // InterpreterCPPinterface
    void UCodeLangAPIExport UCodeLangCAPI_InterpreterCPPinterface_GetParamter(UCodeLangCStruct UCodeLangCAPI_InterpreterCPPinterface* value, void* output, size_t outsize);
    void UCodeLangAPIExport UCodeLangCAPI_InterpreterCPPinterface_SetReturn(UCodeLangCStruct UCodeLangCAPI_InterpreterCPPinterface* value, void* input, size_t inputsize);


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