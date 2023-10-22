/*Made using UCodeLang C89 Backend.


*/
/*defs*/

#if defined(_MSC_VER)
#define UCIR_Compiler_MSVC 1 
#else
#define UCIR_Compiler_MSVC 0 
#endif

#if defined(__GNUC__)
#define UCIR_Compiler_GCC 1 
#else
#define UCIR_Compiler_GCC 0 
#endif

#if defined(__clang__)
#define UCIR_Compiler_Clang 1 
#else
#define UCIR_Compiler_Clang 0 
#endif

#if _WIN64 || _WIN32
#define UCIR_Platform_Windows 1 
#else
#define UCIR_Platform_Windows 0 
#endif

#if __gnu_linux__ || __linux__
#define UCIR_Platform_Linux 1 
#else
#define UCIR_Platform_Linux 0 
#endif

#if __APPLE__ && __MACH__
#define UCIR_Platform_MacOS 1 
#else
#define UCIR_Platform_MacOS 0 
#endif

#if __APPLE__
#include "TargetConditionals.h";
#if TARGET_OS_IPHONE
#define UCIR_Platform_IPHONE 1 
#else
#define UCIR_Platform_IPHONE 0 
#endif
#else
#define UCodeLang_Platform_IPHONE 0
#endif

#if __ANDROID__
#define UCIR_Platform_ANDROID 1 
#else
#define UCIR_Platform_ANDROID 0 
#endif

#if defined(__wasm32__) || defined(__wasm64__)
#define UCIR_Platform_Wasm 1 
#else
#define UCIR_Platform_Wasm 0 
#endif

#if UCIR_Compiler_MSVC
#define UCIR_Forceinline __forceinline
#elif UCIR_Compiler_GCC
#define UCIR_Forceinline __attribute__((always_inline))
#else
#define UCIR_Forceinline inline
#endif

#define ThreadLocalKeyWord 
#if UCIR_Compiler_MSVC
#define UCIR_Unreachable __assume(0)
#elif UCIR_Compiler_GCC
#define UCIR_Unreachable __builtin_unreachable()
#else
#define UCIR_Unreachable 
#endif

#if UCIR_Compiler_MSVC
#define UCIR_Assume(X) __assume(X)
#elif UCIR_Compiler_Clang
#define UCIR_Assume(X) __builtin_assume(X)
#else
#define UCIR_Assume(X) 
#endif

/*includes*/

#include <inttypes.h>
#include <stdlib.h>
/*Types*/
typedef float float32_t;
typedef double float64_t;



/*file.h*/

typedef int32_t(*_tepfptr_ULang_Debug_ToString)();

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} _Class;

typedef void(*_tepfptr_ULang_Debug____drop__)(_Class*);

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Compiler;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Empty;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_LowLevel;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Math;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Fmt;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_OpenedFile;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_OpenFileError;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_File;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Writer;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Future;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Thread;

typedef struct 
{
 int __Empty; /*C requires that a struct or union has at least one member*/
} ULang_Path;

typedef struct 
{
 int64_t __0; 
 int64_t __1; 

} ULang_Range_t_uintptr_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_char_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_char_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_char_;

typedef struct 
{
 ULang_OpenedFile __0; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Value_;

typedef struct 
{
 ULang_OpenFileError __0; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Error_;

typedef struct 
{
 ULang_OpenFileError __0; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError__Error_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_uint8_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_uint8_;

typedef struct 
{
 ULang_Vector_uint8_ __0; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Value_;

typedef struct 
{
 ULang_OpenFileError __0; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Error_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_sint32_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_sint32_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_uft8_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_uft8_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_uft8_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_uft16_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_uft16_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_uft16_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} ULang_Vector_uft32_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_Span_uft32_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_uft32_;

typedef struct 
{
 float32_t __0; 
 float32_t __1; 

} ULang_Vec2f_t_float32_;

typedef struct 
{
 int32_t __0; 
 int32_t __1; 

} ULang_Vec2f_t_sint32_;

typedef struct 
{
 float32_t __0; 
 float32_t __1; 
 float32_t __2; 

} ULang_Vec3f_t_float32_;

typedef struct 
{
 int32_t __0; 
 int32_t __1; 
 int32_t __2; 

} ULang_Vec3f_t_sint32_;

typedef struct 
{
 ULang_Empty* __0; 

} ULang_Ptr_ULang_Empty_;

typedef struct 
{
 void* __0; 

} ULang_Ptr_sint32_;

typedef struct 
{
 ULang_Vec2f_t_float32_* __0; 

} ULang_Ptr_ULang_Vec2f_t_float32__;

typedef struct 
{
 int32_t __0; 

} ULang_Debug_Class;

static int8_t _Const_SpanString_Hello_World[] = {72,101,108,108,111,32,87,111,114,108,100};
static int8_t _Const_SpanString_Hello[] = {72,101,108,108,111};
static int8_t _Const_SpanString_World[] = {87,111,114,108,100};
typedef int32_t(*_tepfptr_ULang_Debug_ToStringV2)(_Class*);

typedef struct 
{
 ULang_Vector_char_ __0; 

} ULang_String_t_char_;

typedef union 
{
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError_u;

typedef struct 
{
 ULang_String_t_char_ __0; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError__Value_;

typedef union 
{
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_u;

typedef struct 
{
 ULang_Vector_uft8_ __0; 

} ULang_String_t_uft8_;

typedef struct 
{
 ULang_Vector_uft16_ __0; 

} ULang_String_t_uft16_;

typedef struct 
{
 ULang_Vector_uft32_ __0; 

} ULang_String_t_uft32_;

typedef struct 
{
 _tepfptr_ULang_Debug_ToString __0; 
 _tepfptr_ULang_Debug_ToStringV2 __1; 
 _tepfptr_ULang_Debug____drop__ __2; 

} ULang_Debugvtable_type;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError_u __1; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError_;

typedef union 
{
 ULang_Result_ULang_String_t_char__ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_String_t_char__ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError_u;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_u __1; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_String_t_char__ULang_OpenFileError_u __1; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int8_t ULang_Compiler_Is64Bit();
int8_t ULang_Compiler_Is32Bit();
int8_t ULang_Compiler_IsBackendC89();
int8_t ULang_Compiler_IsBackendUCodeVM();
int8_t ULang_Compiler_IsBackendWasm();
int8_t ULang_Compiler_IsBackendVM();
void ULang_Compiler____new___ULang_Compiler_(ULang_Compiler* __this__);
void ULang_Empty____new___ULang_Empty_(ULang_Empty* __this__);
void ULang_LowLevel_Malloc_uptr(int64_t Size);
void ULang_LowLevel_Free_u8___(void* Block);
void ULang_LowLevel_Memcopy_u8____u8____uptr(void* destination,void* source,int64_t num);
void ULang_LowLevel_Memmove_u8____u8____uptr(void* destination,void* source,int64_t num);
void ULang_LowLevel____new___ULang_LowLevel_(ULang_LowLevel* __this__);
void ULang_Math____new___ULang_Math_(ULang_Math* __this__);
void ULang_Fmt_Print_c(int8_t Str);
void ULang_Fmt_Print_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str);
void ULang_Fmt_Print_ULang_String_t_char__imut(ULang_String_t_char_* Str);
void ULang_Fmt_Println_c(int8_t Str);
void ULang_Fmt_Println_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str);
void ULang_Fmt_Println_ULang_String_t_char__imut(ULang_String_t_char_* Str);
void ULang_Fmt____new___ULang_Fmt_(ULang_Fmt* __this__);
void ULang_OpenedFile____new___ULang_OpenedFile_(ULang_OpenedFile* __this__);
void ULang_OpenFileError____new___ULang_OpenFileError_(ULang_OpenFileError* __this__);
ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ ULang_File_Open_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path);
ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ ULang_File_Open_ULang_String_t_char__imut(ULang_String_t_char_* path);
ULang_Result_ULang_String_t_char__ULang_OpenFileError_ ULang_File_GetString_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path);
ULang_Result_ULang_String_t_char__ULang_OpenFileError_ ULang_File_GetString_ULang_String_t_char__imut(ULang_String_t_char_* path);
ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ ULang_File_GetBytes_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path);
ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ ULang_File_GetBytes_ULang_String_t_char__imut(ULang_String_t_char_* path);
void ULang_File____new___ULang_File_(ULang_File* __this__);
void ULang_Writer____new___ULang_Writer_(ULang_Writer* __this__);
int8_t ULang_Tests_Print1();
int8_t ULang_Tests_Print2();
int8_t ULang_Tests_Max_1();
int8_t ULang_Tests_Max_2();
int8_t ULang_Tests_Min_1();
int8_t ULang_Tests_Min_2();
int8_t ULang_Tests_StringSpan_1();
int8_t ULang_Tests_StringSpan_2();
int8_t ULang_Tests_StringSpan_3();
int8_t ULang_Tests_StringSpan_4();
int8_t ULang_Tests_Vector_1();
int8_t ULang_Tests_Vector_2();
void ULang_Future____new___ULang_Future_(ULang_Future* __this__);
void ULang_Thread____new___ULang_Thread_(ULang_Thread* __this__);
void ULang_Path____new___ULang_Path_(ULang_Path* __this__);
int8_t ULang_Tests_Ptr_1();
int8_t ULang_Tests_Ptr_2();
int8_t ULang_Tests_Ptr_3();
int8_t ULang_Tests_Ptr_4();
int8_t ULang_Tests_Ptr_5();
void ULang_Range_t_uintptr_____new___ULang_Range_t_uintptr__(ULang_Range_t_uintptr_* __this__);
void ULang_Range_t_uintptr_____new___ULang_Range_t_uintptr___uptr_uptr(ULang_Range_t_uintptr_* __this__,int64_t start,int64_t end);
int64_t ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(ULang_Range_t_uintptr_* __this__);
int64_t ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(ULang_Range_t_uintptr_* __this__);
void ULang_Span_char_____new___ULang_Span_char__(ULang_Span_char_* __this__);
void ULang_Span_char_____new___ULang_Span_char___c____uptr(ULang_Span_char_* __this__,void* data,int64_t size);
int64_t ULang_Span_char__Size_ULang_Span_char__imut(ULang_Span_char_* __this__);
void* ULang_Span_char__Data_ULang_Span_char__imut(ULang_Span_char_* __this__);
void* ULang_Span_char____Index___ULang_Span_char___uptr(ULang_Span_char_* __this__,int64_t Index);
void* ULang_Span_char____Index___ULang_Span_char__imut_uptr(ULang_Span_char_* __this__,int64_t Index);
ULang_Span_char_ ULang_Span_char____Index___ULang_Span_char___ULang_Range_t_uintptr_(ULang_Span_char_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_char_ ULang_Span_char____Index___ULang_Span_char__imut_ULang_Range_t_uintptr_(ULang_Span_char_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__);
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(ULang_StringSpan_t_char_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__);
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__);
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__);
int8_t ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__,ULang_StringSpan_t_char_* Other);
int8_t ULang_StringSpan_t_char____notequal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__,ULang_StringSpan_t_char_* Other);
ULang_Span_char_ ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__);
ULang_Span_char_ ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__);
ULang_StringSpan_t_char_ ULang_StringSpan_t_char__ToStr_ULang_Span_char__(ULang_Span_char_* span);
ULang_StringSpan_t_char_ ULang_StringSpan_t_char__ToStr_ULang_Span_char__imut(ULang_Span_char_* span);
void* ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___uptr(ULang_StringSpan_t_char_* __this__,int64_t Index);
void* ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char__imut_uptr(ULang_StringSpan_t_char_* __this__,int64_t Index);
ULang_StringSpan_t_char_ ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___ULang_Range_t_uintptr_(ULang_StringSpan_t_char_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_char_ ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_char_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_char_____new___ULang_Vector_char__(ULang_Vector_char_* __this__);
void ULang_Vector_char_____drop___ULang_Vector_char__(ULang_Vector_char_* __this__);
void* ULang_Vector_char__Data_ULang_Vector_char__imut(ULang_Vector_char_* __this__);
void* ULang_Vector_char__Data_ULang_Vector_char__(ULang_Vector_char_* __this__);
int64_t ULang_Vector_char__Size_ULang_Vector_char__imut(ULang_Vector_char_* __this__);
int64_t ULang_Vector_char__Capacity_ULang_Vector_char__imut(ULang_Vector_char_* __this__);
void ULang_Vector_char__Resize_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Size);
void ULang_Vector_char__Reserve_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Size);
void ULang_Vector_char__Clear_ULang_Vector_char__(ULang_Vector_char_* __this__);
int8_t ULang_Vector_char__Pop_ULang_Vector_char__(ULang_Vector_char_* __this__);
int8_t ULang_Vector_char__Remove_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Index);
void ULang_Vector_char__Push_ULang_Vector_char___c_imut(ULang_Vector_char_* __this__,void* Val);
void ULang_Vector_char__Insert_ULang_Vector_char___uptr_c_imut(ULang_Vector_char_* __this__,int64_t Index,void* Item);
void ULang_Vector_char__Append_ULang_Vector_char___ULang_Span_char_imut(ULang_Vector_char_* __this__,ULang_Span_char_ Val);
void* ULang_Vector_char____Index___ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Index);
void* ULang_Vector_char____Index___ULang_Vector_char__imut_uptr(ULang_Vector_char_* __this__,int64_t Index);
ULang_Span_char_ ULang_Vector_char____Index___ULang_Vector_char___ULang_Range_t_uintptr_(ULang_Vector_char_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_char_ ULang_Vector_char____Index___ULang_Vector_char__imut_ULang_Range_t_uintptr_(ULang_Vector_char_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_char_ ULang_Vector_char__AsSpan_ULang_Vector_char__(ULang_Vector_char_* __this__);
ULang_Span_char_ ULang_Vector_char__AsSpan_ULang_Vector_char__imut(ULang_Vector_char_* __this__);
void ULang_String_t_char_____new___ULang_String_t_char__(ULang_String_t_char_* __this__);
void ULang_String_t_char_____new___ULang_String_t_char___ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* span);
void* ULang_String_t_char__Data_ULang_String_t_char__imut(ULang_String_t_char_* __this__);
void* ULang_String_t_char__Data_ULang_String_t_char__(ULang_String_t_char_* __this__);
int64_t ULang_String_t_char__Size_ULang_String_t_char__imut(ULang_String_t_char_* __this__);
int64_t ULang_String_t_char__Capacity_ULang_String_t_char__imut(ULang_String_t_char_* __this__);
void ULang_String_t_char__Resize_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Size);
void ULang_String_t_char__Reserve_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Size);
void ULang_String_t_char__Clear_ULang_String_t_char__(ULang_String_t_char_* __this__);
int8_t ULang_String_t_char__Pop_ULang_String_t_char__(ULang_String_t_char_* __this__);
int8_t ULang_String_t_char__Remove_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Index);
void ULang_String_t_char__Push_ULang_String_t_char___c_imut(ULang_String_t_char_* __this__,void* Val);
void ULang_String_t_char__Insert_ULang_String_t_char___uptr_c_imut(ULang_String_t_char_* __this__,int64_t Index,void* Item);
void* ULang_String_t_char____Index___ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Index);
void* ULang_String_t_char____Index___ULang_String_t_char__imut_uptr(ULang_String_t_char_* __this__,int64_t Index);
void ULang_String_t_char__Append_ULang_String_t_char___ULang_Span_char_imut(ULang_String_t_char_* __this__,ULang_Span_char_ Val);
int8_t ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other);
int8_t ULang_String_t_char____notequal___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other);
ULang_String_t_char_ ULang_String_t_char____Plus___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other);
ULang_String_t_char_ ULang_String_t_char____Compound_Plus___ULang_String_t_char___ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other);
int8_t ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other);
int8_t ULang_String_t_char____notequal___ULang_String_t_char__imut_ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other);
void ULang_String_t_char____Compound_Plus___ULang_String_t_char___ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other);
ULang_Span_char_ ULang_String_t_char__AsSpan_ULang_String_t_char__(ULang_String_t_char_* __this__);
ULang_Span_char_ ULang_String_t_char__AsSpan_ULang_String_t_char__imut(ULang_String_t_char_* __this__);
ULang_StringSpan_t_char_ ULang_String_t_char__AsStrSpan_ULang_String_t_char__(ULang_String_t_char_* __this__);
ULang_StringSpan_t_char_ ULang_String_t_char__AsStrSpan_ULang_String_t_char__imut(ULang_String_t_char_* __this__);
ULang_StringSpan_t_char_ ULang_String_t_char____Index___ULang_String_t_char___ULang_Range_t_uintptr_(ULang_String_t_char_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_char_ ULang_String_t_char____Index___ULang_String_t_char__imut_ULang_Range_t_uintptr_(ULang_String_t_char_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_char_____drop___ULang_String_t_char__(ULang_String_t_char_* __this__);
void ULang_String_t_char_____de_init__(ULang_String_t_char_* __this__);
void ULang_Span_uint8_____new___ULang_Span_uint8__(ULang_Span_uint8_* __this__);
void ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(ULang_Span_uint8_* __this__,void* data,int64_t size);
int64_t ULang_Span_uint8__Size_ULang_Span_uint8__imut(ULang_Span_uint8_* __this__);
void* ULang_Span_uint8__Data_ULang_Span_uint8__imut(ULang_Span_uint8_* __this__);
void* ULang_Span_uint8____Index___ULang_Span_uint8___uptr(ULang_Span_uint8_* __this__,int64_t Index);
void* ULang_Span_uint8____Index___ULang_Span_uint8__imut_uptr(ULang_Span_uint8_* __this__,int64_t Index);
ULang_Span_uint8_ ULang_Span_uint8____Index___ULang_Span_uint8___ULang_Range_t_uintptr_(ULang_Span_uint8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uint8_ ULang_Span_uint8____Index___ULang_Span_uint8__imut_ULang_Range_t_uintptr_(ULang_Span_uint8_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_uint8_____new___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
void ULang_Vector_uint8_____drop___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
void* ULang_Vector_uint8__Data_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__);
void* ULang_Vector_uint8__Data_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
int64_t ULang_Vector_uint8__Size_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__);
int64_t ULang_Vector_uint8__Capacity_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__);
void ULang_Vector_uint8__Resize_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Size);
void ULang_Vector_uint8__Reserve_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Size);
void ULang_Vector_uint8__Clear_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
int8_t ULang_Vector_uint8__Pop_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
int8_t ULang_Vector_uint8__Remove_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Index);
void ULang_Vector_uint8__Push_ULang_Vector_uint8___u8_imut(ULang_Vector_uint8_* __this__,void* Val);
void ULang_Vector_uint8__Insert_ULang_Vector_uint8___uptr_u8_imut(ULang_Vector_uint8_* __this__,int64_t Index,void* Item);
void ULang_Vector_uint8__Append_ULang_Vector_uint8___ULang_Span_uint8_imut(ULang_Vector_uint8_* __this__,ULang_Span_uint8_ Val);
void* ULang_Vector_uint8____Index___ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Index);
void* ULang_Vector_uint8____Index___ULang_Vector_uint8__imut_uptr(ULang_Vector_uint8_* __this__,int64_t Index);
ULang_Span_uint8_ ULang_Vector_uint8____Index___ULang_Vector_uint8___ULang_Range_t_uintptr_(ULang_Vector_uint8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uint8_ ULang_Vector_uint8____Index___ULang_Vector_uint8__imut_ULang_Range_t_uintptr_(ULang_Vector_uint8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uint8_ ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
ULang_Span_uint8_ ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__);
int32_t ULang_Math_Max_sint32__i32_i32(int32_t A,int32_t B);
int32_t ULang_Math_Min_sint32__i32_i32(int32_t A,int32_t B);
void ULang_Span_sint32_____new___ULang_Span_sint32__(ULang_Span_sint32_* __this__);
void ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(ULang_Span_sint32_* __this__,void* data,int64_t size);
int64_t ULang_Span_sint32__Size_ULang_Span_sint32__imut(ULang_Span_sint32_* __this__);
void* ULang_Span_sint32__Data_ULang_Span_sint32__imut(ULang_Span_sint32_* __this__);
void* ULang_Span_sint32____Index___ULang_Span_sint32___uptr(ULang_Span_sint32_* __this__,int64_t Index);
void* ULang_Span_sint32____Index___ULang_Span_sint32__imut_uptr(ULang_Span_sint32_* __this__,int64_t Index);
ULang_Span_sint32_ ULang_Span_sint32____Index___ULang_Span_sint32___ULang_Range_t_uintptr_(ULang_Span_sint32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_sint32_ ULang_Span_sint32____Index___ULang_Span_sint32__imut_ULang_Range_t_uintptr_(ULang_Span_sint32_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_sint32_____new___ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
void ULang_Vector_sint32_____drop___ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
void* ULang_Vector_sint32__Data_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__);
void* ULang_Vector_sint32__Data_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
int64_t ULang_Vector_sint32__Size_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__);
int64_t ULang_Vector_sint32__Capacity_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__);
void ULang_Vector_sint32__Resize_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Size);
void ULang_Vector_sint32__Reserve_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Size);
void ULang_Vector_sint32__Clear_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
int32_t ULang_Vector_sint32__Pop_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
int32_t ULang_Vector_sint32__Remove_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Index);
void ULang_Vector_sint32__Push_ULang_Vector_sint32___i32_imut(ULang_Vector_sint32_* __this__,void* Val);
void ULang_Vector_sint32__Insert_ULang_Vector_sint32___uptr_i32_imut(ULang_Vector_sint32_* __this__,int64_t Index,void* Item);
void ULang_Vector_sint32__Append_ULang_Vector_sint32___ULang_Span_sint32_imut(ULang_Vector_sint32_* __this__,ULang_Span_sint32_ Val);
void* ULang_Vector_sint32____Index___ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Index);
void* ULang_Vector_sint32____Index___ULang_Vector_sint32__imut_uptr(ULang_Vector_sint32_* __this__,int64_t Index);
ULang_Span_sint32_ ULang_Vector_sint32____Index___ULang_Vector_sint32___ULang_Range_t_uintptr_(ULang_Vector_sint32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_sint32_ ULang_Vector_sint32____Index___ULang_Vector_sint32__imut_ULang_Range_t_uintptr_(ULang_Vector_sint32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_sint32_ ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__);
ULang_Span_sint32_ ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__);
ULang_Range_t_uintptr_ ULang_Range_uintptr__uptr_uptr(int64_t start,int64_t end);
void ULang_Span_uft8_____new___ULang_Span_uft8__(ULang_Span_uft8_* __this__);
void ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(ULang_Span_uft8_* __this__,void* data,int64_t size);
int64_t ULang_Span_uft8__Size_ULang_Span_uft8__imut(ULang_Span_uft8_* __this__);
void* ULang_Span_uft8__Data_ULang_Span_uft8__imut(ULang_Span_uft8_* __this__);
void* ULang_Span_uft8____Index___ULang_Span_uft8___uptr(ULang_Span_uft8_* __this__,int64_t Index);
void* ULang_Span_uft8____Index___ULang_Span_uft8__imut_uptr(ULang_Span_uft8_* __this__,int64_t Index);
ULang_Span_uft8_ ULang_Span_uft8____Index___ULang_Span_uft8___ULang_Range_t_uintptr_(ULang_Span_uft8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft8_ ULang_Span_uft8____Index___ULang_Span_uft8__imut_ULang_Range_t_uintptr_(ULang_Span_uft8_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_uft8_____new___ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
void ULang_Vector_uft8_____drop___ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
void* ULang_Vector_uft8__Data_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__);
void* ULang_Vector_uft8__Data_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
int64_t ULang_Vector_uft8__Size_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__);
int64_t ULang_Vector_uft8__Capacity_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__);
void ULang_Vector_uft8__Resize_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Size);
void ULang_Vector_uft8__Reserve_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Size);
void ULang_Vector_uft8__Clear_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
int8_t ULang_Vector_uft8__Pop_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
int8_t ULang_Vector_uft8__Remove_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Index);
void ULang_Vector_uft8__Push_ULang_Vector_uft8____imut(ULang_Vector_uft8_* __this__,void* Val);
void ULang_Vector_uft8__Insert_ULang_Vector_uft8___uptr__imut(ULang_Vector_uft8_* __this__,int64_t Index,void* Item);
void ULang_Vector_uft8__Append_ULang_Vector_uft8___ULang_Span_uft8_imut(ULang_Vector_uft8_* __this__,ULang_Span_uft8_ Val);
void* ULang_Vector_uft8____Index___ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Index);
void* ULang_Vector_uft8____Index___ULang_Vector_uft8__imut_uptr(ULang_Vector_uft8_* __this__,int64_t Index);
ULang_Span_uft8_ ULang_Vector_uft8____Index___ULang_Vector_uft8___ULang_Range_t_uintptr_(ULang_Vector_uft8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft8_ ULang_Vector_uft8____Index___ULang_Vector_uft8__imut_ULang_Range_t_uintptr_(ULang_Vector_uft8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft8_ ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__);
ULang_Span_uft8_ ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__);
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__);
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(ULang_StringSpan_t_uft8_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__);
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__);
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__);
int8_t ULang_StringSpan_t_uft8____equal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other);
int8_t ULang_StringSpan_t_uft8____notequal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other);
ULang_Span_uft8_ ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__);
ULang_Span_uft8_ ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__);
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__(ULang_Span_uft8_* span);
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__imut(ULang_Span_uft8_* span);
void* ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___uptr(ULang_StringSpan_t_uft8_* __this__,int64_t Index);
void* ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8__imut_uptr(ULang_StringSpan_t_uft8_* __this__,int64_t Index);
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft8_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft8_____new___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
void ULang_String_t_uft8_____new___ULang_String_t_uft8___ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* span);
void* ULang_String_t_uft8__Data_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__);
void* ULang_String_t_uft8__Data_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
int64_t ULang_String_t_uft8__Size_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__);
int64_t ULang_String_t_uft8__Capacity_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__);
void ULang_String_t_uft8__Resize_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Size);
void ULang_String_t_uft8__Reserve_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Size);
void ULang_String_t_uft8__Clear_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
int8_t ULang_String_t_uft8__Pop_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
int8_t ULang_String_t_uft8__Remove_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Index);
void ULang_String_t_uft8__Push_ULang_String_t_uft8____imut(ULang_String_t_uft8_* __this__,void* Val);
void ULang_String_t_uft8__Insert_ULang_String_t_uft8___uptr__imut(ULang_String_t_uft8_* __this__,int64_t Index,void* Item);
void* ULang_String_t_uft8____Index___ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Index);
void* ULang_String_t_uft8____Index___ULang_String_t_uft8__imut_uptr(ULang_String_t_uft8_* __this__,int64_t Index);
void ULang_String_t_uft8__Append_ULang_String_t_uft8___ULang_Span_uft8_imut(ULang_String_t_uft8_* __this__,ULang_Span_uft8_ Val);
int8_t ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other);
int8_t ULang_String_t_uft8____notequal___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other);
ULang_String_t_uft8_ ULang_String_t_uft8____Plus___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other);
ULang_String_t_uft8_ ULang_String_t_uft8____Compound_Plus___ULang_String_t_uft8___ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other);
int8_t ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other);
int8_t ULang_String_t_uft8____notequal___ULang_String_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other);
void ULang_String_t_uft8____Compound_Plus___ULang_String_t_uft8___ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other);
ULang_Span_uft8_ ULang_String_t_uft8__AsSpan_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
ULang_Span_uft8_ ULang_String_t_uft8__AsSpan_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__);
ULang_StringSpan_t_uft8_ ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
ULang_StringSpan_t_uft8_ ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__);
ULang_StringSpan_t_uft8_ ULang_String_t_uft8____Index___ULang_String_t_uft8___ULang_Range_t_uintptr_(ULang_String_t_uft8_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft8_ ULang_String_t_uft8____Index___ULang_String_t_uft8__imut_ULang_Range_t_uintptr_(ULang_String_t_uft8_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft8_____drop___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
void ULang_String_t_uft8_____de_init__(ULang_String_t_uft8_* __this__);
void ULang_Span_uft16_____new___ULang_Span_uft16__(ULang_Span_uft16_* __this__);
void ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(ULang_Span_uft16_* __this__,void* data,int64_t size);
int64_t ULang_Span_uft16__Size_ULang_Span_uft16__imut(ULang_Span_uft16_* __this__);
void* ULang_Span_uft16__Data_ULang_Span_uft16__imut(ULang_Span_uft16_* __this__);
void* ULang_Span_uft16____Index___ULang_Span_uft16___uptr(ULang_Span_uft16_* __this__,int64_t Index);
void* ULang_Span_uft16____Index___ULang_Span_uft16__imut_uptr(ULang_Span_uft16_* __this__,int64_t Index);
ULang_Span_uft16_ ULang_Span_uft16____Index___ULang_Span_uft16___ULang_Range_t_uintptr_(ULang_Span_uft16_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft16_ ULang_Span_uft16____Index___ULang_Span_uft16__imut_ULang_Range_t_uintptr_(ULang_Span_uft16_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_uft16_____new___ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
void ULang_Vector_uft16_____drop___ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
void* ULang_Vector_uft16__Data_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__);
void* ULang_Vector_uft16__Data_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
int64_t ULang_Vector_uft16__Size_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__);
int64_t ULang_Vector_uft16__Capacity_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__);
void ULang_Vector_uft16__Resize_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Size);
void ULang_Vector_uft16__Reserve_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Size);
void ULang_Vector_uft16__Clear_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
int16_t ULang_Vector_uft16__Pop_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
int16_t ULang_Vector_uft16__Remove_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Index);
void ULang_Vector_uft16__Push_ULang_Vector_uft16____imut(ULang_Vector_uft16_* __this__,void* Val);
void ULang_Vector_uft16__Insert_ULang_Vector_uft16___uptr__imut(ULang_Vector_uft16_* __this__,int64_t Index,void* Item);
void ULang_Vector_uft16__Append_ULang_Vector_uft16___ULang_Span_uft16_imut(ULang_Vector_uft16_* __this__,ULang_Span_uft16_ Val);
void* ULang_Vector_uft16____Index___ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Index);
void* ULang_Vector_uft16____Index___ULang_Vector_uft16__imut_uptr(ULang_Vector_uft16_* __this__,int64_t Index);
ULang_Span_uft16_ ULang_Vector_uft16____Index___ULang_Vector_uft16___ULang_Range_t_uintptr_(ULang_Vector_uft16_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft16_ ULang_Vector_uft16____Index___ULang_Vector_uft16__imut_ULang_Range_t_uintptr_(ULang_Vector_uft16_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft16_ ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__);
ULang_Span_uft16_ ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__);
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__);
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(ULang_StringSpan_t_uft16_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__);
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__);
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__);
int8_t ULang_StringSpan_t_uft16____equal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other);
int8_t ULang_StringSpan_t_uft16____notequal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other);
ULang_Span_uft16_ ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__);
ULang_Span_uft16_ ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__);
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__(ULang_Span_uft16_* span);
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__imut(ULang_Span_uft16_* span);
void* ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___uptr(ULang_StringSpan_t_uft16_* __this__,int64_t Index);
void* ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16__imut_uptr(ULang_StringSpan_t_uft16_* __this__,int64_t Index);
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft16_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft16_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft16_____new___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
void ULang_String_t_uft16_____new___ULang_String_t_uft16___ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* span);
void* ULang_String_t_uft16__Data_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__);
void* ULang_String_t_uft16__Data_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
int64_t ULang_String_t_uft16__Size_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__);
int64_t ULang_String_t_uft16__Capacity_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__);
void ULang_String_t_uft16__Resize_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Size);
void ULang_String_t_uft16__Reserve_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Size);
void ULang_String_t_uft16__Clear_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
int16_t ULang_String_t_uft16__Pop_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
int16_t ULang_String_t_uft16__Remove_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Index);
void ULang_String_t_uft16__Push_ULang_String_t_uft16____imut(ULang_String_t_uft16_* __this__,void* Val);
void ULang_String_t_uft16__Insert_ULang_String_t_uft16___uptr__imut(ULang_String_t_uft16_* __this__,int64_t Index,void* Item);
void* ULang_String_t_uft16____Index___ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Index);
void* ULang_String_t_uft16____Index___ULang_String_t_uft16__imut_uptr(ULang_String_t_uft16_* __this__,int64_t Index);
void ULang_String_t_uft16__Append_ULang_String_t_uft16___ULang_Span_uft16_imut(ULang_String_t_uft16_* __this__,ULang_Span_uft16_ Val);
int8_t ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other);
int8_t ULang_String_t_uft16____notequal___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other);
ULang_String_t_uft16_ ULang_String_t_uft16____Plus___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other);
ULang_String_t_uft16_ ULang_String_t_uft16____Compound_Plus___ULang_String_t_uft16___ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other);
int8_t ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other);
int8_t ULang_String_t_uft16____notequal___ULang_String_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other);
void ULang_String_t_uft16____Compound_Plus___ULang_String_t_uft16___ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other);
ULang_Span_uft16_ ULang_String_t_uft16__AsSpan_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
ULang_Span_uft16_ ULang_String_t_uft16__AsSpan_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__);
ULang_StringSpan_t_uft16_ ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
ULang_StringSpan_t_uft16_ ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__);
ULang_StringSpan_t_uft16_ ULang_String_t_uft16____Index___ULang_String_t_uft16___ULang_Range_t_uintptr_(ULang_String_t_uft16_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft16_ ULang_String_t_uft16____Index___ULang_String_t_uft16__imut_ULang_Range_t_uintptr_(ULang_String_t_uft16_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft16_____drop___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
void ULang_String_t_uft16_____de_init__(ULang_String_t_uft16_* __this__);
void ULang_Span_uft32_____new___ULang_Span_uft32__(ULang_Span_uft32_* __this__);
void ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(ULang_Span_uft32_* __this__,void* data,int64_t size);
int64_t ULang_Span_uft32__Size_ULang_Span_uft32__imut(ULang_Span_uft32_* __this__);
void* ULang_Span_uft32__Data_ULang_Span_uft32__imut(ULang_Span_uft32_* __this__);
void* ULang_Span_uft32____Index___ULang_Span_uft32___uptr(ULang_Span_uft32_* __this__,int64_t Index);
void* ULang_Span_uft32____Index___ULang_Span_uft32__imut_uptr(ULang_Span_uft32_* __this__,int64_t Index);
ULang_Span_uft32_ ULang_Span_uft32____Index___ULang_Span_uft32___ULang_Range_t_uintptr_(ULang_Span_uft32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft32_ ULang_Span_uft32____Index___ULang_Span_uft32__imut_ULang_Range_t_uintptr_(ULang_Span_uft32_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_Vector_uft32_____new___ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
void ULang_Vector_uft32_____drop___ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
void* ULang_Vector_uft32__Data_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__);
void* ULang_Vector_uft32__Data_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
int64_t ULang_Vector_uft32__Size_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__);
int64_t ULang_Vector_uft32__Capacity_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__);
void ULang_Vector_uft32__Resize_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Size);
void ULang_Vector_uft32__Reserve_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Size);
void ULang_Vector_uft32__Clear_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
int32_t ULang_Vector_uft32__Pop_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
int32_t ULang_Vector_uft32__Remove_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Index);
void ULang_Vector_uft32__Push_ULang_Vector_uft32____imut(ULang_Vector_uft32_* __this__,void* Val);
void ULang_Vector_uft32__Insert_ULang_Vector_uft32___uptr__imut(ULang_Vector_uft32_* __this__,int64_t Index,void* Item);
void ULang_Vector_uft32__Append_ULang_Vector_uft32___ULang_Span_uft32_imut(ULang_Vector_uft32_* __this__,ULang_Span_uft32_ Val);
void* ULang_Vector_uft32____Index___ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Index);
void* ULang_Vector_uft32____Index___ULang_Vector_uft32__imut_uptr(ULang_Vector_uft32_* __this__,int64_t Index);
ULang_Span_uft32_ ULang_Vector_uft32____Index___ULang_Vector_uft32___ULang_Range_t_uintptr_(ULang_Vector_uft32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft32_ ULang_Vector_uft32____Index___ULang_Vector_uft32__imut_ULang_Range_t_uintptr_(ULang_Vector_uft32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_Span_uft32_ ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__);
ULang_Span_uft32_ ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__);
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__);
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(ULang_StringSpan_t_uft32_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__);
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__);
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__);
int8_t ULang_StringSpan_t_uft32____equal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other);
int8_t ULang_StringSpan_t_uft32____notequal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other);
ULang_Span_uft32_ ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__);
ULang_Span_uft32_ ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__);
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__(ULang_Span_uft32_* span);
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__imut(ULang_Span_uft32_* span);
void* ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___uptr(ULang_StringSpan_t_uft32_* __this__,int64_t Index);
void* ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32__imut_uptr(ULang_StringSpan_t_uft32_* __this__,int64_t Index);
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft32_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft32_____new___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
void ULang_String_t_uft32_____new___ULang_String_t_uft32___ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* span);
void* ULang_String_t_uft32__Data_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__);
void* ULang_String_t_uft32__Data_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
int64_t ULang_String_t_uft32__Size_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__);
int64_t ULang_String_t_uft32__Capacity_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__);
void ULang_String_t_uft32__Resize_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Size);
void ULang_String_t_uft32__Reserve_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Size);
void ULang_String_t_uft32__Clear_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
int32_t ULang_String_t_uft32__Pop_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
int32_t ULang_String_t_uft32__Remove_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Index);
void ULang_String_t_uft32__Push_ULang_String_t_uft32____imut(ULang_String_t_uft32_* __this__,void* Val);
void ULang_String_t_uft32__Insert_ULang_String_t_uft32___uptr__imut(ULang_String_t_uft32_* __this__,int64_t Index,void* Item);
void* ULang_String_t_uft32____Index___ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Index);
void* ULang_String_t_uft32____Index___ULang_String_t_uft32__imut_uptr(ULang_String_t_uft32_* __this__,int64_t Index);
void ULang_String_t_uft32__Append_ULang_String_t_uft32___ULang_Span_uft32_imut(ULang_String_t_uft32_* __this__,ULang_Span_uft32_ Val);
int8_t ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other);
int8_t ULang_String_t_uft32____notequal___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other);
ULang_String_t_uft32_ ULang_String_t_uft32____Plus___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other);
ULang_String_t_uft32_ ULang_String_t_uft32____Compound_Plus___ULang_String_t_uft32___ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other);
int8_t ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other);
int8_t ULang_String_t_uft32____notequal___ULang_String_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other);
void ULang_String_t_uft32____Compound_Plus___ULang_String_t_uft32___ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other);
ULang_Span_uft32_ ULang_String_t_uft32__AsSpan_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
ULang_Span_uft32_ ULang_String_t_uft32__AsSpan_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__);
ULang_StringSpan_t_uft32_ ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
ULang_StringSpan_t_uft32_ ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__);
ULang_StringSpan_t_uft32_ ULang_String_t_uft32____Index___ULang_String_t_uft32___ULang_Range_t_uintptr_(ULang_String_t_uft32_* __this__,ULang_Range_t_uintptr_ Range);
ULang_StringSpan_t_uft32_ ULang_String_t_uft32____Index___ULang_String_t_uft32__imut_ULang_Range_t_uintptr_(ULang_String_t_uft32_* __this__,ULang_Range_t_uintptr_ Range);
void ULang_String_t_uft32_____drop___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
void ULang_String_t_uft32_____de_init__(ULang_String_t_uft32_* __this__);
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* __this__);
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(ULang_Vec2f_t_float32_* __this__,float32_t x,float32_t y);
int8_t ULang_Vec2f_t_float32____equal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
int8_t ULang_Vec2f_t_float32____notequal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
void ULang_Vec2f_t_float32____Compound_Plus___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
void ULang_Vec2f_t_float32____Compound_Sub___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
ULang_Vec2f_t_float32_ ULang_Vec2f_t_float32____Plus___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
ULang_Vec2f_t_float32_ ULang_Vec2f_t_float32____Minus___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other);
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32__(ULang_Vec2f_t_sint32_* __this__);
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32___i32_i32(ULang_Vec2f_t_sint32_* __this__,int32_t x,int32_t y);
int8_t ULang_Vec2f_t_sint32____equal___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
int8_t ULang_Vec2f_t_sint32____notequal___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
void ULang_Vec2f_t_sint32____Compound_Plus___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
void ULang_Vec2f_t_sint32____Compound_Sub___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
ULang_Vec2f_t_sint32_ ULang_Vec2f_t_sint32____Plus___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
ULang_Vec2f_t_sint32_ ULang_Vec2f_t_sint32____Minus___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other);
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__);
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32___f32_f32_f32(ULang_Vec3f_t_float32_* __this__,float32_t x,float32_t y,float32_t z);
int8_t ULang_Vec3f_t_float32____equal___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
int8_t ULang_Vec3f_t_float32____notequal___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
void ULang_Vec3f_t_float32____Compound_Plus___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
void ULang_Vec3f_t_float32____Compound_Sub___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
ULang_Vec3f_t_float32_ ULang_Vec3f_t_float32____Plus___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
ULang_Vec3f_t_float32_ ULang_Vec3f_t_float32____Minus___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other);
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__);
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32___i32_i32_i32(ULang_Vec3f_t_sint32_* __this__,int32_t x,int32_t y,int32_t z);
int8_t ULang_Vec3f_t_sint32____equal___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
int8_t ULang_Vec3f_t_sint32____notequal___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
void ULang_Vec3f_t_sint32____Compound_Plus___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
void ULang_Vec3f_t_sint32____Compound_Sub___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
ULang_Vec3f_t_sint32_ ULang_Vec3f_t_sint32____Plus___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
ULang_Vec3f_t_sint32_ ULang_Vec3f_t_sint32____Minus___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other);
void ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty___ULang_Empty_(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* base);
ULang_Empty* ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__);
ULang_Empty* ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__);
void ULang_Ptr_ULang_Empty__Write_ULang_Ptr_ULang_Empty___ULang_Empty_imut(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* newvalue);
void ULang_Ptr_ULang_Empty__Write_ULang_Ptr_ULang_Empty___ULang_Emptymov(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty newvalue);
ULang_Empty* ULang_Ptr_ULang_Empty__ReadPtr_ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__);
ULang_Empty* ULang_Ptr_ULang_Empty____IndirectMember___ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__);
ULang_Empty* ULang_Ptr_ULang_Empty____IndirectMember___ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__);
void ULang_Ptr_ULang_Empty__ReassignTo_ULang_Ptr_ULang_Empty___ULang_Empty_(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* newbase);
ULang_Ptr_ULang_Empty_ ULang_Ptr_ULang_Empty__Make_ULang_Empty_imut(ULang_Empty* V);
ULang_Ptr_ULang_Empty_ ULang_Ptr_ULang_Empty__Make_ULang_Empty_(ULang_Empty* V);
void ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__);
void ULang_Ptr_sint32_____new___ULang_Ptr_sint32___i32_(ULang_Ptr_sint32_* __this__,void* base);
void* ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__);
void* ULang_Ptr_sint32__Get_ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__);
void ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32_imut(ULang_Ptr_sint32_* __this__,void* newvalue);
void ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32mov(ULang_Ptr_sint32_* __this__,int32_t newvalue);
void* ULang_Ptr_sint32__ReadPtr_ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__);
void* ULang_Ptr_sint32____IndirectMember___ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__);
void* ULang_Ptr_sint32____IndirectMember___ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__);
void ULang_Ptr_sint32__ReassignTo_ULang_Ptr_sint32___i32_(ULang_Ptr_sint32_* __this__,void* newbase);
ULang_Ptr_sint32_ ULang_Ptr_sint32__Make_i32_imut(void* V);
ULang_Ptr_sint32_ ULang_Ptr_sint32__Make_i32_(void* V);
void ULang_Ptr_sint32_____new___ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__);
ULang_Ptr_sint32_ ULang_ptr_sint32__i32_(void* V);
void ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* base);
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
void ULang_Ptr_ULang_Vec2f_t_float32___Write_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* newvalue);
void ULang_Ptr_ULang_Vec2f_t_float32___Write_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32_mov(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_ newvalue);
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___ReadPtr_ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
void ULang_Ptr_ULang_Vec2f_t_float32___ReassignTo_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* newbase);
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_Ptr_ULang_Vec2f_t_float32___Make_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* V);
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_Ptr_ULang_Vec2f_t_float32___Make_ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* V);
void ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__);
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_ptr_ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* V);
void putchar(int8_t Val);
void ULang_Native_OS_PrintChar_c(int8_t Value);
void ULang_Native_OS_PrintSpan_c___imut_uptr(void* Buffer,int64_t Size);
void ULang_Native_OS_PrintClear();
void ULang_Native_OS_Malloc_uptr(int64_t Size);
void ULang_Native_OS_Free_u8___(void* Block);


/*file.cpp*/

void __StaticInit__()
{
}
void __StaticUnload__()
{
}
void __threadInit__()
{
}
void __threadUnload__()
{
}
int8_t ULang_Compiler_Is64Bit()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:3*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_Compiler_Is32Bit()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:4*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendC89()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:6*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendUCodeVM()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:7*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendWasm()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:8*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendVM()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:10*/

 int8_t tep0 = 1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Compiler____new___ULang_Compiler_(ULang_Compiler* __this__)
{ /*Block*/ 
 return;

}
void ULang_Empty____new___ULang_Empty_(ULang_Empty* __this__)
{ /*Block*/ 
 return;

}
void ULang_LowLevel_Malloc_uptr(int64_t Size)
{ /*Block*/ 

 /*File:/Helper/LowLevel.uc*/
 /*Line:3*/

 int64_t tep0 = Size;
 ULang_Native_OS_Malloc_uptr(tep0);
 return;

}
void ULang_LowLevel_Free_u8___(void* Block)
{ /*Block*/ 

 /*File:/Helper/LowLevel.uc*/
 /*Line:4*/

 void* tep0 = Block;
 ULang_Native_OS_Free_u8___(tep0);
 return;

}
void ULang_LowLevel_Memcopy_u8____u8____uptr(void* destination,void* source,int64_t num)
{ /*Block*/ 


 return;

}
void ULang_LowLevel_Memmove_u8____u8____uptr(void* destination,void* source,int64_t num)
{ /*Block*/ 


 return;

}
void ULang_LowLevel____new___ULang_LowLevel_(ULang_LowLevel* __this__)
{ /*Block*/ 
 return;

}
void ULang_Math____new___ULang_Math_(ULang_Math* __this__)
{ /*Block*/ 
 return;

}
void ULang_Fmt_Print_c(int8_t Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:5*/

 int8_t tep0 = Str;
 ULang_Native_OS_PrintChar_c(tep0);
 return;

}
void ULang_Fmt_Print_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:9*/

 ULang_StringSpan_t_char_* tep0 = &Str;
 void* tep1 = ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(tep0);
 ULang_StringSpan_t_char_* tep2 = &Str;
 int64_t tep3 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep2);
 ULang_Native_OS_PrintSpan_c___imut_uptr(tep1,tep3);
 return;

}
void ULang_Fmt_Print_ULang_String_t_char__imut(ULang_String_t_char_* Str)
{ /*Block*/ 


 return;

}
void ULang_Fmt_Println_c(int8_t Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:14*/

 int8_t tep0 = Str;
 ULang_Fmt_Print_c(tep0);

 /*Line:15*/

 int8_t tep1 = 10;
 ULang_Fmt_Print_c(tep1);
 return;

}
void ULang_Fmt_Println_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:18*/

 ULang_StringSpan_t_char_ tep0 = Str;
 ULang_Fmt_Print_ULang_StringSpan_t_char_imut(tep0);

 /*Line:19*/

 int8_t tep1 = 10;
 ULang_Fmt_Print_c(tep1);
 return;

}
void ULang_Fmt_Println_ULang_String_t_char__imut(ULang_String_t_char_* Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:22*/

 ULang_String_t_char_* tep0 = Str;
 ULang_Fmt_Print_ULang_String_t_char__imut(tep0);

 /*Line:23*/

 int8_t tep1 = 10;
 ULang_Fmt_Print_c(tep1);
 return;

}
void ULang_Fmt____new___ULang_Fmt_(ULang_Fmt* __this__)
{ /*Block*/ 
 return;

}
void ULang_OpenedFile____new___ULang_OpenedFile_(ULang_OpenedFile* __this__)
{ /*Block*/ 
 return;

}
void ULang_OpenFileError____new___ULang_OpenFileError_(ULang_OpenFileError* __this__)
{ /*Block*/ 
 return;

}
ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ ULang_File_Open_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path)
{
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ ULang_File_Open_ULang_String_t_char__imut(ULang_String_t_char_* path)
{
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_Result_ULang_String_t_char__ULang_OpenFileError_ ULang_File_GetString_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path)
{
 ULang_Result_ULang_String_t_char__ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_Result_ULang_String_t_char__ULang_OpenFileError_ ULang_File_GetString_ULang_String_t_char__imut(ULang_String_t_char_* path)
{
 ULang_Result_ULang_String_t_char__ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ ULang_File_GetBytes_ULang_StringSpan_t_char_(ULang_StringSpan_t_char_ path)
{
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ ULang_File_GetBytes_ULang_String_t_char__imut(ULang_String_t_char_* path)
{
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_File____new___ULang_File_(ULang_File* __this__)
{ /*Block*/ 
 return;

}
void ULang_Writer____new___ULang_Writer_(ULang_Writer* __this__)
{ /*Block*/ 
 return;

}
int8_t ULang_Tests_Print1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/IOTest.uc*/
 /*Line:7*/

 void* tep0 = &_Const_SpanString_Hello_World;
 int64_t tep1 = 11;
 ULang_StringSpan_t_char_ tep2;
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep3,tep0,tep1);
 ULang_Fmt_Println_ULang_StringSpan_t_char_imut(tep2);

 /*Line:8*/

 int8_t tep4 = 1;
 _ReturnVal = tep4;
 return _ReturnVal;

}
int8_t ULang_Tests_Print2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/IOTest.uc*/
 /*Line:12*/

 int8_t tep0 = 72;
 ULang_Fmt_Print_c(tep0);

 /*Line:13*/

 int8_t tep1 = 105;
 ULang_Fmt_Println_c(tep1);

 /*Line:14*/

 int8_t tep2 = 1;
 _ReturnVal = tep2;
 return _ReturnVal;

}
int8_t ULang_Tests_Max_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:7*/

 int32_t tep0 = 5;
 int32_t tep1 = 2;
 int32_t tep2 = ULang_Math_Max_sint32__i32_i32(tep0,tep1);
 int32_t tep3 = 5;
 int8_t tep4 = (int8_t)(tep2==tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
int8_t ULang_Tests_Max_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:11*/

 int32_t tep0 = 2;
 int32_t tep1 = 5;
 int32_t tep2 = ULang_Math_Max_sint32__i32_i32(tep0,tep1);
 int32_t tep3 = 5;
 int8_t tep4 = (int8_t)(tep2==tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
int8_t ULang_Tests_Min_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:15*/

 int32_t tep0 = 5;
 int32_t tep1 = 2;
 int32_t tep2 = ULang_Math_Min_sint32__i32_i32(tep0,tep1);
 int32_t tep3 = 2;
 int8_t tep4 = (int8_t)(tep2==tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
int8_t ULang_Tests_Min_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:19*/

 int32_t tep0 = 2;
 int32_t tep1 = 5;
 int32_t tep2 = ULang_Math_Min_sint32__i32_i32(tep0,tep1);
 int32_t tep3 = 2;
 int8_t tep4 = (int8_t)(tep2==tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
int8_t ULang_Tests_StringSpan_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/StringTest.uc*/
 /*Line:6*/

 void* tep0 = &_Const_SpanString_Hello;
 int64_t tep1 = 5;
 ULang_StringSpan_t_char_ tep2;
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep3,tep0,tep1);
 ULang_StringSpan_t_char_* tep4 = &tep2;
 int64_t tep5 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep4);
 int64_t tep6 = 5;
 int8_t tep7 = (int8_t)(tep5==tep6);
 _ReturnVal = tep7;
 return _ReturnVal;

}
int8_t ULang_Tests_StringSpan_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/StringTest.uc*/
 /*Line:10*/

 void* tep0 = &_Const_SpanString_Hello;
 int64_t tep1 = 5;
 ULang_StringSpan_t_char_ tep2;
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep3,tep0,tep1);
 ULang_StringSpan_t_char_* tep4 = &tep2;
 void* tep5 = &_Const_SpanString_World;
 int64_t tep6 = 5;
 ULang_StringSpan_t_char_ tep7;
 ULang_StringSpan_t_char_* tep8 = &tep7;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep8,tep5,tep6);
 ULang_StringSpan_t_char_* tep9 = &tep7;
 int8_t tep10 = ULang_StringSpan_t_char____notequal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(tep4,tep9);
 _ReturnVal = tep10;
 return _ReturnVal;

}
int8_t ULang_Tests_StringSpan_3()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/StringTest.uc*/
 /*Line:14*/

 void* tep0 = &_Const_SpanString_Hello;
 int64_t tep1 = 5;
 ULang_StringSpan_t_char_ tep2;
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep3,tep0,tep1);
 ULang_StringSpan_t_char_* tep4 = &tep2;
 void* tep5 = &_Const_SpanString_Hello;
 int64_t tep6 = 5;
 ULang_StringSpan_t_char_ tep7;
 ULang_StringSpan_t_char_* tep8 = &tep7;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep8,tep5,tep6);
 ULang_StringSpan_t_char_* tep9 = &tep7;
 int8_t tep10 = ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(tep4,tep9);
 _ReturnVal = tep10;
 return _ReturnVal;

}
int8_t ULang_Tests_StringSpan_4()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/StringTest.uc*/
 /*Line:18*/

 ULang_String_t_char_ tep0;
 void* tep1 = &_Const_SpanString_Hello;
 int64_t tep2 = 5;
 ULang_StringSpan_t_char_ tep3;
 ULang_StringSpan_t_char_* tep4 = &tep3;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep4,tep1,tep2);
 ULang_String_t_char_ tep5;
 ULang_String_t_char_* tep6 = &tep5;
 ULang_StringSpan_t_char_* tep7 = &tep3;
 ULang_String_t_char_____new___ULang_String_t_char___ULang_StringSpan_t_char__imut(tep6,tep7);
 tep0 = tep5;

 /*Line:19*/

 ULang_String_t_char_* tep8 = &tep0;
 void* tep9 = &_Const_SpanString_Hello;
 int64_t tep10 = 5;
 ULang_StringSpan_t_char_ tep11;
 ULang_StringSpan_t_char_* tep12 = &tep11;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep12,tep9,tep10);
 ULang_String_t_char_* tep13;
 ULang_String_t_char_* tep14 = &tep13;
 ULang_StringSpan_t_char_* tep15 = &tep11;
 ULang_String_t_char_____new___ULang_String_t_char___ULang_StringSpan_t_char__imut(tep14,tep15);
 int8_t tep16 = ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_String_t_char__imut(tep8,tep13);
 _ReturnVal = tep16;

 /*Line:0*/

 ULang_String_t_char_* tep17 = &tep0;
 ULang_String_t_char_____drop___ULang_String_t_char__(tep17);
 return _ReturnVal;

}
int8_t ULang_Tests_Vector_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/VectorTest.uc*/
 /*Line:5*/

 ULang_Vector_sint32_ tep0;

 /*Line:0*/

 ULang_Vector_sint32_* tep1 = &tep0;
 ULang_Vector_sint32_____new___ULang_Vector_sint32__(tep1);

 /*Line:5*/
 /*Line:6*/

 ULang_Vector_sint32_* tep2 = &tep0;
 int32_t tep3 = 5;
 void* tep4 = &tep3;
 ULang_Vector_sint32__Push_ULang_Vector_sint32___i32_imut(tep2,tep4);

 /*Line:7*/

 ULang_Vector_sint32_* tep5 = &tep0;
 int64_t tep6 = 0;
 void* tep7 = ULang_Vector_sint32____Index___ULang_Vector_sint32___uptr(tep5,tep6);
 int32_t tep8 = *(int32_t*)tep7;
 int32_t tep9 = 5;
 int8_t tep10 = (int8_t)(tep8==tep9);
 ULang_Vector_sint32_* tep11 = &tep0;
 int64_t tep12 = ULang_Vector_sint32__Size_ULang_Vector_sint32__imut(tep11);
 int64_t tep13 = 1;
 int8_t tep14 = (int8_t)(tep12==tep13);
 int8_t tep15 = tep10&&tep14;
 _ReturnVal = tep15;

 /*Line:0*/

 ULang_Vector_sint32_* tep16 = &tep0;
 ULang_Vector_sint32_____drop___ULang_Vector_sint32__(tep16);
 return _ReturnVal;

}
int8_t ULang_Tests_Vector_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/VectorTest.uc*/
 /*Line:11*/

 ULang_Vector_sint32_ tep0;

 /*Line:0*/

 ULang_Vector_sint32_* tep1 = &tep0;
 ULang_Vector_sint32_____new___ULang_Vector_sint32__(tep1);

 /*Line:11*/
 /*Line:12*/

 ULang_Vector_sint32_* tep2 = &tep0;
 int32_t tep3 = 5;
 void* tep4 = &tep3;
 ULang_Vector_sint32__Push_ULang_Vector_sint32___i32_imut(tep2,tep4);

 /*Line:13*/

 ULang_Vector_sint32_* tep5 = &tep0;
 int32_t tep6 = 15;
 void* tep7 = &tep6;
 ULang_Vector_sint32__Push_ULang_Vector_sint32___i32_imut(tep5,tep7);

 /*Line:15*/

 ULang_Span_sint32_ tep8;

 /*Line:13*/
 /*Line:15*/

 ULang_Vector_sint32_* tep9 = &tep0;
 int64_t tep10 = 1;
 int64_t tep11 = 2;
 ULang_Range_t_uintptr_ tep12 = ULang_Range_uintptr__uptr_uptr(tep10,tep11);
 ULang_Span_sint32_ tep13 = ULang_Vector_sint32____Index___ULang_Vector_sint32___ULang_Range_t_uintptr_(tep9,tep12);
 tep8 = tep13;

 /*Line:17*/

 ULang_Span_sint32_* tep14 = &tep8;
 int64_t tep15 = 0;
 void* tep16 = ULang_Span_sint32____Index___ULang_Span_sint32___uptr(tep14,tep15);
 int32_t tep17 = *(int32_t*)tep16;
 int32_t tep18 = 15;
 int8_t tep19 = (int8_t)(tep17==tep18);
 ULang_Span_sint32_* tep20 = &tep8;
 int64_t tep21 = ULang_Span_sint32__Size_ULang_Span_sint32__imut(tep20);
 int64_t tep22 = 1;
 int8_t tep23 = (int8_t)(tep21==tep22);
 int8_t tep24 = tep19&&tep23;
 _ReturnVal = tep24;

 /*Line:0*/

 ULang_Vector_sint32_* tep25 = &tep0;
 ULang_Vector_sint32_____drop___ULang_Vector_sint32__(tep25);
 return _ReturnVal;

}
void ULang_Future____new___ULang_Future_(ULang_Future* __this__)
{ /*Block*/ 
 return;

}
void ULang_Thread____new___ULang_Thread_(ULang_Thread* __this__)
{ /*Block*/ 
 return;

}
void ULang_Path____new___ULang_Path_(ULang_Path* __this__)
{ /*Block*/ 
 return;

}
int8_t ULang_Tests_Ptr_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/PtrTest.uc*/
 /*Line:6*/

 int32_t tep0;
 int32_t tep1 = 5;
 tep0 = tep1;

 /*Line:7*/

 ULang_Ptr_sint32_ tep2;
 void* tep3 = &tep0;
 ULang_Ptr_sint32_ tep4 = ULang_ptr_sint32__i32_(tep3);
 tep2 = tep4;

 /*Line:9*/

 int32_t tep5 = 10;
 ULang_Ptr_sint32_* tep6 = &tep2;
 void* tep7 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(tep6);
 *(int32_t*)tep7 = tep5;

 /*Line:12*/

 int32_t tep8 = tep0;
 int32_t tep9 = 10;
 int8_t tep10 = (int8_t)(tep8==tep9);
 _ReturnVal = tep10;
 return _ReturnVal;

}
int8_t ULang_Tests_Ptr_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/PtrTest.uc*/
 /*Line:16*/

 int32_t tep0;
 int32_t tep1 = 5;
 tep0 = tep1;

 /*Line:17*/

 ULang_Ptr_sint32_ tep2;
 void* tep3 = &tep0;
 ULang_Ptr_sint32_ tep4 = ULang_ptr_sint32__i32_(tep3);
 tep2 = tep4;

 /*Line:19*/

 ULang_Ptr_sint32_* tep5 = &tep2;
 int32_t tep6 = 10;
 void* tep7 = &tep6;
 ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32_imut(tep5,tep7);

 /*Line:22*/

 int32_t tep8 = tep0;
 int32_t tep9 = 10;
 int8_t tep10 = (int8_t)(tep8==tep9);
 _ReturnVal = tep10;
 return _ReturnVal;

}
int8_t ULang_Tests_Ptr_3()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/PtrTest.uc*/
 /*Line:27*/

 int32_t tep0;
 int32_t tep1 = 5;
 tep0 = tep1;

 /*Line:28*/

 int32_t tep2;
 int32_t tep3 = 8;
 tep2 = tep3;

 /*Line:29*/

 ULang_Ptr_sint32_ tep4;
 void* tep5 = &tep0;
 ULang_Ptr_sint32_ tep6 = ULang_ptr_sint32__i32_(tep5);
 tep4 = tep6;

 /*Line:31*/

 ULang_Ptr_sint32_* tep7 = &tep4;
 void* tep8 = &tep2;
 ULang_Ptr_sint32__ReassignTo_ULang_Ptr_sint32___i32_(tep7,tep8);

 /*Line:33*/

 ULang_Ptr_sint32_* tep9 = &tep4;
 int32_t tep10 = 7;
 void* tep11 = &tep10;
 ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32_imut(tep9,tep11);

 /*Line:35*/

 int32_t tep12 = tep2;
 int32_t tep13 = 7;
 int8_t tep14 = (int8_t)(tep12==tep13);
 _ReturnVal = tep14;
 return _ReturnVal;

}
int8_t ULang_Tests_Ptr_4()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/PtrTest.uc*/
 /*Line:39*/

 int32_t tep0;
 int32_t tep1 = 5;
 tep0 = tep1;

 /*Line:40*/

 ULang_Ptr_sint32_ tep2;
 void* tep3 = &tep0;
 ULang_Ptr_sint32_ tep4 = ULang_ptr_sint32__i32_(tep3);
 tep2 = tep4;

 /*Line:42*/

 int32_t tep5 = tep0;
 int32_t tep6 = 5;
 int32_t tep7 = tep5+tep6;
 tep0 = tep7;

 /*Line:44*/

 ULang_Ptr_sint32_* tep8 = &tep2;
 void* tep9 = ULang_Ptr_sint32__ReadPtr_ULang_Ptr_sint32__imut(tep8);
 int32_t tep10 = *(int32_t*)tep9;
 int32_t tep11 = 10;
 int8_t tep12 = (int8_t)(tep10==tep11);
 _ReturnVal = tep12;
 return _ReturnVal;

}
int8_t ULang_Tests_Ptr_5()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/PtrTest.uc*/
 /*Line:50*/

 ULang_Vec2f_t_float32_ tep0;

 /*Line:0*/

 ULang_Vec2f_t_float32_* tep1 = &tep0;

 /*Line:50*/

 float32_t tep2 = 8.000000;
 float32_t tep3 = 6.000000;
 ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(tep1,tep2,tep3);

 /*Line:51*/

 ULang_Ptr_ULang_Vec2f_t_float32__ tep4;
 ULang_Vec2f_t_float32_* tep5 = &tep0;
 ULang_Ptr_ULang_Vec2f_t_float32__ tep6 = ULang_ptr_ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__(tep5);
 tep4 = tep6;

 /*Line:53*/

 float32_t tep7 = 5.000000;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep8 = &tep4;

 /*Line:0*/

 ULang_Vec2f_t_float32_* tep9 = ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___(tep8);
  tep9->__0 = tep7;

 /*Line:54*/

 float32_t tep10 = 8.000000;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep11 = &tep4;

 /*Line:0*/

 ULang_Vec2f_t_float32_* tep12 = ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___(tep11);
  tep12->__1 = tep10;

 /*Line:57*/

 ULang_Vec2f_t_float32_* tep13 = &tep0;
 ULang_Vec2f_t_float32_ tep14;
 ULang_Vec2f_t_float32_* tep15 = &tep14;
 float32_t tep16 = 5.000000;
 float32_t tep17 = 8.000000;
 ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(tep15,tep16,tep17);
 ULang_Vec2f_t_float32_* tep18 = &tep14;
 int8_t tep19 = ULang_Vec2f_t_float32____equal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(tep13,tep18);
 _ReturnVal = tep19;
 return _ReturnVal;

}
void ULang_Range_t_uintptr_____new___ULang_Range_t_uintptr__(ULang_Range_t_uintptr_* __this__)
{ /*Block*/ 

 /*File:/Types/Range.uc*/
 /*Line:7*/
 /*Line:0*/

 int64_t tep0 = 0;
  __this__->__0 = tep0;

 /*Line:8*/
 /*Line:0*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Range_t_uintptr_____new___ULang_Range_t_uintptr___uptr_uptr(ULang_Range_t_uintptr_* __this__,int64_t start,int64_t end)
{ /*Block*/ 

 /*File:/Types/Range.uc*/
 /*Line:10*/

 int64_t tep0 = start;
  __this__->__0 = tep0;

 /*Line:11*/

 int64_t tep1 = end;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(ULang_Range_t_uintptr_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Range.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(ULang_Range_t_uintptr_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Range.uc*/
 /*Line:14*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Span_char_____new___ULang_Span_char__(ULang_Span_char_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_char_____new___ULang_Span_char___c____uptr(ULang_Span_char_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_char__Size_ULang_Span_char__imut(ULang_Span_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_char__Data_ULang_Span_char__imut(ULang_Span_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_char____Index___ULang_Span_char___uptr(ULang_Span_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_char____Index___ULang_Span_char__imut_uptr(ULang_Span_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Span_char____Index___ULang_Span_char___ULang_Range_t_uintptr_(ULang_Span_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Span_char____Index___ULang_Span_char__imut_ULang_Range_t_uintptr_(ULang_Span_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:13*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(ULang_StringSpan_t_char_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:16*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:17*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:19*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:20*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:21*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__,ULang_StringSpan_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:24*/

 ULang_StringSpan_t_char_* tep0 = __this__;
 ULang_StringSpan_t_char_ tep1 = *(ULang_StringSpan_t_char_*)tep0;
 ULang_StringSpan_t_char_* tep2 = &tep1;
 int64_t tep3 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep2);
 ULang_StringSpan_t_char_* tep4 = Other;
 int64_t tep5 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep4);
 int8_t tep6 = (int8_t)(tep3!=tep5);
 int8_t tep7 = !tep6;
 if (tep7){goto _label0; };
 int8_t tep8 = 0;
 _ReturnVal = tep8;
 goto _label1;

 /*Line:26*/
 _label0:

 int64_t tep9;


 int32_t tep10 = 0;
 tep9 = (int64_t)tep10;

 _label4:

 int64_t tep11 = tep9;
 ULang_StringSpan_t_char_* tep12 = __this__;
 ULang_StringSpan_t_char_ tep13 = *(ULang_StringSpan_t_char_*)tep12;
 ULang_StringSpan_t_char_* tep14 = &tep13;
 int64_t tep15 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep14);
 int8_t tep16 = (int8_t)(tep11<tep15);
 int8_t tep17 = !tep16;
 if (tep17){goto _label2; };
 ULang_StringSpan_t_char_* tep18 = __this__;

 /*Line:28*/

 ULang_StringSpan_t_char_* tep19 = tep18;
 int64_t tep20 = tep9;
 void* tep21 = ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___uptr(tep19,tep20);
 int8_t tep22 = *(int8_t*)tep21;
 ULang_StringSpan_t_char_* tep23 = Other;
 int64_t tep24 = tep9;
 void* tep25 = ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___uptr(tep23,tep24);
 int8_t tep26 = *(int8_t*)tep25;
 int8_t tep27 = tep22!=tep26;
 int8_t tep28 = !tep27;
 if (tep28){goto _label3; };
 int8_t tep29 = 0;
 _ReturnVal = tep29;
 goto _label1;

 /*Line:26*/
 _label3:

 int64_t tep30 = tep9;
 int64_t tep31 = 1;
 int64_t tep32 = tep30+tep31;
 tep9 = tep32;
 goto _label4;

 /*Line:30*/
 _label2:

 int8_t tep33 = 1;
 _ReturnVal = tep33;

 _label1:

 return _ReturnVal;

}
int8_t ULang_StringSpan_t_char____notequal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__,ULang_StringSpan_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:32*/

 ULang_StringSpan_t_char_* tep0 = __this__;
 ULang_StringSpan_t_char_* tep1 = tep0;
 ULang_StringSpan_t_char_* tep2 = Other;
 int8_t tep3 = ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_char_ ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:34*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_char_ ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:35*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_StringSpan_t_char__ToStr_ULang_Span_char__(ULang_Span_char_* span)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_char_ tep0;
 ULang_StringSpan_t_char_* tep1 = &tep0;

 /*Line:38*/

 ULang_Span_char_* tep2 = span;
 void* tep3 = ULang_Span_char__Data_ULang_Span_char__imut(tep2);
 ULang_Span_char_* tep4 = span;
 int64_t tep5 = ULang_Span_char__Size_ULang_Span_char__imut(tep4);
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_StringSpan_t_char__ToStr_ULang_Span_char__imut(ULang_Span_char_* span)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_char_ tep0;
 ULang_StringSpan_t_char_* tep1 = &tep0;

 /*Line:39*/

 ULang_Span_char_* tep2 = span;
 void* tep3 = ULang_Span_char__Data_ULang_Span_char__imut(tep2);
 ULang_Span_char_* tep4 = span;
 int64_t tep5 = ULang_Span_char__Size_ULang_Span_char__imut(tep4);
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___uptr(ULang_StringSpan_t_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:41*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char__imut_uptr(ULang_StringSpan_t_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:42*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___ULang_Range_t_uintptr_(ULang_StringSpan_t_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:44*/
 /*Line:42*/
 /*Line:44*/

 ULang_StringSpan_t_char_* tep0 = __this__;
 ULang_Span_char_ tep1 = ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__(tep0);
 ULang_Span_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_char_ tep4 = ULang_Span_char____Index___ULang_Span_char___ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_char_* tep5 = &tep4;
 ULang_StringSpan_t_char_ tep6 = ULang_StringSpan_t_char__ToStr_ULang_Span_char__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:45*/
 /*Line:44*/
 /*Line:45*/

 ULang_StringSpan_t_char_* tep0 = __this__;
 ULang_Span_char_ tep1 = ULang_StringSpan_t_char__AsSpan_ULang_StringSpan_t_char__imut(tep0);
 ULang_Span_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_char_ tep4 = ULang_Span_char____Index___ULang_Span_char__imut_ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_char_* tep5 = &tep4;
 ULang_StringSpan_t_char_ tep6 = ULang_StringSpan_t_char__ToStr_ULang_Span_char__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
void ULang_Vector_char_____new___ULang_Vector_char__(ULang_Vector_char_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_char_____drop___ULang_Vector_char__(ULang_Vector_char_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_char__Data_ULang_Vector_char__imut(ULang_Vector_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_char__Data_ULang_Vector_char__(ULang_Vector_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_char__Size_ULang_Vector_char__imut(ULang_Vector_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_char__Capacity_ULang_Vector_char__imut(ULang_Vector_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_char__Resize_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_char_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_char__Reserve_ULang_Vector_char___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_char__Reserve_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 1;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int8_t tep17 = 0;
 *(int8_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 1;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int8_t tep31 = *(int8_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 1;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int8_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_char__Clear_ULang_Vector_char__(ULang_Vector_char_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int8_t ULang_Vector_char__Pop_ULang_Vector_char__(ULang_Vector_char_* __this__)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_Vector_char__Remove_ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Index)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_char__Push_ULang_Vector_char___c_imut(ULang_Vector_char_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_char_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_char__Resize_ULang_Vector_char___uptr(tep0,tep3);

 /*Line:48*/

 int8_t tep4 = *(int8_t*)Val;
 ULang_Vector_char_* tep5 = __this__;
 ULang_Vector_char_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_char____Index___ULang_Vector_char___uptr(tep6,tep9);
 int8_t tep11 = *(int8_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_char__Insert_ULang_Vector_char___uptr_c_imut(ULang_Vector_char_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_char__Append_ULang_Vector_char___ULang_Span_char_imut(ULang_Vector_char_* __this__,ULang_Span_char_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_char____Index___ULang_Vector_char___uptr(ULang_Vector_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_char____Index___ULang_Vector_char__imut_uptr(ULang_Vector_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Vector_char____Index___ULang_Vector_char___ULang_Range_t_uintptr_(ULang_Vector_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_char_* tep0 = __this__;
 ULang_Span_char_ tep1 = ULang_Vector_char__AsSpan_ULang_Vector_char__(tep0);
 ULang_Span_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_char_ tep4 = ULang_Span_char____Index___ULang_Span_char___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Vector_char____Index___ULang_Vector_char__imut_ULang_Range_t_uintptr_(ULang_Vector_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_char_* tep0 = __this__;
 ULang_Span_char_ tep1 = ULang_Vector_char__AsSpan_ULang_Vector_char__imut(tep0);
 ULang_Span_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_char_ tep4 = ULang_Span_char____Index___ULang_Span_char__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Vector_char__AsSpan_ULang_Vector_char__(ULang_Vector_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_char_ ULang_Vector_char__AsSpan_ULang_Vector_char__imut(ULang_Vector_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_char_____new___ULang_Span_char___c____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_String_t_char_____new___ULang_String_t_char__(ULang_String_t_char_* __this__)
{ /*Block*/ 


 return;

}
void ULang_String_t_char_____new___ULang_String_t_char___ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* span)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:58*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_StringSpan_t_char_* tep1 = span;
 int64_t tep2 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep1);
 ULang_String_t_char__Resize_ULang_String_t_char___uptr(tep0,tep2);

 /*Line:59*/

 int64_t tep3;


 int32_t tep4 = 0;
 tep3 = (int64_t)tep4;

 _label1:

 int64_t tep5 = tep3;
 ULang_StringSpan_t_char_* tep6 = span;
 int64_t tep7 = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(tep6);
 int8_t tep8 = (int8_t)(tep5<tep7);
 int8_t tep9 = !tep8;
 if (tep9){goto _label0; };

 /*Line:60*/
 /*Line:59*/
 /*Line:60*/

 ULang_StringSpan_t_char_* tep10 = span;
 int64_t tep11 = tep3;
 void* tep12 = ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___uptr(tep10,tep11);
 int8_t tep13 = *(int8_t*)tep12;
 ULang_String_t_char_* tep14 = __this__;
 ULang_String_t_char_* tep15 = tep14;
 int64_t tep16 = tep3;
 void* tep17 = ULang_String_t_char____Index___ULang_String_t_char___uptr(tep15,tep16);
 int8_t tep18 = *(int8_t*)tep17;
 tep17 = (void*)tep13;

 /*Line:59*/

 int64_t tep19 = tep3;
 int64_t tep20 = 1;
 int64_t tep21 = tep19+tep20;
 tep3 = tep21;
 goto _label1;

 _label0:

 return;

}
void* ULang_String_t_char__Data_ULang_String_t_char__imut(ULang_String_t_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:62*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_char__Data_ULang_Vector_char__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_String_t_char__Data_ULang_String_t_char__(ULang_String_t_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:63*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_char__Data_ULang_Vector_char__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_char__Size_ULang_String_t_char__imut(ULang_String_t_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:66*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_char__Size_ULang_Vector_char__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_char__Capacity_ULang_String_t_char__imut(ULang_String_t_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:67*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_char__Capacity_ULang_Vector_char__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_String_t_char__Resize_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:69*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_char__Resize_ULang_Vector_char___uptr(tep0,tep1);
 return;

}
void ULang_String_t_char__Reserve_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:70*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_char__Reserve_ULang_Vector_char___uptr(tep0,tep1);
 return;

}
void ULang_String_t_char__Clear_ULang_String_t_char__(ULang_String_t_char_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:71*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 ULang_Vector_char__Clear_ULang_Vector_char__(tep0);
 return;

}
int8_t ULang_String_t_char__Pop_ULang_String_t_char__(ULang_String_t_char_* __this__)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:73*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int8_t tep1 = ULang_Vector_char__Pop_ULang_Vector_char__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int8_t ULang_String_t_char__Remove_ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Index)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:74*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 int8_t tep2 = ULang_Vector_char__Remove_ULang_Vector_char___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_char__Push_ULang_String_t_char___c_imut(ULang_String_t_char_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:76*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 void* tep1 = Val;
 ULang_Vector_char__Push_ULang_Vector_char___c_imut(tep0,tep1);
 return;

}
void ULang_String_t_char__Insert_ULang_String_t_char___uptr_c_imut(ULang_String_t_char_* __this__,int64_t Index,void* Item)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = Item;
 ULang_Vector_char__Insert_ULang_Vector_char___uptr_c_imut(tep0,tep1,tep2);
 return;

}
void* ULang_String_t_char____Index___ULang_String_t_char___uptr(ULang_String_t_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

 
 /*Line:83*/

 ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_char____Index___ULang_Vector_char___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void* ULang_String_t_char____Index___ULang_String_t_char__imut_uptr(ULang_String_t_char_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:83*/

 
 /*Line:84*/

 ULang_Vector_char_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_char____Index___ULang_Vector_char___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_char__Append_ULang_String_t_char___ULang_Span_char_imut(ULang_String_t_char_* __this__,ULang_Span_char_ Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:86*/

  ULang_Vector_char_* tep0 = &__this__->__0;
 ULang_Span_char_ tep1 = Val;
 ULang_Vector_char__Append_ULang_Vector_char___ULang_Span_char_imut(tep0,tep1);
 return;

}
int8_t ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:90*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_String_t_char_* tep1 = tep0;
 ULang_StringSpan_t_char_ tep2 = ULang_String_t_char__AsStrSpan_ULang_String_t_char__(tep1);
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_String_t_char_* tep4 = Other;
 ULang_StringSpan_t_char_ tep5 = ULang_String_t_char__AsStrSpan_ULang_String_t_char__imut(tep4);
 ULang_StringSpan_t_char_* tep6 = &tep5;
 int8_t tep7 = ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(tep3,tep6);
 _ReturnVal = tep7;
 return _ReturnVal;

}
int8_t ULang_String_t_char____notequal___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:92*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_String_t_char_* tep1 = tep0;
 ULang_String_t_char_* tep2 = Other;
 int8_t tep3 = ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_String_t_char__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_String_t_char_ ULang_String_t_char____Plus___ULang_String_t_char__imut_ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other)
{
 ULang_String_t_char_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_String_t_char_ ULang_String_t_char____Compound_Plus___ULang_String_t_char___ULang_String_t_char__imut(ULang_String_t_char_* __this__,ULang_String_t_char_* Other)
{
 ULang_String_t_char_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:100*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_String_t_char_* tep1 = tep0;
 ULang_StringSpan_t_char_ tep2 = ULang_String_t_char__AsStrSpan_ULang_String_t_char__(tep1);
 ULang_StringSpan_t_char_* tep3 = &tep2;
 ULang_StringSpan_t_char_* tep4 = Other;
 int8_t tep5 = ULang_StringSpan_t_char____equal___ULang_StringSpan_t_char__imut_ULang_StringSpan_t_char__imut(tep3,tep4);
 _ReturnVal = tep5;
 return _ReturnVal;

}
int8_t ULang_String_t_char____notequal___ULang_String_t_char__imut_ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_String_t_char_* tep1 = tep0;
 ULang_StringSpan_t_char_* tep2 = Other;
 int8_t tep3 = ULang_String_t_char____equal___ULang_String_t_char__imut_ULang_StringSpan_t_char__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_char____Compound_Plus___ULang_String_t_char___ULang_StringSpan_t_char__imut(ULang_String_t_char_* __this__,ULang_StringSpan_t_char_* Other)
{ /*Block*/ 


 return;

}
ULang_Span_char_ ULang_String_t_char__AsSpan_ULang_String_t_char__(ULang_String_t_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 ULang_Span_char_____new___ULang_Span_char__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_char_ ULang_String_t_char__AsSpan_ULang_String_t_char__imut(ULang_String_t_char_* __this__)
{
 ULang_Span_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_char_ tep0;
 ULang_Span_char_* tep1 = &tep0;
 ULang_Span_char_____new___ULang_Span_char__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_String_t_char__AsStrSpan_ULang_String_t_char__(ULang_String_t_char_* __this__)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_char_ tep0;
 ULang_StringSpan_t_char_* tep1 = &tep0;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_String_t_char__AsStrSpan_ULang_String_t_char__imut(ULang_String_t_char_* __this__)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_char_ tep0;
 ULang_StringSpan_t_char_* tep1 = &tep0;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_String_t_char____Index___ULang_String_t_char___ULang_Range_t_uintptr_(ULang_String_t_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/
 /*Line:114*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_StringSpan_t_char_ tep1 = ULang_String_t_char__AsStrSpan_ULang_String_t_char__(tep0);
 ULang_StringSpan_t_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_char_ tep4 = ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_char_ ULang_String_t_char____Index___ULang_String_t_char__imut_ULang_Range_t_uintptr_(ULang_String_t_char_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:114*/
 /*Line:115*/

 ULang_String_t_char_* tep0 = __this__;
 ULang_StringSpan_t_char_ tep1 = ULang_String_t_char__AsStrSpan_ULang_String_t_char__imut(tep0);
 ULang_StringSpan_t_char_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_char_ tep4 = ULang_StringSpan_t_char____Index___ULang_StringSpan_t_char__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_char_____drop___ULang_String_t_char__(ULang_String_t_char_* __this__)
{ /*Block*/ 
 ULang_String_t_char_* tep0 = __this__;
 ULang_String_t_char_____de_init__(tep0);
 return;

}
void ULang_String_t_char_____de_init__(ULang_String_t_char_* __this__)
{ /*Block*/ 
 
 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Vector_char_* tep0 = &__this__->__0;
 ULang_Vector_char_____drop___ULang_Vector_char__(tep0);
 return;

}
void ULang_Span_uint8_____new___ULang_Span_uint8__(ULang_Span_uint8_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(ULang_Span_uint8_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_uint8__Size_ULang_Span_uint8__imut(ULang_Span_uint8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uint8__Data_ULang_Span_uint8__imut(ULang_Span_uint8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uint8____Index___ULang_Span_uint8___uptr(ULang_Span_uint8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_uint8____Index___ULang_Span_uint8__imut_uptr(ULang_Span_uint8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Span_uint8____Index___ULang_Span_uint8___ULang_Range_t_uintptr_(ULang_Span_uint8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uint8_ tep0;
 ULang_Span_uint8_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Span_uint8____Index___ULang_Span_uint8__imut_ULang_Range_t_uintptr_(ULang_Span_uint8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uint8_ tep0;
 ULang_Span_uint8_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uint8_____new___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_uint8_____drop___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_uint8__Data_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_uint8__Data_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uint8__Size_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uint8__Capacity_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uint8__Resize_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_uint8_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_uint8__Reserve_ULang_Vector_uint8___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_uint8__Reserve_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 1;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int8_t tep17 = 0;
 *(int8_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 1;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int8_t tep31 = *(int8_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 1;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int8_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_uint8__Clear_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int8_t ULang_Vector_uint8__Pop_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_Vector_uint8__Remove_ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Index)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_uint8__Push_ULang_Vector_uint8___u8_imut(ULang_Vector_uint8_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_uint8_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_uint8__Resize_ULang_Vector_uint8___uptr(tep0,tep3);

 /*Line:48*/

 int8_t tep4 = *(int8_t*)Val;
 ULang_Vector_uint8_* tep5 = __this__;
 ULang_Vector_uint8_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_uint8____Index___ULang_Vector_uint8___uptr(tep6,tep9);
 int8_t tep11 = *(int8_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_uint8__Insert_ULang_Vector_uint8___uptr_u8_imut(ULang_Vector_uint8_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_uint8__Append_ULang_Vector_uint8___ULang_Span_uint8_imut(ULang_Vector_uint8_* __this__,ULang_Span_uint8_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_uint8____Index___ULang_Vector_uint8___uptr(ULang_Vector_uint8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_uint8____Index___ULang_Vector_uint8__imut_uptr(ULang_Vector_uint8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Vector_uint8____Index___ULang_Vector_uint8___ULang_Range_t_uintptr_(ULang_Vector_uint8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_uint8_* tep0 = __this__;
 ULang_Span_uint8_ tep1 = ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__(tep0);
 ULang_Span_uint8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uint8_ tep4 = ULang_Span_uint8____Index___ULang_Span_uint8___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Vector_uint8____Index___ULang_Vector_uint8__imut_ULang_Range_t_uintptr_(ULang_Vector_uint8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_uint8_* tep0 = __this__;
 ULang_Span_uint8_ tep1 = ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__imut(tep0);
 ULang_Span_uint8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uint8_ tep4 = ULang_Span_uint8____Index___ULang_Span_uint8__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uint8_ tep0;
 ULang_Span_uint8_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uint8_ ULang_Vector_uint8__AsSpan_ULang_Vector_uint8__imut(ULang_Vector_uint8_* __this__)
{
 ULang_Span_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uint8_ tep0;
 ULang_Span_uint8_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uint8_____new___ULang_Span_uint8___u8____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
int32_t ULang_Math_Max_sint32__i32_i32(int32_t A,int32_t B)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Math.uc*/
 /*Line:6*/

 int32_t tep0 = A;
 int32_t tep1 = B;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };
 int32_t tep4 = A;
 _ReturnVal = tep4;
 goto _label1;

 /*Line:7*/
 _label0:

 int32_t tep5 = B;
 _ReturnVal = tep5;

 _label1:

 return _ReturnVal;

}
int32_t ULang_Math_Min_sint32__i32_i32(int32_t A,int32_t B)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Math.uc*/
 /*Line:10*/

 int32_t tep0 = A;
 int32_t tep1 = B;
 int8_t tep2 = (int8_t)(tep0<tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };
 int32_t tep4 = A;
 _ReturnVal = tep4;
 goto _label1;

 /*Line:11*/
 _label0:

 int32_t tep5 = B;
 _ReturnVal = tep5;

 _label1:

 return _ReturnVal;

}
void ULang_Span_sint32_____new___ULang_Span_sint32__(ULang_Span_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(ULang_Span_sint32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_sint32__Size_ULang_Span_sint32__imut(ULang_Span_sint32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_sint32__Data_ULang_Span_sint32__imut(ULang_Span_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_sint32____Index___ULang_Span_sint32___uptr(ULang_Span_sint32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_sint32____Index___ULang_Span_sint32__imut_uptr(ULang_Span_sint32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Span_sint32____Index___ULang_Span_sint32___ULang_Range_t_uintptr_(ULang_Span_sint32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_sint32_ tep0;
 ULang_Span_sint32_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 4;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Span_sint32____Index___ULang_Span_sint32__imut_ULang_Range_t_uintptr_(ULang_Span_sint32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_sint32_ tep0;
 ULang_Span_sint32_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 4;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_sint32_____new___ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_sint32_____drop___ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_sint32__Data_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_sint32__Data_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_sint32__Size_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_sint32__Capacity_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_sint32__Resize_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_sint32_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_sint32__Reserve_ULang_Vector_sint32___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_sint32__Reserve_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 4;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int32_t tep17 = 0;
 *(int32_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 4;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int32_t tep31 = *(int32_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 4;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int32_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_sint32__Clear_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int32_t ULang_Vector_sint32__Pop_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{
 int32_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int32_t ULang_Vector_sint32__Remove_ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Index)
{
 int32_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_sint32__Push_ULang_Vector_sint32___i32_imut(ULang_Vector_sint32_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_sint32_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_sint32__Resize_ULang_Vector_sint32___uptr(tep0,tep3);

 /*Line:48*/

 int32_t tep4 = *(int32_t*)Val;
 ULang_Vector_sint32_* tep5 = __this__;
 ULang_Vector_sint32_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_sint32____Index___ULang_Vector_sint32___uptr(tep6,tep9);
 int32_t tep11 = *(int32_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_sint32__Insert_ULang_Vector_sint32___uptr_i32_imut(ULang_Vector_sint32_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_sint32__Append_ULang_Vector_sint32___ULang_Span_sint32_imut(ULang_Vector_sint32_* __this__,ULang_Span_sint32_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_sint32____Index___ULang_Vector_sint32___uptr(ULang_Vector_sint32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_sint32____Index___ULang_Vector_sint32__imut_uptr(ULang_Vector_sint32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Vector_sint32____Index___ULang_Vector_sint32___ULang_Range_t_uintptr_(ULang_Vector_sint32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_sint32_* tep0 = __this__;
 ULang_Span_sint32_ tep1 = ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__(tep0);
 ULang_Span_sint32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_sint32_ tep4 = ULang_Span_sint32____Index___ULang_Span_sint32___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Vector_sint32____Index___ULang_Vector_sint32__imut_ULang_Range_t_uintptr_(ULang_Vector_sint32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_sint32_* tep0 = __this__;
 ULang_Span_sint32_ tep1 = ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__imut(tep0);
 ULang_Span_sint32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_sint32_ tep4 = ULang_Span_sint32____Index___ULang_Span_sint32__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__(ULang_Vector_sint32_* __this__)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_sint32_ tep0;
 ULang_Span_sint32_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_sint32_ ULang_Vector_sint32__AsSpan_ULang_Vector_sint32__imut(ULang_Vector_sint32_* __this__)
{
 ULang_Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_sint32_ tep0;
 ULang_Span_sint32_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_sint32_____new___ULang_Span_sint32___i32____uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Range_t_uintptr_ ULang_Range_uintptr__uptr_uptr(int64_t start,int64_t end)
{
 ULang_Range_t_uintptr_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Range.uc*/
 /*Line:0*/

 ULang_Range_t_uintptr_ tep0;
 ULang_Range_t_uintptr_* tep1 = &tep0;

 /*Line:37*/

 int64_t tep2 = start;
 int64_t tep3 = end;
 ULang_Range_t_uintptr_____new___ULang_Range_t_uintptr___uptr_uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Span_uft8_____new___ULang_Span_uft8__(ULang_Span_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(ULang_Span_uft8_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_uft8__Size_ULang_Span_uft8__imut(ULang_Span_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft8__Data_ULang_Span_uft8__imut(ULang_Span_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft8____Index___ULang_Span_uft8___uptr(ULang_Span_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_uft8____Index___ULang_Span_uft8__imut_uptr(ULang_Span_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Span_uft8____Index___ULang_Span_uft8___ULang_Range_t_uintptr_(ULang_Span_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Span_uft8____Index___ULang_Span_uft8__imut_ULang_Range_t_uintptr_(ULang_Span_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 1;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft8_____new___ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_uft8_____drop___ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_uft8__Data_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_uft8__Data_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft8__Size_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft8__Capacity_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft8__Resize_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_uft8_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_uft8__Reserve_ULang_Vector_uft8___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_uft8__Reserve_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 1;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int8_t tep17 = 0;
 *(int8_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 1;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int8_t tep31 = *(int8_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 1;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int8_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_uft8__Clear_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int8_t ULang_Vector_uft8__Pop_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_Vector_uft8__Remove_ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Index)
{
 int8_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_uft8__Push_ULang_Vector_uft8____imut(ULang_Vector_uft8_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_uft8_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_uft8__Resize_ULang_Vector_uft8___uptr(tep0,tep3);

 /*Line:48*/

 int8_t tep4 = *(int8_t*)Val;
 ULang_Vector_uft8_* tep5 = __this__;
 ULang_Vector_uft8_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_uft8____Index___ULang_Vector_uft8___uptr(tep6,tep9);
 int8_t tep11 = *(int8_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_uft8__Insert_ULang_Vector_uft8___uptr__imut(ULang_Vector_uft8_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_uft8__Append_ULang_Vector_uft8___ULang_Span_uft8_imut(ULang_Vector_uft8_* __this__,ULang_Span_uft8_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_uft8____Index___ULang_Vector_uft8___uptr(ULang_Vector_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_uft8____Index___ULang_Vector_uft8__imut_uptr(ULang_Vector_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Vector_uft8____Index___ULang_Vector_uft8___ULang_Range_t_uintptr_(ULang_Vector_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_uft8_* tep0 = __this__;
 ULang_Span_uft8_ tep1 = ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__(tep0);
 ULang_Span_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft8_ tep4 = ULang_Span_uft8____Index___ULang_Span_uft8___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Vector_uft8____Index___ULang_Vector_uft8__imut_ULang_Range_t_uintptr_(ULang_Vector_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_uft8_* tep0 = __this__;
 ULang_Span_uft8_ tep1 = ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__imut(tep0);
 ULang_Span_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft8_ tep4 = ULang_Span_uft8____Index___ULang_Span_uft8__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__(ULang_Vector_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_Vector_uft8__AsSpan_ULang_Vector_uft8__imut(ULang_Vector_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:13*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(ULang_StringSpan_t_uft8_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:16*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:17*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:19*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:20*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:21*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft8____equal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:24*/

 ULang_StringSpan_t_uft8_* tep0 = __this__;
 ULang_StringSpan_t_uft8_ tep1 = *(ULang_StringSpan_t_uft8_*)tep0;
 ULang_StringSpan_t_uft8_* tep2 = &tep1;
 int64_t tep3 = ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(tep2);
 ULang_StringSpan_t_uft8_* tep4 = Other;
 int64_t tep5 = ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(tep4);
 int8_t tep6 = (int8_t)(tep3!=tep5);
 int8_t tep7 = !tep6;
 if (tep7){goto _label0; };
 int8_t tep8 = 0;
 _ReturnVal = tep8;
 goto _label1;

 /*Line:26*/
 _label0:

 int64_t tep9;


 int32_t tep10 = 0;
 tep9 = (int64_t)tep10;

 _label4:

 int64_t tep11 = tep9;
 ULang_StringSpan_t_uft8_* tep12 = __this__;
 ULang_StringSpan_t_uft8_ tep13 = *(ULang_StringSpan_t_uft8_*)tep12;
 ULang_StringSpan_t_uft8_* tep14 = &tep13;
 int64_t tep15 = ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(tep14);
 int8_t tep16 = (int8_t)(tep11<tep15);
 int8_t tep17 = !tep16;
 if (tep17){goto _label2; };
 ULang_StringSpan_t_uft8_* tep18 = __this__;

 /*Line:28*/

 ULang_StringSpan_t_uft8_* tep19 = tep18;
 int64_t tep20 = tep9;
 void* tep21 = ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___uptr(tep19,tep20);
 int8_t tep22 = *(int8_t*)tep21;
 ULang_StringSpan_t_uft8_* tep23 = Other;
 int64_t tep24 = tep9;
 void* tep25 = ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___uptr(tep23,tep24);
 int8_t tep26 = *(int8_t*)tep25;
 int8_t tep27 = tep22!=tep26;
 int8_t tep28 = !tep27;
 if (tep28){goto _label3; };
 int8_t tep29 = 0;
 _ReturnVal = tep29;
 goto _label1;

 /*Line:26*/
 _label3:

 int64_t tep30 = tep9;
 int64_t tep31 = 1;
 int64_t tep32 = tep30+tep31;
 tep9 = tep32;
 goto _label4;

 /*Line:30*/
 _label2:

 int8_t tep33 = 1;
 _ReturnVal = tep33;

 _label1:

 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft8____notequal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:32*/

 ULang_StringSpan_t_uft8_* tep0 = __this__;
 ULang_StringSpan_t_uft8_* tep1 = tep0;
 ULang_StringSpan_t_uft8_* tep2 = Other;
 int8_t tep3 = ULang_StringSpan_t_uft8____equal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:34*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 
 /*Line:35*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft8_____new___ULang_Span_uft8_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__(ULang_Span_uft8_* span)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft8_ tep0;
 ULang_StringSpan_t_uft8_* tep1 = &tep0;

 /*Line:38*/

 ULang_Span_uft8_* tep2 = span;
 void* tep3 = ULang_Span_uft8__Data_ULang_Span_uft8__imut(tep2);
 ULang_Span_uft8_* tep4 = span;
 int64_t tep5 = ULang_Span_uft8__Size_ULang_Span_uft8__imut(tep4);
 ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__imut(ULang_Span_uft8_* span)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft8_ tep0;
 ULang_StringSpan_t_uft8_* tep1 = &tep0;

 /*Line:39*/

 ULang_Span_uft8_* tep2 = span;
 void* tep3 = ULang_Span_uft8__Data_ULang_Span_uft8__imut(tep2);
 ULang_Span_uft8_* tep4 = span;
 int64_t tep5 = ULang_Span_uft8__Size_ULang_Span_uft8__imut(tep4);
 ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___uptr(ULang_StringSpan_t_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:41*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8__imut_uptr(ULang_StringSpan_t_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:42*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 1;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:44*/
 /*Line:42*/
 /*Line:44*/

 ULang_StringSpan_t_uft8_* tep0 = __this__;
 ULang_Span_uft8_ tep1 = ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__(tep0);
 ULang_Span_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft8_ tep4 = ULang_Span_uft8____Index___ULang_Span_uft8___ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft8_* tep5 = &tep4;
 ULang_StringSpan_t_uft8_ tep6 = ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:45*/
 /*Line:44*/
 /*Line:45*/

 ULang_StringSpan_t_uft8_* tep0 = __this__;
 ULang_Span_uft8_ tep1 = ULang_StringSpan_t_uft8__AsSpan_ULang_StringSpan_t_uft8__imut(tep0);
 ULang_Span_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft8_ tep4 = ULang_Span_uft8____Index___ULang_Span_uft8__imut_ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft8_* tep5 = &tep4;
 ULang_StringSpan_t_uft8_ tep6 = ULang_StringSpan_t_uft8__ToStr_ULang_Span_uft8__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
void ULang_String_t_uft8_____new___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{ /*Block*/ 


 return;

}
void ULang_String_t_uft8_____new___ULang_String_t_uft8___ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* span)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:58*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_StringSpan_t_uft8_* tep1 = span;
 int64_t tep2 = ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(tep1);
 ULang_String_t_uft8__Resize_ULang_String_t_uft8___uptr(tep0,tep2);

 /*Line:59*/

 int64_t tep3;


 int32_t tep4 = 0;
 tep3 = (int64_t)tep4;

 _label1:

 int64_t tep5 = tep3;
 ULang_StringSpan_t_uft8_* tep6 = span;
 int64_t tep7 = ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(tep6);
 int8_t tep8 = (int8_t)(tep5<tep7);
 int8_t tep9 = !tep8;
 if (tep9){goto _label0; };

 /*Line:60*/
 /*Line:59*/
 /*Line:60*/

 ULang_StringSpan_t_uft8_* tep10 = span;
 int64_t tep11 = tep3;
 void* tep12 = ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___uptr(tep10,tep11);
 int8_t tep13 = *(int8_t*)tep12;
 ULang_String_t_uft8_* tep14 = __this__;
 ULang_String_t_uft8_* tep15 = tep14;
 int64_t tep16 = tep3;
 void* tep17 = ULang_String_t_uft8____Index___ULang_String_t_uft8___uptr(tep15,tep16);
 int8_t tep18 = *(int8_t*)tep17;
 tep17 = (void*)tep13;

 /*Line:59*/

 int64_t tep19 = tep3;
 int64_t tep20 = 1;
 int64_t tep21 = tep19+tep20;
 tep3 = tep21;
 goto _label1;

 _label0:

 return;

}
void* ULang_String_t_uft8__Data_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:62*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft8__Data_ULang_Vector_uft8__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_String_t_uft8__Data_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:63*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft8__Data_ULang_Vector_uft8__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft8__Size_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:66*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft8__Size_ULang_Vector_uft8__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft8__Capacity_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:67*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft8__Capacity_ULang_Vector_uft8__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_String_t_uft8__Resize_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:69*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft8__Resize_ULang_Vector_uft8___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft8__Reserve_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:70*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft8__Reserve_ULang_Vector_uft8___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft8__Clear_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:71*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 ULang_Vector_uft8__Clear_ULang_Vector_uft8__(tep0);
 return;

}
int8_t ULang_String_t_uft8__Pop_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:73*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int8_t tep1 = ULang_Vector_uft8__Pop_ULang_Vector_uft8__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int8_t ULang_String_t_uft8__Remove_ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Index)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:74*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 int8_t tep2 = ULang_Vector_uft8__Remove_ULang_Vector_uft8___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft8__Push_ULang_String_t_uft8____imut(ULang_String_t_uft8_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:76*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 void* tep1 = Val;
 ULang_Vector_uft8__Push_ULang_Vector_uft8____imut(tep0,tep1);
 return;

}
void ULang_String_t_uft8__Insert_ULang_String_t_uft8___uptr__imut(ULang_String_t_uft8_* __this__,int64_t Index,void* Item)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = Item;
 ULang_Vector_uft8__Insert_ULang_Vector_uft8___uptr__imut(tep0,tep1,tep2);
 return;

}
void* ULang_String_t_uft8____Index___ULang_String_t_uft8___uptr(ULang_String_t_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

 
 /*Line:83*/

 ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft8____Index___ULang_Vector_uft8___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void* ULang_String_t_uft8____Index___ULang_String_t_uft8__imut_uptr(ULang_String_t_uft8_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:83*/

 
 /*Line:84*/

 ULang_Vector_uft8_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft8____Index___ULang_Vector_uft8___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft8__Append_ULang_String_t_uft8___ULang_Span_uft8_imut(ULang_String_t_uft8_* __this__,ULang_Span_uft8_ Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:86*/

  ULang_Vector_uft8_* tep0 = &__this__->__0;
 ULang_Span_uft8_ tep1 = Val;
 ULang_Vector_uft8__Append_ULang_Vector_uft8___ULang_Span_uft8_imut(tep0,tep1);
 return;

}
int8_t ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:90*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_String_t_uft8_* tep1 = tep0;
 ULang_StringSpan_t_uft8_ tep2 = ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__(tep1);
 ULang_StringSpan_t_uft8_* tep3 = &tep2;
 ULang_String_t_uft8_* tep4 = Other;
 ULang_StringSpan_t_uft8_ tep5 = ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__imut(tep4);
 ULang_StringSpan_t_uft8_* tep6 = &tep5;
 int8_t tep7 = ULang_StringSpan_t_uft8____equal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(tep3,tep6);
 _ReturnVal = tep7;
 return _ReturnVal;

}
int8_t ULang_String_t_uft8____notequal___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:92*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_String_t_uft8_* tep1 = tep0;
 ULang_String_t_uft8_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_String_t_uft8_ ULang_String_t_uft8____Plus___ULang_String_t_uft8__imut_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other)
{
 ULang_String_t_uft8_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_String_t_uft8_ ULang_String_t_uft8____Compound_Plus___ULang_String_t_uft8___ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_String_t_uft8_* Other)
{
 ULang_String_t_uft8_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:100*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_String_t_uft8_* tep1 = tep0;
 ULang_StringSpan_t_uft8_ tep2 = ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__(tep1);
 ULang_StringSpan_t_uft8_* tep3 = &tep2;
 ULang_StringSpan_t_uft8_* tep4 = Other;
 int8_t tep5 = ULang_StringSpan_t_uft8____equal___ULang_StringSpan_t_uft8__imut_ULang_StringSpan_t_uft8__imut(tep3,tep4);
 _ReturnVal = tep5;
 return _ReturnVal;

}
int8_t ULang_String_t_uft8____notequal___ULang_String_t_uft8__imut_ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_String_t_uft8_* tep1 = tep0;
 ULang_StringSpan_t_uft8_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft8____equal___ULang_String_t_uft8__imut_ULang_StringSpan_t_uft8__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft8____Compound_Plus___ULang_String_t_uft8___ULang_StringSpan_t_uft8__imut(ULang_String_t_uft8_* __this__,ULang_StringSpan_t_uft8_* Other)
{ /*Block*/ 


 return;

}
ULang_Span_uft8_ ULang_String_t_uft8__AsSpan_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 ULang_Span_uft8_____new___ULang_Span_uft8__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft8_ ULang_String_t_uft8__AsSpan_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__)
{
 ULang_Span_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft8_ tep0;
 ULang_Span_uft8_* tep1 = &tep0;
 ULang_Span_uft8_____new___ULang_Span_uft8__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft8_ tep0;
 ULang_StringSpan_t_uft8_* tep1 = &tep0;
 ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__imut(ULang_String_t_uft8_* __this__)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft8_ tep0;
 ULang_StringSpan_t_uft8_* tep1 = &tep0;
 ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_String_t_uft8____Index___ULang_String_t_uft8___ULang_Range_t_uintptr_(ULang_String_t_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/
 /*Line:114*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_StringSpan_t_uft8_ tep1 = ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__(tep0);
 ULang_StringSpan_t_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft8_ tep4 = ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft8_ ULang_String_t_uft8____Index___ULang_String_t_uft8__imut_ULang_Range_t_uintptr_(ULang_String_t_uft8_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft8_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:114*/
 /*Line:115*/

 ULang_String_t_uft8_* tep0 = __this__;
 ULang_StringSpan_t_uft8_ tep1 = ULang_String_t_uft8__AsStrSpan_ULang_String_t_uft8__imut(tep0);
 ULang_StringSpan_t_uft8_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft8_ tep4 = ULang_StringSpan_t_uft8____Index___ULang_StringSpan_t_uft8__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft8_____drop___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{ /*Block*/ 
 ULang_String_t_uft8_* tep0 = __this__;
 ULang_String_t_uft8_____de_init__(tep0);
 return;

}
void ULang_String_t_uft8_____de_init__(ULang_String_t_uft8_* __this__)
{ /*Block*/ 
 
 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Vector_uft8_* tep0 = &__this__->__0;
 ULang_Vector_uft8_____drop___ULang_Vector_uft8__(tep0);
 return;

}
void ULang_Span_uft16_____new___ULang_Span_uft16__(ULang_Span_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(ULang_Span_uft16_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_uft16__Size_ULang_Span_uft16__imut(ULang_Span_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft16__Data_ULang_Span_uft16__imut(ULang_Span_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft16____Index___ULang_Span_uft16___uptr(ULang_Span_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_uft16____Index___ULang_Span_uft16__imut_uptr(ULang_Span_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Span_uft16____Index___ULang_Span_uft16___ULang_Range_t_uintptr_(ULang_Span_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 2;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Span_uft16____Index___ULang_Span_uft16__imut_ULang_Range_t_uintptr_(ULang_Span_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 2;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft16_____new___ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_uft16_____drop___ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_uft16__Data_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_uft16__Data_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft16__Size_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft16__Capacity_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft16__Resize_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_uft16_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_uft16__Reserve_ULang_Vector_uft16___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_uft16__Reserve_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 2;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int16_t tep17 = 0;
 *(int16_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 2;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int16_t tep31 = *(int16_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 2;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int16_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_uft16__Clear_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int16_t ULang_Vector_uft16__Pop_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{
 int16_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int16_t ULang_Vector_uft16__Remove_ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Index)
{
 int16_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_uft16__Push_ULang_Vector_uft16____imut(ULang_Vector_uft16_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_uft16_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_uft16__Resize_ULang_Vector_uft16___uptr(tep0,tep3);

 /*Line:48*/

 int16_t tep4 = *(int16_t*)Val;
 ULang_Vector_uft16_* tep5 = __this__;
 ULang_Vector_uft16_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_uft16____Index___ULang_Vector_uft16___uptr(tep6,tep9);
 int16_t tep11 = *(int16_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_uft16__Insert_ULang_Vector_uft16___uptr__imut(ULang_Vector_uft16_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_uft16__Append_ULang_Vector_uft16___ULang_Span_uft16_imut(ULang_Vector_uft16_* __this__,ULang_Span_uft16_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_uft16____Index___ULang_Vector_uft16___uptr(ULang_Vector_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_uft16____Index___ULang_Vector_uft16__imut_uptr(ULang_Vector_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Vector_uft16____Index___ULang_Vector_uft16___ULang_Range_t_uintptr_(ULang_Vector_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_uft16_* tep0 = __this__;
 ULang_Span_uft16_ tep1 = ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__(tep0);
 ULang_Span_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft16_ tep4 = ULang_Span_uft16____Index___ULang_Span_uft16___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Vector_uft16____Index___ULang_Vector_uft16__imut_ULang_Range_t_uintptr_(ULang_Vector_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_uft16_* tep0 = __this__;
 ULang_Span_uft16_ tep1 = ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__imut(tep0);
 ULang_Span_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft16_ tep4 = ULang_Span_uft16____Index___ULang_Span_uft16__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__(ULang_Vector_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_Vector_uft16__AsSpan_ULang_Vector_uft16__imut(ULang_Vector_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:13*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(ULang_StringSpan_t_uft16_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:16*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:17*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:19*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:20*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:21*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft16____equal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:24*/

 ULang_StringSpan_t_uft16_* tep0 = __this__;
 ULang_StringSpan_t_uft16_ tep1 = *(ULang_StringSpan_t_uft16_*)tep0;
 ULang_StringSpan_t_uft16_* tep2 = &tep1;
 int64_t tep3 = ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(tep2);
 ULang_StringSpan_t_uft16_* tep4 = Other;
 int64_t tep5 = ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(tep4);
 int8_t tep6 = (int8_t)(tep3!=tep5);
 int8_t tep7 = !tep6;
 if (tep7){goto _label0; };
 int8_t tep8 = 0;
 _ReturnVal = tep8;
 goto _label1;

 /*Line:26*/
 _label0:

 int64_t tep9;


 int32_t tep10 = 0;
 tep9 = (int64_t)tep10;

 _label4:

 int64_t tep11 = tep9;
 ULang_StringSpan_t_uft16_* tep12 = __this__;
 ULang_StringSpan_t_uft16_ tep13 = *(ULang_StringSpan_t_uft16_*)tep12;
 ULang_StringSpan_t_uft16_* tep14 = &tep13;
 int64_t tep15 = ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(tep14);
 int8_t tep16 = (int8_t)(tep11<tep15);
 int8_t tep17 = !tep16;
 if (tep17){goto _label2; };
 ULang_StringSpan_t_uft16_* tep18 = __this__;

 /*Line:28*/

 ULang_StringSpan_t_uft16_* tep19 = tep18;
 int64_t tep20 = tep9;
 void* tep21 = ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___uptr(tep19,tep20);
 int16_t tep22 = *(int16_t*)tep21;
 ULang_StringSpan_t_uft16_* tep23 = Other;
 int64_t tep24 = tep9;
 void* tep25 = ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___uptr(tep23,tep24);
 int16_t tep26 = *(int16_t*)tep25;
 int8_t tep27 = (int8_t)(tep22!=tep26);
 int8_t tep28 = !tep27;
 if (tep28){goto _label3; };
 int8_t tep29 = 0;
 _ReturnVal = tep29;
 goto _label1;

 /*Line:26*/
 _label3:

 int64_t tep30 = tep9;
 int64_t tep31 = 1;
 int64_t tep32 = tep30+tep31;
 tep9 = tep32;
 goto _label4;

 /*Line:30*/
 _label2:

 int8_t tep33 = 1;
 _ReturnVal = tep33;

 _label1:

 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft16____notequal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:32*/

 ULang_StringSpan_t_uft16_* tep0 = __this__;
 ULang_StringSpan_t_uft16_* tep1 = tep0;
 ULang_StringSpan_t_uft16_* tep2 = Other;
 int8_t tep3 = ULang_StringSpan_t_uft16____equal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:34*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 
 /*Line:35*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft16_____new___ULang_Span_uft16_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__(ULang_Span_uft16_* span)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft16_ tep0;
 ULang_StringSpan_t_uft16_* tep1 = &tep0;

 /*Line:38*/

 ULang_Span_uft16_* tep2 = span;
 void* tep3 = ULang_Span_uft16__Data_ULang_Span_uft16__imut(tep2);
 ULang_Span_uft16_* tep4 = span;
 int64_t tep5 = ULang_Span_uft16__Size_ULang_Span_uft16__imut(tep4);
 ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__imut(ULang_Span_uft16_* span)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft16_ tep0;
 ULang_StringSpan_t_uft16_* tep1 = &tep0;

 /*Line:39*/

 ULang_Span_uft16_* tep2 = span;
 void* tep3 = ULang_Span_uft16__Data_ULang_Span_uft16__imut(tep2);
 ULang_Span_uft16_* tep4 = span;
 int64_t tep5 = ULang_Span_uft16__Size_ULang_Span_uft16__imut(tep4);
 ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___uptr(ULang_StringSpan_t_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:41*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16__imut_uptr(ULang_StringSpan_t_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:42*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 2;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:44*/
 /*Line:42*/
 /*Line:44*/

 ULang_StringSpan_t_uft16_* tep0 = __this__;
 ULang_Span_uft16_ tep1 = ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__(tep0);
 ULang_Span_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft16_ tep4 = ULang_Span_uft16____Index___ULang_Span_uft16___ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft16_* tep5 = &tep4;
 ULang_StringSpan_t_uft16_ tep6 = ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:45*/
 /*Line:44*/
 /*Line:45*/

 ULang_StringSpan_t_uft16_* tep0 = __this__;
 ULang_Span_uft16_ tep1 = ULang_StringSpan_t_uft16__AsSpan_ULang_StringSpan_t_uft16__imut(tep0);
 ULang_Span_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft16_ tep4 = ULang_Span_uft16____Index___ULang_Span_uft16__imut_ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft16_* tep5 = &tep4;
 ULang_StringSpan_t_uft16_ tep6 = ULang_StringSpan_t_uft16__ToStr_ULang_Span_uft16__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
void ULang_String_t_uft16_____new___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{ /*Block*/ 


 return;

}
void ULang_String_t_uft16_____new___ULang_String_t_uft16___ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* span)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:58*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_StringSpan_t_uft16_* tep1 = span;
 int64_t tep2 = ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(tep1);
 ULang_String_t_uft16__Resize_ULang_String_t_uft16___uptr(tep0,tep2);

 /*Line:59*/

 int64_t tep3;


 int32_t tep4 = 0;
 tep3 = (int64_t)tep4;

 _label1:

 int64_t tep5 = tep3;
 ULang_StringSpan_t_uft16_* tep6 = span;
 int64_t tep7 = ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(tep6);
 int8_t tep8 = (int8_t)(tep5<tep7);
 int8_t tep9 = !tep8;
 if (tep9){goto _label0; };

 /*Line:60*/
 /*Line:59*/
 /*Line:60*/

 ULang_StringSpan_t_uft16_* tep10 = span;
 int64_t tep11 = tep3;
 void* tep12 = ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___uptr(tep10,tep11);
 int16_t tep13 = *(int16_t*)tep12;
 ULang_String_t_uft16_* tep14 = __this__;
 ULang_String_t_uft16_* tep15 = tep14;
 int64_t tep16 = tep3;
 void* tep17 = ULang_String_t_uft16____Index___ULang_String_t_uft16___uptr(tep15,tep16);
 int16_t tep18 = *(int16_t*)tep17;
 tep17 = (void*)tep13;

 /*Line:59*/

 int64_t tep19 = tep3;
 int64_t tep20 = 1;
 int64_t tep21 = tep19+tep20;
 tep3 = tep21;
 goto _label1;

 _label0:

 return;

}
void* ULang_String_t_uft16__Data_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:62*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft16__Data_ULang_Vector_uft16__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_String_t_uft16__Data_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:63*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft16__Data_ULang_Vector_uft16__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft16__Size_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:66*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft16__Size_ULang_Vector_uft16__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft16__Capacity_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:67*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft16__Capacity_ULang_Vector_uft16__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_String_t_uft16__Resize_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:69*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft16__Resize_ULang_Vector_uft16___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft16__Reserve_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:70*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft16__Reserve_ULang_Vector_uft16___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft16__Clear_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:71*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 ULang_Vector_uft16__Clear_ULang_Vector_uft16__(tep0);
 return;

}
int16_t ULang_String_t_uft16__Pop_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{
 int16_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:73*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int16_t tep1 = ULang_Vector_uft16__Pop_ULang_Vector_uft16__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int16_t ULang_String_t_uft16__Remove_ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Index)
{
 int16_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:74*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 int16_t tep2 = ULang_Vector_uft16__Remove_ULang_Vector_uft16___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft16__Push_ULang_String_t_uft16____imut(ULang_String_t_uft16_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:76*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 void* tep1 = Val;
 ULang_Vector_uft16__Push_ULang_Vector_uft16____imut(tep0,tep1);
 return;

}
void ULang_String_t_uft16__Insert_ULang_String_t_uft16___uptr__imut(ULang_String_t_uft16_* __this__,int64_t Index,void* Item)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = Item;
 ULang_Vector_uft16__Insert_ULang_Vector_uft16___uptr__imut(tep0,tep1,tep2);
 return;

}
void* ULang_String_t_uft16____Index___ULang_String_t_uft16___uptr(ULang_String_t_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

 
 /*Line:83*/

 ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft16____Index___ULang_Vector_uft16___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void* ULang_String_t_uft16____Index___ULang_String_t_uft16__imut_uptr(ULang_String_t_uft16_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:83*/

 
 /*Line:84*/

 ULang_Vector_uft16_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft16____Index___ULang_Vector_uft16___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft16__Append_ULang_String_t_uft16___ULang_Span_uft16_imut(ULang_String_t_uft16_* __this__,ULang_Span_uft16_ Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:86*/

  ULang_Vector_uft16_* tep0 = &__this__->__0;
 ULang_Span_uft16_ tep1 = Val;
 ULang_Vector_uft16__Append_ULang_Vector_uft16___ULang_Span_uft16_imut(tep0,tep1);
 return;

}
int8_t ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:90*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_String_t_uft16_* tep1 = tep0;
 ULang_StringSpan_t_uft16_ tep2 = ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__(tep1);
 ULang_StringSpan_t_uft16_* tep3 = &tep2;
 ULang_String_t_uft16_* tep4 = Other;
 ULang_StringSpan_t_uft16_ tep5 = ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__imut(tep4);
 ULang_StringSpan_t_uft16_* tep6 = &tep5;
 int8_t tep7 = ULang_StringSpan_t_uft16____equal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(tep3,tep6);
 _ReturnVal = tep7;
 return _ReturnVal;

}
int8_t ULang_String_t_uft16____notequal___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:92*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_String_t_uft16_* tep1 = tep0;
 ULang_String_t_uft16_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_String_t_uft16_ ULang_String_t_uft16____Plus___ULang_String_t_uft16__imut_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other)
{
 ULang_String_t_uft16_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_String_t_uft16_ ULang_String_t_uft16____Compound_Plus___ULang_String_t_uft16___ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_String_t_uft16_* Other)
{
 ULang_String_t_uft16_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:100*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_String_t_uft16_* tep1 = tep0;
 ULang_StringSpan_t_uft16_ tep2 = ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__(tep1);
 ULang_StringSpan_t_uft16_* tep3 = &tep2;
 ULang_StringSpan_t_uft16_* tep4 = Other;
 int8_t tep5 = ULang_StringSpan_t_uft16____equal___ULang_StringSpan_t_uft16__imut_ULang_StringSpan_t_uft16__imut(tep3,tep4);
 _ReturnVal = tep5;
 return _ReturnVal;

}
int8_t ULang_String_t_uft16____notequal___ULang_String_t_uft16__imut_ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_String_t_uft16_* tep1 = tep0;
 ULang_StringSpan_t_uft16_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft16____equal___ULang_String_t_uft16__imut_ULang_StringSpan_t_uft16__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft16____Compound_Plus___ULang_String_t_uft16___ULang_StringSpan_t_uft16__imut(ULang_String_t_uft16_* __this__,ULang_StringSpan_t_uft16_* Other)
{ /*Block*/ 


 return;

}
ULang_Span_uft16_ ULang_String_t_uft16__AsSpan_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 ULang_Span_uft16_____new___ULang_Span_uft16__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft16_ ULang_String_t_uft16__AsSpan_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__)
{
 ULang_Span_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft16_ tep0;
 ULang_Span_uft16_* tep1 = &tep0;
 ULang_Span_uft16_____new___ULang_Span_uft16__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft16_ tep0;
 ULang_StringSpan_t_uft16_* tep1 = &tep0;
 ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__imut(ULang_String_t_uft16_* __this__)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft16_ tep0;
 ULang_StringSpan_t_uft16_* tep1 = &tep0;
 ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_String_t_uft16____Index___ULang_String_t_uft16___ULang_Range_t_uintptr_(ULang_String_t_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/
 /*Line:114*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_StringSpan_t_uft16_ tep1 = ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__(tep0);
 ULang_StringSpan_t_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft16_ tep4 = ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft16_ ULang_String_t_uft16____Index___ULang_String_t_uft16__imut_ULang_Range_t_uintptr_(ULang_String_t_uft16_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft16_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:114*/
 /*Line:115*/

 ULang_String_t_uft16_* tep0 = __this__;
 ULang_StringSpan_t_uft16_ tep1 = ULang_String_t_uft16__AsStrSpan_ULang_String_t_uft16__imut(tep0);
 ULang_StringSpan_t_uft16_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft16_ tep4 = ULang_StringSpan_t_uft16____Index___ULang_StringSpan_t_uft16__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft16_____drop___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{ /*Block*/ 
 ULang_String_t_uft16_* tep0 = __this__;
 ULang_String_t_uft16_____de_init__(tep0);
 return;

}
void ULang_String_t_uft16_____de_init__(ULang_String_t_uft16_* __this__)
{ /*Block*/ 
 
 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Vector_uft16_* tep0 = &__this__->__0;
 ULang_Vector_uft16_____drop___ULang_Vector_uft16__(tep0);
 return;

}
void ULang_Span_uft32_____new___ULang_Span_uft32__(ULang_Span_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:8*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(ULang_Span_uft32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:11*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:12*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_Span_uft32__Size_ULang_Span_uft32__imut(ULang_Span_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:13*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft32__Data_ULang_Span_uft32__imut(ULang_Span_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Span_uft32____Index___ULang_Span_uft32___uptr(ULang_Span_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Span_uft32____Index___ULang_Span_uft32__imut_uptr(ULang_Span_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Span.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Span_uft32____Index___ULang_Span_uft32___ULang_Range_t_uintptr_(ULang_Span_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:20*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 4;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Span_uft32____Index___ULang_Span_uft32__imut_ULang_Range_t_uintptr_(ULang_Span_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Span.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:21*/

 void* tep2 = __this__->__0;
 ULang_Range_t_uintptr_* tep3 = &Range;
 int64_t tep4 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep3);
 int64_t tep5 = 4;
 int64_t tep6 = tep4*tep5;
 void* tep7 = (void*)((int64_t)tep2+tep6);
 ULang_Range_t_uintptr_* tep8 = &Range;
 int64_t tep9 = ULang_Range_t_uintptr__End_ULang_Range_t_uintptr__imut(tep8);
 ULang_Range_t_uintptr_* tep10 = &Range;
 int64_t tep11 = ULang_Range_t_uintptr__Start_ULang_Range_t_uintptr__imut(tep10);
 int64_t tep12 = tep9-tep11;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep7,tep12);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft32_____new___ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:9*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:10*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:11*/

 int64_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vector_uft32_____drop___ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:13*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:14*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:15*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
void* ULang_Vector_uft32__Data_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:17*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Vector_uft32__Data_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:18*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft32__Size_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:20*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int64_t ULang_Vector_uft32__Capacity_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:21*/

 int64_t tep0 = __this__->__2;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Vector_uft32__Resize_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:24*/

 ULang_Vector_uft32_* tep0 = __this__;
 int64_t tep1 = Size;
 ULang_Vector_uft32__Reserve_ULang_Vector_uft32___uptr(tep0,tep1);

 /*Line:25*/

 int64_t tep2 = Size;
  __this__->__1 = tep2;
 return;

}
void ULang_Vector_uft32__Reserve_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:28*/

 int64_t tep0 = Size;
  int64_t tep1 = __this__->__2;
 int8_t tep2 = (int8_t)(tep0>tep1);
 int8_t tep3 = !tep2;
 if (tep3){goto _label0; };

 /*Line:29*/

 int64_t tep4;
  int64_t tep5 = __this__->__1;
 tep4 = tep5;

 /*Line:30*/

 void* tep6;
  void* tep7 = __this__->__0;
 tep6 = tep7;

 /*Line:32*/

 int64_t tep8 = Size;
  __this__->__2 = tep8;

 /*Line:33*/

 int64_t tep9 = Size;
 int64_t tep10 = 4;
 int64_t tep11 = tep9*tep10;
 void* tep12 = malloc(tep11);
 int64_t tep13 = 0;

 _label2:

 int8_t tep14 = (int8_t)(tep13==tep9);
 if (tep14){goto _label1; };
 int64_t tep15 = tep13*tep10;
 void* tep16 = (void*)((int64_t)tep12+tep15);

 /*Line:0*/

 int32_t tep17 = 0;
 *(int32_t*)tep16 = tep17;
 int32_t tep18 = 1;
 int64_t tep19 = (int64_t)(tep13+(int64_t)tep18);
 tep13 = tep19;
 goto _label2;

 _label1:

  __this__->__0 = tep12;

 /*Line:34*/

 int64_t tep20;


 int32_t tep21 = 0;
 tep20 = (int64_t)tep21;

 _label4:

 int64_t tep22 = tep20;
 int64_t tep23 = tep4;
 int8_t tep24 = (int8_t)(tep22<tep23);
 int8_t tep25 = !tep24;
 if (tep25){goto _label3; };

 /*Line:35*/

 void* tep26 = tep6;
 int64_t tep27 = tep20;
 int64_t tep28 = 4;
 int64_t tep29 = tep27*tep28;
 void* tep30 = (void*)((int64_t)tep26+tep29);
 int32_t tep31 = *(int32_t*)tep30;
  void* tep32 = __this__->__0;
 int64_t tep33 = tep20;
 int64_t tep34 = 4;
 int64_t tep35 = tep33*tep34;
 void* tep36 = (void*)((int64_t)tep32+tep35);
 *(int32_t*)tep36 = tep31;

 /*Line:34*/

 int64_t tep37 = tep20;
 int64_t tep38 = 1;
 int64_t tep39 = tep37+tep38;
 tep20 = tep39;
 goto _label4;

 /*Line:37*/
 _label3:

 int64_t tep40;
 void* tep41 = tep6;
 tep40 = (int64_t)tep41;

 /*Line:38*/

 int64_t tep42 = tep40;
 int64_t tep43 = 0;
 int8_t tep44 = (int8_t)(tep42==tep43);
 int8_t tep45 = !tep44;
 if (tep45){goto _label0; };

 /*Line:39*/

 void* tep46 = tep6;
 free(tep46);

 _label0:

 return;

}
void ULang_Vector_uft32__Clear_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:41*/

 int64_t tep0 = 0;
  __this__->__1 = tep0;
 return;

}
int32_t ULang_Vector_uft32__Pop_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{
 int32_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int32_t ULang_Vector_uft32__Remove_ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Index)
{
 int32_t _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
void ULang_Vector_uft32__Push_ULang_Vector_uft32____imut(ULang_Vector_uft32_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:47*/

 ULang_Vector_uft32_* tep0 = __this__;
  int64_t tep1 = __this__->__1;
 int64_t tep2 = 1;
 int64_t tep3 = tep1+tep1;
 ULang_Vector_uft32__Resize_ULang_Vector_uft32___uptr(tep0,tep3);

 /*Line:48*/

 int32_t tep4 = *(int32_t*)Val;
 ULang_Vector_uft32_* tep5 = __this__;
 ULang_Vector_uft32_* tep6 = tep5;
  int64_t tep7 = __this__->__1;
 int64_t tep8 = 1;
 int64_t tep9 = tep7-tep8;
 void* tep10 = ULang_Vector_uft32____Index___ULang_Vector_uft32___uptr(tep6,tep9);
 int32_t tep11 = *(int32_t*)tep10;
 tep10 = (void*)tep4;
 return;

}
void ULang_Vector_uft32__Insert_ULang_Vector_uft32___uptr__imut(ULang_Vector_uft32_* __this__,int64_t Index,void* Item)
{ /*Block*/ 


 return;

}
void ULang_Vector_uft32__Append_ULang_Vector_uft32___ULang_Span_uft32_imut(ULang_Vector_uft32_* __this__,ULang_Span_uft32_ Val)
{ /*Block*/ 


 return;

}
void* ULang_Vector_uft32____Index___ULang_Vector_uft32___uptr(ULang_Vector_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:61*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_Vector_uft32____Index___ULang_Vector_uft32__imut_uptr(ULang_Vector_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Vector.uc*/
 /*Line:62*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Vector_uft32____Index___ULang_Vector_uft32___ULang_Range_t_uintptr_(ULang_Vector_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:62*/
 /*Line:67*/

 ULang_Vector_uft32_* tep0 = __this__;
 ULang_Span_uft32_ tep1 = ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__(tep0);
 ULang_Span_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft32_ tep4 = ULang_Span_uft32____Index___ULang_Span_uft32___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Vector_uft32____Index___ULang_Vector_uft32__imut_ULang_Range_t_uintptr_(ULang_Vector_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:67*/
 /*Line:68*/

 ULang_Vector_uft32_* tep0 = __this__;
 ULang_Span_uft32_ tep1 = ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__imut(tep0);
 ULang_Span_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft32_ tep4 = ULang_Span_uft32____Index___ULang_Span_uft32__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__(ULang_Vector_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:70*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_Vector_uft32__AsSpan_ULang_Vector_uft32__imut(ULang_Vector_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vector.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:71*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;

 /*Line:13*/

 int64_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(ULang_StringSpan_t_uft32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:16*/

 void* tep0 = data;
  __this__->__0 = tep0;

 /*Line:17*/

 int64_t tep1 = size;
  __this__->__1 = tep1;
 return;

}
int64_t ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:19*/

 int64_t tep0 = __this__->__1;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:20*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:21*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft32____equal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:24*/

 ULang_StringSpan_t_uft32_* tep0 = __this__;
 ULang_StringSpan_t_uft32_ tep1 = *(ULang_StringSpan_t_uft32_*)tep0;
 ULang_StringSpan_t_uft32_* tep2 = &tep1;
 int64_t tep3 = ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(tep2);
 ULang_StringSpan_t_uft32_* tep4 = Other;
 int64_t tep5 = ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(tep4);
 int8_t tep6 = (int8_t)(tep3!=tep5);
 int8_t tep7 = !tep6;
 if (tep7){goto _label0; };
 int8_t tep8 = 0;
 _ReturnVal = tep8;
 goto _label1;

 /*Line:26*/
 _label0:

 int64_t tep9;


 int32_t tep10 = 0;
 tep9 = (int64_t)tep10;

 _label4:

 int64_t tep11 = tep9;
 ULang_StringSpan_t_uft32_* tep12 = __this__;
 ULang_StringSpan_t_uft32_ tep13 = *(ULang_StringSpan_t_uft32_*)tep12;
 ULang_StringSpan_t_uft32_* tep14 = &tep13;
 int64_t tep15 = ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(tep14);
 int8_t tep16 = (int8_t)(tep11<tep15);
 int8_t tep17 = !tep16;
 if (tep17){goto _label2; };
 ULang_StringSpan_t_uft32_* tep18 = __this__;

 /*Line:28*/

 ULang_StringSpan_t_uft32_* tep19 = tep18;
 int64_t tep20 = tep9;
 void* tep21 = ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___uptr(tep19,tep20);
 int32_t tep22 = *(int32_t*)tep21;
 ULang_StringSpan_t_uft32_* tep23 = Other;
 int64_t tep24 = tep9;
 void* tep25 = ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___uptr(tep23,tep24);
 int32_t tep26 = *(int32_t*)tep25;
 int8_t tep27 = (int8_t)(tep22!=tep26);
 int8_t tep28 = !tep27;
 if (tep28){goto _label3; };
 int8_t tep29 = 0;
 _ReturnVal = tep29;
 goto _label1;

 /*Line:26*/
 _label3:

 int64_t tep30 = tep9;
 int64_t tep31 = 1;
 int64_t tep32 = tep30+tep31;
 tep9 = tep32;
 goto _label4;

 /*Line:30*/
 _label2:

 int8_t tep33 = 1;
 _ReturnVal = tep33;

 _label1:

 return _ReturnVal;

}
int8_t ULang_StringSpan_t_uft32____notequal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:32*/

 ULang_StringSpan_t_uft32_* tep0 = __this__;
 ULang_StringSpan_t_uft32_* tep1 = tep0;
 ULang_StringSpan_t_uft32_* tep2 = Other;
 int8_t tep3 = ULang_StringSpan_t_uft32____equal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:34*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 
 /*Line:35*/

 void* tep2 = __this__->__0;
  int64_t tep3 = __this__->__1;
 ULang_Span_uft32_____new___ULang_Span_uft32_______uptr(tep1,tep2,tep3);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__(ULang_Span_uft32_* span)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft32_ tep0;
 ULang_StringSpan_t_uft32_* tep1 = &tep0;

 /*Line:38*/

 ULang_Span_uft32_* tep2 = span;
 void* tep3 = ULang_Span_uft32__Data_ULang_Span_uft32__imut(tep2);
 ULang_Span_uft32_* tep4 = span;
 int64_t tep5 = ULang_Span_uft32__Size_ULang_Span_uft32__imut(tep4);
 ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__imut(ULang_Span_uft32_* span)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft32_ tep0;
 ULang_StringSpan_t_uft32_* tep1 = &tep0;

 /*Line:39*/

 ULang_Span_uft32_* tep2 = span;
 void* tep3 = ULang_Span_uft32__Data_ULang_Span_uft32__imut(tep2);
 ULang_Span_uft32_* tep4 = span;
 int64_t tep5 = ULang_Span_uft32__Size_ULang_Span_uft32__imut(tep4);
 ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(tep1,tep3,tep5);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___uptr(ULang_StringSpan_t_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:41*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32__imut_uptr(ULang_StringSpan_t_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:42*/

 void* tep0 = __this__->__0;
 int64_t tep1 = Index;
 int64_t tep2 = 4;
 int64_t tep3 = tep1*tep2;
 void* tep4 = (void*)((int64_t)tep0+tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___ULang_Range_t_uintptr_(ULang_StringSpan_t_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:44*/
 /*Line:42*/
 /*Line:44*/

 ULang_StringSpan_t_uft32_* tep0 = __this__;
 ULang_Span_uft32_ tep1 = ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__(tep0);
 ULang_Span_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft32_ tep4 = ULang_Span_uft32____Index___ULang_Span_uft32___ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft32_* tep5 = &tep4;
 ULang_StringSpan_t_uft32_ tep6 = ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32__imut_ULang_Range_t_uintptr_(ULang_StringSpan_t_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:45*/
 /*Line:44*/
 /*Line:45*/

 ULang_StringSpan_t_uft32_* tep0 = __this__;
 ULang_Span_uft32_ tep1 = ULang_StringSpan_t_uft32__AsSpan_ULang_StringSpan_t_uft32__imut(tep0);
 ULang_Span_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_Span_uft32_ tep4 = ULang_Span_uft32____Index___ULang_Span_uft32__imut_ULang_Range_t_uintptr_(tep2,tep3);
 ULang_Span_uft32_* tep5 = &tep4;
 ULang_StringSpan_t_uft32_ tep6 = ULang_StringSpan_t_uft32__ToStr_ULang_Span_uft32__(tep5);
 _ReturnVal = tep6;
 return _ReturnVal;

}
void ULang_String_t_uft32_____new___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{ /*Block*/ 


 return;

}
void ULang_String_t_uft32_____new___ULang_String_t_uft32___ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* span)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:58*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_StringSpan_t_uft32_* tep1 = span;
 int64_t tep2 = ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(tep1);
 ULang_String_t_uft32__Resize_ULang_String_t_uft32___uptr(tep0,tep2);

 /*Line:59*/

 int64_t tep3;


 int32_t tep4 = 0;
 tep3 = (int64_t)tep4;

 _label1:

 int64_t tep5 = tep3;
 ULang_StringSpan_t_uft32_* tep6 = span;
 int64_t tep7 = ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(tep6);
 int8_t tep8 = (int8_t)(tep5<tep7);
 int8_t tep9 = !tep8;
 if (tep9){goto _label0; };

 /*Line:60*/
 /*Line:59*/
 /*Line:60*/

 ULang_StringSpan_t_uft32_* tep10 = span;
 int64_t tep11 = tep3;
 void* tep12 = ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___uptr(tep10,tep11);
 int32_t tep13 = *(int32_t*)tep12;
 ULang_String_t_uft32_* tep14 = __this__;
 ULang_String_t_uft32_* tep15 = tep14;
 int64_t tep16 = tep3;
 void* tep17 = ULang_String_t_uft32____Index___ULang_String_t_uft32___uptr(tep15,tep16);
 int32_t tep18 = *(int32_t*)tep17;
 tep17 = (void*)tep13;

 /*Line:59*/

 int64_t tep19 = tep3;
 int64_t tep20 = 1;
 int64_t tep21 = tep19+tep20;
 tep3 = tep21;
 goto _label1;

 _label0:

 return;

}
void* ULang_String_t_uft32__Data_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:62*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft32__Data_ULang_Vector_uft32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_String_t_uft32__Data_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:63*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 void* tep1 = ULang_Vector_uft32__Data_ULang_Vector_uft32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft32__Size_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:66*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft32__Size_ULang_Vector_uft32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int64_t ULang_String_t_uft32__Capacity_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:67*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = ULang_Vector_uft32__Capacity_ULang_Vector_uft32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_String_t_uft32__Resize_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:69*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft32__Resize_ULang_Vector_uft32___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft32__Reserve_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:70*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Size;
 ULang_Vector_uft32__Reserve_ULang_Vector_uft32___uptr(tep0,tep1);
 return;

}
void ULang_String_t_uft32__Clear_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:71*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 ULang_Vector_uft32__Clear_ULang_Vector_uft32__(tep0);
 return;

}
int32_t ULang_String_t_uft32__Pop_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:73*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int32_t tep1 = ULang_Vector_uft32__Pop_ULang_Vector_uft32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
int32_t ULang_String_t_uft32__Remove_ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Index)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:74*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 int32_t tep2 = ULang_Vector_uft32__Remove_ULang_Vector_uft32___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft32__Push_ULang_String_t_uft32____imut(ULang_String_t_uft32_* __this__,void* Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:76*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 void* tep1 = Val;
 ULang_Vector_uft32__Push_ULang_Vector_uft32____imut(tep0,tep1);
 return;

}
void ULang_String_t_uft32__Insert_ULang_String_t_uft32___uptr__imut(ULang_String_t_uft32_* __this__,int64_t Index,void* Item)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = Item;
 ULang_Vector_uft32__Insert_ULang_Vector_uft32___uptr__imut(tep0,tep1,tep2);
 return;

}
void* ULang_String_t_uft32____Index___ULang_String_t_uft32___uptr(ULang_String_t_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:79*/

 
 /*Line:83*/

 ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft32____Index___ULang_Vector_uft32___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void* ULang_String_t_uft32____Index___ULang_String_t_uft32__imut_uptr(ULang_String_t_uft32_* __this__,int64_t Index)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:83*/

 
 /*Line:84*/

 ULang_Vector_uft32_* tep0 = &__this__->__0;
 int64_t tep1 = Index;
 void* tep2 = ULang_Vector_uft32____Index___ULang_Vector_uft32___uptr(tep0,tep1);
 _ReturnVal = tep2;
 return _ReturnVal;

}
void ULang_String_t_uft32__Append_ULang_String_t_uft32___ULang_Span_uft32_imut(ULang_String_t_uft32_* __this__,ULang_Span_uft32_ Val)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:86*/

  ULang_Vector_uft32_* tep0 = &__this__->__0;
 ULang_Span_uft32_ tep1 = Val;
 ULang_Vector_uft32__Append_ULang_Vector_uft32___ULang_Span_uft32_imut(tep0,tep1);
 return;

}
int8_t ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:90*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_String_t_uft32_* tep1 = tep0;
 ULang_StringSpan_t_uft32_ tep2 = ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__(tep1);
 ULang_StringSpan_t_uft32_* tep3 = &tep2;
 ULang_String_t_uft32_* tep4 = Other;
 ULang_StringSpan_t_uft32_ tep5 = ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__imut(tep4);
 ULang_StringSpan_t_uft32_* tep6 = &tep5;
 int8_t tep7 = ULang_StringSpan_t_uft32____equal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(tep3,tep6);
 _ReturnVal = tep7;
 return _ReturnVal;

}
int8_t ULang_String_t_uft32____notequal___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:92*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_String_t_uft32_* tep1 = tep0;
 ULang_String_t_uft32_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_String_t_uft32_ ULang_String_t_uft32____Plus___ULang_String_t_uft32__imut_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other)
{
 ULang_String_t_uft32_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
ULang_String_t_uft32_ ULang_String_t_uft32____Compound_Plus___ULang_String_t_uft32___ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_String_t_uft32_* Other)
{
 ULang_String_t_uft32_ _ReturnVal;
 /*Block*/ 


 return _ReturnVal;

}
int8_t ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:100*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_String_t_uft32_* tep1 = tep0;
 ULang_StringSpan_t_uft32_ tep2 = ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__(tep1);
 ULang_StringSpan_t_uft32_* tep3 = &tep2;
 ULang_StringSpan_t_uft32_* tep4 = Other;
 int8_t tep5 = ULang_StringSpan_t_uft32____equal___ULang_StringSpan_t_uft32__imut_ULang_StringSpan_t_uft32__imut(tep3,tep4);
 _ReturnVal = tep5;
 return _ReturnVal;

}
int8_t ULang_String_t_uft32____notequal___ULang_String_t_uft32__imut_ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_String_t_uft32_* tep1 = tep0;
 ULang_StringSpan_t_uft32_* tep2 = Other;
 int8_t tep3 = ULang_String_t_uft32____equal___ULang_String_t_uft32__imut_ULang_StringSpan_t_uft32__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft32____Compound_Plus___ULang_String_t_uft32___ULang_StringSpan_t_uft32__imut(ULang_String_t_uft32_* __this__,ULang_StringSpan_t_uft32_* Other)
{ /*Block*/ 


 return;

}
ULang_Span_uft32_ ULang_String_t_uft32__AsSpan_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 ULang_Span_uft32_____new___ULang_Span_uft32__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Span_uft32_ ULang_String_t_uft32__AsSpan_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__)
{
 ULang_Span_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Span_uft32_ tep0;
 ULang_Span_uft32_* tep1 = &tep0;
 ULang_Span_uft32_____new___ULang_Span_uft32__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft32_ tep0;
 ULang_StringSpan_t_uft32_* tep1 = &tep0;
 ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__imut(ULang_String_t_uft32_* __this__)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_StringSpan_t_uft32_ tep0;
 ULang_StringSpan_t_uft32_* tep1 = &tep0;
 ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(tep1);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_String_t_uft32____Index___ULang_String_t_uft32___ULang_Range_t_uintptr_(ULang_String_t_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:102*/
 /*Line:114*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_StringSpan_t_uft32_ tep1 = ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__(tep0);
 ULang_StringSpan_t_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft32_ tep4 = ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32___ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
ULang_StringSpan_t_uft32_ ULang_String_t_uft32____Index___ULang_String_t_uft32__imut_ULang_Range_t_uintptr_(ULang_String_t_uft32_* __this__,ULang_Range_t_uintptr_ Range)
{
 ULang_StringSpan_t_uft32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:114*/
 /*Line:115*/

 ULang_String_t_uft32_* tep0 = __this__;
 ULang_StringSpan_t_uft32_ tep1 = ULang_String_t_uft32__AsStrSpan_ULang_String_t_uft32__imut(tep0);
 ULang_StringSpan_t_uft32_* tep2 = &tep1;
 ULang_Range_t_uintptr_ tep3 = Range;
 ULang_StringSpan_t_uft32_ tep4 = ULang_StringSpan_t_uft32____Index___ULang_StringSpan_t_uft32__imut_ULang_Range_t_uintptr_(tep2,tep3);
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_String_t_uft32_____drop___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{ /*Block*/ 
 ULang_String_t_uft32_* tep0 = __this__;
 ULang_String_t_uft32_____de_init__(tep0);
 return;

}
void ULang_String_t_uft32_____de_init__(ULang_String_t_uft32_* __this__)
{ /*Block*/ 
 
 /*File:/Types/String.uc*/
 /*Line:0*/

 ULang_Vector_uft32_* tep0 = &__this__->__0;
 ULang_Vector_uft32_____drop___ULang_Vector_uft32__(tep0);
 return;

}
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:7*/

 float32_t tep0 = 0.000000;
  __this__->__0 = tep0;

 /*Line:8*/

 float32_t tep1 = 0.000000;
  __this__->__1 = tep1;
 return;

}
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(ULang_Vec2f_t_float32_* __this__,float32_t x,float32_t y)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:11*/

 float32_t tep0 = x;
  __this__->__0 = tep0;

 /*Line:12*/

 float32_t tep1 = y;
  __this__->__1 = tep1;
 return;

}
int8_t ULang_Vec2f_t_float32____equal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 


 ULang_Vec2f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:15*/

  float32_t tep1 = tep0->__1;
  float32_t tep2 = Other->__1;
 int8_t tep3 = (int8_t)(tep1==tep2);
 ULang_Vec2f_t_float32_* tep4 = __this__;
  float32_t tep5 = tep4->__0;
  float32_t tep6 = Other->__0;
 int8_t tep7 = (int8_t)(tep5==tep6);
 int8_t tep8 = tep3&&tep7;
 _ReturnVal = tep8;
 return _ReturnVal;

}
int8_t ULang_Vec2f_t_float32____notequal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:18*/

 ULang_Vec2f_t_float32_* tep0 = __this__;
 ULang_Vec2f_t_float32_* tep1 = tep0;
 ULang_Vec2f_t_float32_* tep2 = Other;
 int8_t tep3 = ULang_Vec2f_t_float32____equal___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_Vec2f_t_float32____Compound_Plus___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{ /*Block*/ 


 ULang_Vec2f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:21*/

  ULang_Vec2f_t_float32_* tep1 = __this__;
  float32_t tep2 = tep1->__0;
  float32_t tep3 = Other->__0;
 float32_t tep4 = tep2+tep3;
 tep0->__0 = tep4;
 ULang_Vec2f_t_float32_* tep5 = __this__;

 /*Line:22*/

  ULang_Vec2f_t_float32_* tep6 = __this__;
  float32_t tep7 = tep6->__1;
  float32_t tep8 = Other->__1;
 float32_t tep9 = tep7+tep8;
 tep5->__1 = tep9;
 return;

}
void ULang_Vec2f_t_float32____Compound_Sub___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{ /*Block*/ 


 ULang_Vec2f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:25*/

  ULang_Vec2f_t_float32_* tep1 = __this__;
  float32_t tep2 = tep1->__0;
  float32_t tep3 = Other->__0;
 float32_t tep4 = tep2-tep3;
 tep0->__0 = tep4;
 ULang_Vec2f_t_float32_* tep5 = __this__;

 /*Line:26*/

  ULang_Vec2f_t_float32_* tep6 = __this__;
  float32_t tep7 = tep6->__1;
  float32_t tep8 = Other->__1;
 float32_t tep9 = tep7-tep8;
 tep5->__1 = tep9;
 return;

}
ULang_Vec2f_t_float32_ ULang_Vec2f_t_float32____Plus___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{
 ULang_Vec2f_t_float32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:29*/

 ULang_Vec2f_t_float32_ tep0;
 ULang_Vec2f_t_float32_* tep1 = __this__;
 ULang_Vec2f_t_float32_ tep2 = *(ULang_Vec2f_t_float32_*)tep1;
 tep0 = tep2;

 /*Line:30*/

 ULang_Vec2f_t_float32_* tep3 = &tep0;
 ULang_Vec2f_t_float32_* tep4 = Other;
 ULang_Vec2f_t_float32____Compound_Plus___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(tep3,tep4);

 /*Line:31*/

 ULang_Vec2f_t_float32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
ULang_Vec2f_t_float32_ ULang_Vec2f_t_float32____Minus___ULang_Vec2f_t_float32__imut_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* __this__,ULang_Vec2f_t_float32_* Other)
{
 ULang_Vec2f_t_float32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:34*/

 ULang_Vec2f_t_float32_ tep0;
 ULang_Vec2f_t_float32_* tep1 = __this__;
 ULang_Vec2f_t_float32_ tep2 = *(ULang_Vec2f_t_float32_*)tep1;
 tep0 = tep2;

 /*Line:35*/

 ULang_Vec2f_t_float32_* tep3 = &tep0;
 ULang_Vec2f_t_float32_* tep4 = Other;
 ULang_Vec2f_t_float32____Compound_Sub___ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__imut(tep3,tep4);

 /*Line:36*/

 ULang_Vec2f_t_float32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32__(ULang_Vec2f_t_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:7*/

 int32_t tep0 = 0;
  __this__->__0 = tep0;

 /*Line:8*/

 int32_t tep1 = 0;
  __this__->__1 = tep1;
 return;

}
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32___i32_i32(ULang_Vec2f_t_sint32_* __this__,int32_t x,int32_t y)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:11*/

 int32_t tep0 = x;
  __this__->__0 = tep0;

 /*Line:12*/

 int32_t tep1 = y;
  __this__->__1 = tep1;
 return;

}
int8_t ULang_Vec2f_t_sint32____equal___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 


 ULang_Vec2f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:15*/

  int32_t tep1 = tep0->__1;
  int32_t tep2 = Other->__1;
 int8_t tep3 = (int8_t)(tep1==tep2);
 ULang_Vec2f_t_sint32_* tep4 = __this__;
  int32_t tep5 = tep4->__0;
  int32_t tep6 = Other->__0;
 int8_t tep7 = (int8_t)(tep5==tep6);
 int8_t tep8 = tep3&&tep7;
 _ReturnVal = tep8;
 return _ReturnVal;

}
int8_t ULang_Vec2f_t_sint32____notequal___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:18*/

 ULang_Vec2f_t_sint32_* tep0 = __this__;
 ULang_Vec2f_t_sint32_* tep1 = tep0;
 ULang_Vec2f_t_sint32_* tep2 = Other;
 int8_t tep3 = ULang_Vec2f_t_sint32____equal___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_Vec2f_t_sint32____Compound_Plus___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{ /*Block*/ 


 ULang_Vec2f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:21*/

  ULang_Vec2f_t_sint32_* tep1 = __this__;
  int32_t tep2 = tep1->__0;
  int32_t tep3 = Other->__0;
 int32_t tep4 = tep2+tep3;
 tep0->__0 = tep4;
 ULang_Vec2f_t_sint32_* tep5 = __this__;

 /*Line:22*/

  ULang_Vec2f_t_sint32_* tep6 = __this__;
  int32_t tep7 = tep6->__1;
  int32_t tep8 = Other->__1;
 int32_t tep9 = tep7+tep8;
 tep5->__1 = tep9;
 return;

}
void ULang_Vec2f_t_sint32____Compound_Sub___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{ /*Block*/ 


 ULang_Vec2f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec2.uc*/
 /*Line:25*/

  ULang_Vec2f_t_sint32_* tep1 = __this__;
  int32_t tep2 = tep1->__0;
  int32_t tep3 = Other->__0;
 int32_t tep4 = tep2-tep3;
 tep0->__0 = tep4;
 ULang_Vec2f_t_sint32_* tep5 = __this__;

 /*Line:26*/

  ULang_Vec2f_t_sint32_* tep6 = __this__;
  int32_t tep7 = tep6->__1;
  int32_t tep8 = Other->__1;
 int32_t tep9 = tep7-tep8;
 tep5->__1 = tep9;
 return;

}
ULang_Vec2f_t_sint32_ ULang_Vec2f_t_sint32____Plus___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{
 ULang_Vec2f_t_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:29*/

 ULang_Vec2f_t_sint32_ tep0;
 ULang_Vec2f_t_sint32_* tep1 = __this__;
 ULang_Vec2f_t_sint32_ tep2 = *(ULang_Vec2f_t_sint32_*)tep1;
 tep0 = tep2;

 /*Line:30*/

 ULang_Vec2f_t_sint32_* tep3 = &tep0;
 ULang_Vec2f_t_sint32_* tep4 = Other;
 ULang_Vec2f_t_sint32____Compound_Plus___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(tep3,tep4);

 /*Line:31*/

 ULang_Vec2f_t_sint32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
ULang_Vec2f_t_sint32_ ULang_Vec2f_t_sint32____Minus___ULang_Vec2f_t_sint32__imut_ULang_Vec2f_t_sint32__imut(ULang_Vec2f_t_sint32_* __this__,ULang_Vec2f_t_sint32_* Other)
{
 ULang_Vec2f_t_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:34*/

 ULang_Vec2f_t_sint32_ tep0;
 ULang_Vec2f_t_sint32_* tep1 = __this__;
 ULang_Vec2f_t_sint32_ tep2 = *(ULang_Vec2f_t_sint32_*)tep1;
 tep0 = tep2;

 /*Line:35*/

 ULang_Vec2f_t_sint32_* tep3 = &tep0;
 ULang_Vec2f_t_sint32_* tep4 = Other;
 ULang_Vec2f_t_sint32____Compound_Sub___ULang_Vec2f_t_sint32___ULang_Vec2f_t_sint32__imut(tep3,tep4);

 /*Line:36*/

 ULang_Vec2f_t_sint32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:8*/

 float32_t tep0 = 0.000000;
  __this__->__0 = tep0;

 /*Line:9*/

 float32_t tep1 = 0.000000;
  __this__->__1 = tep1;

 /*Line:10*/

 float32_t tep2 = 0.000000;
  __this__->__2 = tep2;
 return;

}
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32___f32_f32_f32(ULang_Vec3f_t_float32_* __this__,float32_t x,float32_t y,float32_t z)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:13*/

 float32_t tep0 = x;
  __this__->__0 = tep0;

 /*Line:14*/

 float32_t tep1 = y;
  __this__->__1 = tep1;

 /*Line:15*/

 float32_t tep2 = z;
  __this__->__2 = tep2;
 return;

}
int8_t ULang_Vec3f_t_float32____equal___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 


 ULang_Vec3f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:18*/

  float32_t tep1 = tep0->__2;
  float32_t tep2 = Other->__2;
 int8_t tep3 = (int8_t)(tep1==tep2);
 ULang_Vec3f_t_float32_* tep4 = __this__;
  float32_t tep5 = tep4->__1;
  float32_t tep6 = Other->__1;
 int8_t tep7 = (int8_t)(tep5==tep6);
 int8_t tep8 = tep3&&tep7;
 ULang_Vec3f_t_float32_* tep9 = __this__;
  float32_t tep10 = tep9->__0;
  float32_t tep11 = Other->__0;
 int8_t tep12 = (int8_t)(tep10==tep11);
 int8_t tep13 = tep8&&tep12;
 _ReturnVal = tep13;
 return _ReturnVal;

}
int8_t ULang_Vec3f_t_float32____notequal___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:21*/

 ULang_Vec3f_t_float32_* tep0 = __this__;
 ULang_Vec3f_t_float32_* tep1 = tep0;
 ULang_Vec3f_t_float32_* tep2 = Other;
 int8_t tep3 = ULang_Vec3f_t_float32____equal___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_Vec3f_t_float32____Compound_Plus___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{ /*Block*/ 


 ULang_Vec3f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:24*/

  ULang_Vec3f_t_float32_* tep1 = __this__;
  float32_t tep2 = tep1->__0;
  float32_t tep3 = Other->__0;
 float32_t tep4 = tep2+tep3;
 tep0->__0 = tep4;
 ULang_Vec3f_t_float32_* tep5 = __this__;

 /*Line:25*/

  ULang_Vec3f_t_float32_* tep6 = __this__;
  float32_t tep7 = tep6->__1;
  float32_t tep8 = Other->__1;
 float32_t tep9 = tep7+tep8;
 tep5->__1 = tep9;
 ULang_Vec3f_t_float32_* tep10 = __this__;

 /*Line:26*/

  ULang_Vec3f_t_float32_* tep11 = __this__;
  float32_t tep12 = tep11->__2;
  float32_t tep13 = Other->__2;
 float32_t tep14 = tep12+tep13;
 tep10->__2 = tep14;
 return;

}
void ULang_Vec3f_t_float32____Compound_Sub___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{ /*Block*/ 


 ULang_Vec3f_t_float32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:29*/

  ULang_Vec3f_t_float32_* tep1 = __this__;
  float32_t tep2 = tep1->__0;
  float32_t tep3 = Other->__0;
 float32_t tep4 = tep2-tep3;
 tep0->__0 = tep4;
 ULang_Vec3f_t_float32_* tep5 = __this__;

 /*Line:30*/

  ULang_Vec3f_t_float32_* tep6 = __this__;
  float32_t tep7 = tep6->__1;
  float32_t tep8 = Other->__1;
 float32_t tep9 = tep7-tep8;
 tep5->__1 = tep9;
 ULang_Vec3f_t_float32_* tep10 = __this__;

 /*Line:31*/

  ULang_Vec3f_t_float32_* tep11 = __this__;
  float32_t tep12 = tep11->__2;
  float32_t tep13 = Other->__2;
 float32_t tep14 = tep12-tep13;
 tep10->__2 = tep14;
 return;

}
ULang_Vec3f_t_float32_ ULang_Vec3f_t_float32____Plus___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{
 ULang_Vec3f_t_float32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:34*/

 ULang_Vec3f_t_float32_ tep0;
 ULang_Vec3f_t_float32_* tep1 = __this__;
 ULang_Vec3f_t_float32_ tep2 = *(ULang_Vec3f_t_float32_*)tep1;
 tep0 = tep2;

 /*Line:35*/

 ULang_Vec3f_t_float32_* tep3 = &tep0;
 ULang_Vec3f_t_float32_* tep4 = Other;
 ULang_Vec3f_t_float32____Compound_Plus___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(tep3,tep4);

 /*Line:36*/

 ULang_Vec3f_t_float32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
ULang_Vec3f_t_float32_ ULang_Vec3f_t_float32____Minus___ULang_Vec3f_t_float32__imut_ULang_Vec3f_t_float32__imut(ULang_Vec3f_t_float32_* __this__,ULang_Vec3f_t_float32_* Other)
{
 ULang_Vec3f_t_float32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:39*/

 ULang_Vec3f_t_float32_ tep0;
 ULang_Vec3f_t_float32_* tep1 = __this__;
 ULang_Vec3f_t_float32_ tep2 = *(ULang_Vec3f_t_float32_*)tep1;
 tep0 = tep2;

 /*Line:40*/

 ULang_Vec3f_t_float32_* tep3 = &tep0;
 ULang_Vec3f_t_float32_* tep4 = Other;
 ULang_Vec3f_t_float32____Compound_Sub___ULang_Vec3f_t_float32___ULang_Vec3f_t_float32__imut(tep3,tep4);

 /*Line:41*/

 ULang_Vec3f_t_float32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:8*/

 int32_t tep0 = 0;
  __this__->__0 = tep0;

 /*Line:9*/

 int32_t tep1 = 0;
  __this__->__1 = tep1;

 /*Line:10*/

 int32_t tep2 = 0;
  __this__->__2 = tep2;
 return;

}
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32___i32_i32_i32(ULang_Vec3f_t_sint32_* __this__,int32_t x,int32_t y,int32_t z)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:13*/

 int32_t tep0 = x;
  __this__->__0 = tep0;

 /*Line:14*/

 int32_t tep1 = y;
  __this__->__1 = tep1;

 /*Line:15*/

 int32_t tep2 = z;
  __this__->__2 = tep2;
 return;

}
int8_t ULang_Vec3f_t_sint32____equal___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 


 ULang_Vec3f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:18*/

  int32_t tep1 = tep0->__2;
  int32_t tep2 = Other->__2;
 int8_t tep3 = (int8_t)(tep1==tep2);
 ULang_Vec3f_t_sint32_* tep4 = __this__;
  int32_t tep5 = tep4->__1;
  int32_t tep6 = Other->__1;
 int8_t tep7 = (int8_t)(tep5==tep6);
 int8_t tep8 = tep3&&tep7;
 ULang_Vec3f_t_sint32_* tep9 = __this__;
  int32_t tep10 = tep9->__0;
  int32_t tep11 = Other->__0;
 int8_t tep12 = (int8_t)(tep10==tep11);
 int8_t tep13 = tep8&&tep12;
 _ReturnVal = tep13;
 return _ReturnVal;

}
int8_t ULang_Vec3f_t_sint32____notequal___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:21*/

 ULang_Vec3f_t_sint32_* tep0 = __this__;
 ULang_Vec3f_t_sint32_* tep1 = tep0;
 ULang_Vec3f_t_sint32_* tep2 = Other;
 int8_t tep3 = ULang_Vec3f_t_sint32____equal___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__(tep1,tep2);
 int8_t tep4 = !tep3;
 _ReturnVal = tep4;
 return _ReturnVal;

}
void ULang_Vec3f_t_sint32____Compound_Plus___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{ /*Block*/ 


 ULang_Vec3f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:24*/

  ULang_Vec3f_t_sint32_* tep1 = __this__;
  int32_t tep2 = tep1->__0;
  int32_t tep3 = Other->__0;
 int32_t tep4 = tep2+tep3;
 tep0->__0 = tep4;
 ULang_Vec3f_t_sint32_* tep5 = __this__;

 /*Line:25*/

  ULang_Vec3f_t_sint32_* tep6 = __this__;
  int32_t tep7 = tep6->__1;
  int32_t tep8 = Other->__1;
 int32_t tep9 = tep7+tep8;
 tep5->__1 = tep9;
 ULang_Vec3f_t_sint32_* tep10 = __this__;

 /*Line:26*/

  ULang_Vec3f_t_sint32_* tep11 = __this__;
  int32_t tep12 = tep11->__2;
  int32_t tep13 = Other->__2;
 int32_t tep14 = tep12+tep13;
 tep10->__2 = tep14;
 return;

}
void ULang_Vec3f_t_sint32____Compound_Sub___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{ /*Block*/ 


 ULang_Vec3f_t_sint32_* tep0 = __this__;

 /*File:/Types/Vec3.uc*/
 /*Line:29*/

  ULang_Vec3f_t_sint32_* tep1 = __this__;
  int32_t tep2 = tep1->__0;
  int32_t tep3 = Other->__0;
 int32_t tep4 = tep2-tep3;
 tep0->__0 = tep4;
 ULang_Vec3f_t_sint32_* tep5 = __this__;

 /*Line:30*/

  ULang_Vec3f_t_sint32_* tep6 = __this__;
  int32_t tep7 = tep6->__1;
  int32_t tep8 = Other->__1;
 int32_t tep9 = tep7-tep8;
 tep5->__1 = tep9;
 ULang_Vec3f_t_sint32_* tep10 = __this__;

 /*Line:31*/

  ULang_Vec3f_t_sint32_* tep11 = __this__;
  int32_t tep12 = tep11->__2;
  int32_t tep13 = Other->__2;
 int32_t tep14 = tep12-tep13;
 tep10->__2 = tep14;
 return;

}
ULang_Vec3f_t_sint32_ ULang_Vec3f_t_sint32____Plus___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{
 ULang_Vec3f_t_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:34*/

 ULang_Vec3f_t_sint32_ tep0;
 ULang_Vec3f_t_sint32_* tep1 = __this__;
 ULang_Vec3f_t_sint32_ tep2 = *(ULang_Vec3f_t_sint32_*)tep1;
 tep0 = tep2;

 /*Line:35*/

 ULang_Vec3f_t_sint32_* tep3 = &tep0;
 ULang_Vec3f_t_sint32_* tep4 = Other;
 ULang_Vec3f_t_sint32____Compound_Plus___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(tep3,tep4);

 /*Line:36*/

 ULang_Vec3f_t_sint32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
ULang_Vec3f_t_sint32_ ULang_Vec3f_t_sint32____Minus___ULang_Vec3f_t_sint32__imut_ULang_Vec3f_t_sint32__imut(ULang_Vec3f_t_sint32_* __this__,ULang_Vec3f_t_sint32_* Other)
{
 ULang_Vec3f_t_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:39*/

 ULang_Vec3f_t_sint32_ tep0;
 ULang_Vec3f_t_sint32_* tep1 = __this__;
 ULang_Vec3f_t_sint32_ tep2 = *(ULang_Vec3f_t_sint32_*)tep1;
 tep0 = tep2;

 /*Line:40*/

 ULang_Vec3f_t_sint32_* tep3 = &tep0;
 ULang_Vec3f_t_sint32_* tep4 = Other;
 ULang_Vec3f_t_sint32____Compound_Sub___ULang_Vec3f_t_sint32___ULang_Vec3f_t_sint32__imut(tep3,tep4);

 /*Line:41*/

 ULang_Vec3f_t_sint32_ tep5 = tep0;
 _ReturnVal = tep5;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty___ULang_Empty_(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* base)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:12*/

 ULang_Empty* tep0 = base;
  __this__->__0 = tep0;
 return;

}
ULang_Empty* ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__)
{
 ULang_Empty* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:14*/

 ULang_Empty* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Empty* ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__)
{
 ULang_Empty* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:15*/

 ULang_Empty* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Empty__Write_ULang_Ptr_ULang_Empty___ULang_Empty_imut(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:17*/

 ULang_Empty tep0 = *(ULang_Empty*)newvalue;
 ULang_Ptr_ULang_Empty_* tep1 = __this__;
 ULang_Empty* tep2 = ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__(tep1);
 *(ULang_Empty*)tep2 = tep0;
 return;

}
void ULang_Ptr_ULang_Empty__Write_ULang_Ptr_ULang_Empty___ULang_Emptymov(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:18*/

 ULang_Empty tep0 = newvalue;
 ULang_Ptr_ULang_Empty_* tep1 = __this__;
 ULang_Empty* tep2 = ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__(tep1);
 *(ULang_Empty*)tep2 = tep0;
 return;

}
ULang_Empty* ULang_Ptr_ULang_Empty__ReadPtr_ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__)
{
 ULang_Empty* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:20*/

 ULang_Ptr_ULang_Empty_* tep0 = __this__;
 ULang_Empty* tep1 = ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
ULang_Empty* ULang_Ptr_ULang_Empty____IndirectMember___ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__)
{
 ULang_Empty* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:22*/

 ULang_Ptr_ULang_Empty_* tep0 = __this__;
 ULang_Empty* tep1 = ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
ULang_Empty* ULang_Ptr_ULang_Empty____IndirectMember___ULang_Ptr_ULang_Empty__imut(ULang_Ptr_ULang_Empty_* __this__)
{
 ULang_Empty* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:24*/

 ULang_Ptr_ULang_Empty_* tep0 = __this__;
 ULang_Empty* tep1 = ULang_Ptr_ULang_Empty__Get_ULang_Ptr_ULang_Empty__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Empty__ReassignTo_ULang_Ptr_ULang_Empty___ULang_Empty_(ULang_Ptr_ULang_Empty_* __this__,ULang_Empty* newbase)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:26*/

 ULang_Empty* tep0 = newbase;
  __this__->__0 = tep0;
 return;

}
ULang_Ptr_ULang_Empty_ ULang_Ptr_ULang_Empty__Make_ULang_Empty_imut(ULang_Empty* V)
{
 ULang_Ptr_ULang_Empty_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_ULang_Empty_ tep0;
 ULang_Ptr_ULang_Empty_* tep1 = &tep0;

 /*Line:30*/

 ULang_Empty* tep2 = V;
 ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty___ULang_Empty_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Ptr_ULang_Empty_ ULang_Ptr_ULang_Empty__Make_ULang_Empty_(ULang_Empty* V)
{
 ULang_Ptr_ULang_Empty_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_ULang_Empty_ tep0;
 ULang_Ptr_ULang_Empty_* tep1 = &tep0;

 /*Line:31*/

 ULang_Empty* tep2 = V;
 ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty___ULang_Empty_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Empty_____new___ULang_Ptr_ULang_Empty__(ULang_Ptr_ULang_Empty_* __this__)
{ /*Block*/ 
 return;

}
void ULang_Ptr_sint32_____new___ULang_Ptr_sint32___i32_(ULang_Ptr_sint32_* __this__,void* base)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:12*/

 void* tep0 = base;
  __this__->__0 = tep0;
 return;

}
void* ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* ULang_Ptr_sint32__Get_ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:15*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32_imut(ULang_Ptr_sint32_* __this__,void* newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:17*/

 int32_t tep0 = *(int32_t*)newvalue;
 ULang_Ptr_sint32_* tep1 = __this__;
 void* tep2 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(tep1);
 *(int32_t*)tep2 = tep0;
 return;

}
void ULang_Ptr_sint32__Write_ULang_Ptr_sint32___i32mov(ULang_Ptr_sint32_* __this__,int32_t newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:18*/

 int32_t tep0 = newvalue;
 ULang_Ptr_sint32_* tep1 = __this__;
 void* tep2 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(tep1);
 *(int32_t*)tep2 = tep0;
 return;

}
void* ULang_Ptr_sint32__ReadPtr_ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:20*/

 ULang_Ptr_sint32_* tep0 = __this__;
 void* tep1 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_Ptr_sint32____IndirectMember___ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:22*/

 ULang_Ptr_sint32_* tep0 = __this__;
 void* tep1 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* ULang_Ptr_sint32____IndirectMember___ULang_Ptr_sint32__imut(ULang_Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:24*/

 ULang_Ptr_sint32_* tep0 = __this__;
 void* tep1 = ULang_Ptr_sint32__Get_ULang_Ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_Ptr_sint32__ReassignTo_ULang_Ptr_sint32___i32_(ULang_Ptr_sint32_* __this__,void* newbase)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:26*/

 void* tep0 = newbase;
  __this__->__0 = tep0;
 return;

}
ULang_Ptr_sint32_ ULang_Ptr_sint32__Make_i32_imut(void* V)
{
 ULang_Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_sint32_ tep0;
 ULang_Ptr_sint32_* tep1 = &tep0;

 /*Line:30*/

 void* tep2 = V;
 ULang_Ptr_sint32_____new___ULang_Ptr_sint32___i32_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Ptr_sint32_ ULang_Ptr_sint32__Make_i32_(void* V)
{
 ULang_Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_sint32_ tep0;
 ULang_Ptr_sint32_* tep1 = &tep0;

 /*Line:31*/

 void* tep2 = V;
 ULang_Ptr_sint32_____new___ULang_Ptr_sint32___i32_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_sint32_____new___ULang_Ptr_sint32__(ULang_Ptr_sint32_* __this__)
{ /*Block*/ 
 return;

}
ULang_Ptr_sint32_ ULang_ptr_sint32__i32_(void* V)
{
 ULang_Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:5*/

 void* tep0 = V;
 ULang_Ptr_sint32_ tep1 = ULang_Ptr_sint32__Make_i32_(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* base)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:12*/

 ULang_Vec2f_t_float32_* tep0 = base;
  __this__->__0 = tep0;
 return;

}
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{
 ULang_Vec2f_t_float32_* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:14*/

 ULang_Vec2f_t_float32_* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{
 ULang_Vec2f_t_float32_* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/Ptr.uc*/
 /*Line:15*/

 ULang_Vec2f_t_float32_* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Vec2f_t_float32___Write_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:17*/

 ULang_Vec2f_t_float32_ tep0 = *(ULang_Vec2f_t_float32_*)newvalue;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep1 = __this__;
 ULang_Vec2f_t_float32_* tep2 = ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___(tep1);
 *(ULang_Vec2f_t_float32_*)tep2 = tep0;
 return;

}
void ULang_Ptr_ULang_Vec2f_t_float32___Write_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32_mov(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_ newvalue)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:18*/

 ULang_Vec2f_t_float32_ tep0 = newvalue;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep1 = __this__;
 ULang_Vec2f_t_float32_* tep2 = ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___(tep1);
 *(ULang_Vec2f_t_float32_*)tep2 = tep0;
 return;

}
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32___ReadPtr_ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{
 ULang_Vec2f_t_float32_* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:20*/

 ULang_Ptr_ULang_Vec2f_t_float32__* tep0 = __this__;
 ULang_Vec2f_t_float32_* tep1 = ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{
 ULang_Vec2f_t_float32_* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:22*/

 ULang_Ptr_ULang_Vec2f_t_float32__* tep0 = __this__;
 ULang_Vec2f_t_float32_* tep1 = ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
ULang_Vec2f_t_float32_* ULang_Ptr_ULang_Vec2f_t_float32_____IndirectMember___ULang_Ptr_ULang_Vec2f_t_float32___imut(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{
 ULang_Vec2f_t_float32_* _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:24*/

 ULang_Ptr_ULang_Vec2f_t_float32__* tep0 = __this__;
 ULang_Vec2f_t_float32_* tep1 = ULang_Ptr_ULang_Vec2f_t_float32___Get_ULang_Ptr_ULang_Vec2f_t_float32___imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Vec2f_t_float32___ReassignTo_ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(ULang_Ptr_ULang_Vec2f_t_float32__* __this__,ULang_Vec2f_t_float32_* newbase)
{ /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:26*/

 ULang_Vec2f_t_float32_* tep0 = newbase;
  __this__->__0 = tep0;
 return;

}
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_Ptr_ULang_Vec2f_t_float32___Make_ULang_Vec2f_t_float32__imut(ULang_Vec2f_t_float32_* V)
{
 ULang_Ptr_ULang_Vec2f_t_float32__ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_ULang_Vec2f_t_float32__ tep0;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep1 = &tep0;

 /*Line:30*/

 ULang_Vec2f_t_float32_* tep2 = V;
 ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_Ptr_ULang_Vec2f_t_float32___Make_ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* V)
{
 ULang_Ptr_ULang_Vec2f_t_float32__ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:0*/

 ULang_Ptr_ULang_Vec2f_t_float32__ tep0;
 ULang_Ptr_ULang_Vec2f_t_float32__* tep1 = &tep0;

 /*Line:31*/

 ULang_Vec2f_t_float32_* tep2 = V;
 ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32____ULang_Vec2f_t_float32__(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void ULang_Ptr_ULang_Vec2f_t_float32______new___ULang_Ptr_ULang_Vec2f_t_float32___(ULang_Ptr_ULang_Vec2f_t_float32__* __this__)
{ /*Block*/ 
 return;

}
ULang_Ptr_ULang_Vec2f_t_float32__ ULang_ptr_ULang_Vec2f_t_float32___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* V)
{
 ULang_Ptr_ULang_Vec2f_t_float32__ _ReturnVal;
 /*Block*/ 

 /*File:/Types/Ptr.uc*/
 /*Line:5*/

 ULang_Vec2f_t_float32_* tep0 = V;
 ULang_Ptr_ULang_Vec2f_t_float32__ tep1 = ULang_Ptr_ULang_Vec2f_t_float32___Make_ULang_Vec2f_t_float32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void ULang_Native_OS_PrintChar_c(int8_t Value)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:7*/

 int8_t tep0 = Value;
 putchar(tep0);
 return;

}
void ULang_Native_OS_PrintSpan_c___imut_uptr(void* Buffer,int64_t Size)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:16*/

 int64_t tep0;


 int64_t tep1 = 0;
 tep0 = tep1;

 _label1:

 int64_t tep2 = tep0;
 int64_t tep3 = Size;
 int8_t tep4 = (int8_t)(tep3<tep2);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };

 /*Line:17*/

 void* tep6 = Buffer;
 int64_t tep7 = tep0;
 int64_t tep8 = 1;
 int64_t tep9 = tep7*tep8;
 void* tep10 = (void*)((int64_t)tep6+tep9);
 int8_t tep11 = *(int8_t*)tep10;
 ULang_Native_OS_PrintChar_c(tep11);

 /*Line:16*/

 int64_t tep12 = tep0;
 int64_t tep13 = 1;
 int64_t tep14 = tep12+tep13;
 tep0 = tep14;
 goto _label1;

 _label0:

 return;

}
void ULang_Native_OS_PrintClear()
{ /*Block*/ 
 return;

}
void ULang_Native_OS_Malloc_uptr(int64_t Size)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:27*/

 int64_t tep0 = Size;
 void* tep1 = malloc(tep0);
 return;

}
void ULang_Native_OS_Free_u8___(void* Block)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:34*/

 void* tep0 = Block;
 free(tep0);
 return;

}
