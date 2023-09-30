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

static int8_t _Const_SpanString_Hello_World[] = {72,101,108,108,111,32,87,111,114,108,100};
typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_char_;

typedef struct 
{

} ULang_Compiler;

typedef struct 
{

} ULang_LowLevel;

typedef struct 
{

} ULang_Math;

typedef struct 
{

} ULang_Fmt;

typedef struct 
{

} ULang_OpenedFile;

typedef struct 
{

} ULang_OpenFileError;

typedef struct 
{

} ULang_File;

typedef struct 
{

} ULang_Writer;

typedef struct 
{

} ULang_Path;

typedef struct 
{

} ULang_String_t_char_;

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
 ULang_String_t_char_ __0; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError__Value_;

typedef struct 
{
 ULang_OpenFileError __0; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError__Error_;

typedef struct 
{

} ULang_Vector_uint8_;

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

} ULang_String_t_uft8_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_uft8_;

typedef struct 
{

} ULang_String_t_uft16_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} ULang_StringSpan_t_uft16_;

typedef struct 
{

} ULang_String_t_uft32_;

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

typedef union 
{
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError_u;

typedef union 
{
 ULang_Result_ULang_String_t_char__ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_String_t_char__ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError_u;

typedef union 
{
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Value_ __0; 
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError__Error_ __1; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_u;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_OpenedFile_ULang_OpenFileError_u __1; 

} ULang_Result_ULang_OpenedFile_ULang_OpenFileError_;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_String_t_char__ULang_OpenFileError_u __1; 

} ULang_Result_ULang_String_t_char__ULang_OpenFileError_;

typedef struct 
{
 int8_t __0; 
 ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_u __1; 

} ULang_Result_ULang_Vector_uint8__ULang_OpenFileError_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
void _Entry();
int8_t ULang_Compiler_Is64Bit();
int8_t ULang_Compiler_Is32Bit();
int8_t ULang_Compiler_IsBackendC89();
int8_t ULang_Compiler_IsBackendUCodeVM();
int8_t ULang_Compiler_IsBackendWasm();
int8_t ULang_Compiler_IsBackendVM();
void ULang_Compiler____new___ULang_Compiler_(ULang_Compiler* __this__);
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
void ULang_Path____new___ULang_Path_(ULang_Path* __this__);
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__);
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(ULang_StringSpan_t_char_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__);
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__);
void ULang_String_t_char_____new___ULang_String_t_char__(ULang_String_t_char_* __this__);
void ULang_Vector_uint8_____new___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__);
int32_t ULang_Math_Max_sint32__i32_i32(int32_t A,int32_t B);
int32_t ULang_Math_Min_sint32__i32_i32(int32_t A,int32_t B);
void ULang_String_t_uft8_____new___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__);
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__);
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(ULang_StringSpan_t_uft8_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__);
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__);
void ULang_String_t_uft16_____new___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__);
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__);
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(ULang_StringSpan_t_uft16_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__);
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__);
void ULang_String_t_uft32_____new___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__);
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__);
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(ULang_StringSpan_t_uft32_* __this__,void* data,int64_t size);
int64_t ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__);
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__);
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* __this__);
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(ULang_Vec2f_t_float32_* __this__,float32_t x,float32_t y);
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32__(ULang_Vec2f_t_sint32_* __this__);
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32___i32_i32(ULang_Vec2f_t_sint32_* __this__,int32_t x,int32_t y);
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__);
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32___f32_f32_f32(ULang_Vec3f_t_float32_* __this__,float32_t x,float32_t y,float32_t z);
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__);
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32___i32_i32_i32(ULang_Vec3f_t_sint32_* __this__,int32_t x,int32_t y,int32_t z);
void putchar(int8_t Val);
void ULang_Native_OS_PrintChar_c(int8_t Value);
void ULang_Native_OS_PrintSpan_c___imut_uptr(void* Buffer,int64_t Size);
void ULang_Native_OS_PrintClear();
void ULang_Native_OS_Malloc_uptr(int64_t Size);
void ULang_Native_OS_Free_u8___(void* Block);
int main(int argc, char** argv);

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
void _Entry()
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:6*/

 void* Atepvir = &_Const_SpanString_Hello_World;
 int64_t Atepvistepvir = 11;
 ULang_StringSpan_t_char_ Atepvistepvistepvir;
 ULang_StringSpan_t_char_* Atepvistepvistepvistepvir = &Atepvistepvistepvir;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(Atepvistepvistepvistepvir,Atepvir,Atepvistepvir);
 ULang_Fmt_Println_ULang_StringSpan_t_char_imut(Atepvistepvistepvir);
 return;

}
int8_t ULang_Compiler_Is64Bit()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:3*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
int8_t ULang_Compiler_Is32Bit()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:4*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendC89()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:6*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendUCodeVM()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:7*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendWasm()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:8*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
int8_t ULang_Compiler_IsBackendVM()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Compiler.uc*/
 /*Line:10*/

 int8_t Atepvir = 1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void ULang_Compiler____new___ULang_Compiler_(ULang_Compiler* __this__)
{ /*Block*/ 
 return;

}
void ULang_LowLevel_Malloc_uptr(int64_t Size)
{ /*Block*/ 

 /*File:/Helper/LowLevel.uc*/
 /*Line:3*/

 int64_t Atepvir = Size;
 ULang_Native_OS_Malloc_uptr(Atepvir);
 return;

}
void ULang_LowLevel_Free_u8___(void* Block)
{ /*Block*/ 

 /*File:/Helper/LowLevel.uc*/
 /*Line:4*/

 void* Atepvir = Block;
 ULang_Native_OS_Free_u8___(Atepvir);
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

 int8_t Atepvir = Str;
 ULang_Native_OS_PrintChar_c(Atepvir);
 return;

}
void ULang_Fmt_Print_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:9*/

 ULang_StringSpan_t_char_* Atepvir = &Str;
 void* Atepvistepvir = ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(Atepvir);
 ULang_StringSpan_t_char_* Atepvistepvistepvir = &Str;
 int64_t Atepvistepvistepvistepvir = ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(Atepvistepvistepvir);
 ULang_Native_OS_PrintSpan_c___imut_uptr(Atepvistepvir,Atepvistepvistepvistepvir);
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

 int8_t Atepvir = Str;
 ULang_Fmt_Print_c(Atepvir);

 /*Line:15*/

 int8_t Atepvistepvir = 10;
 ULang_Fmt_Print_c(Atepvistepvir);
 return;

}
void ULang_Fmt_Println_ULang_StringSpan_t_char_imut(ULang_StringSpan_t_char_ Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:18*/

 ULang_StringSpan_t_char_ Atepvir = Str;
 ULang_Fmt_Print_ULang_StringSpan_t_char_imut(Atepvir);

 /*Line:19*/

 int8_t Atepvistepvir = 10;
 ULang_Fmt_Print_c(Atepvistepvir);
 return;

}
void ULang_Fmt_Println_ULang_String_t_char__imut(ULang_String_t_char_* Str)
{ /*Block*/ 

 /*File:/IO/Console/Fmt.uc*/
 /*Line:22*/

 ULang_String_t_char_* Atepvir = Str;
 ULang_Fmt_Print_ULang_String_t_char__imut(Atepvir);

 /*Line:23*/

 int8_t Atepvistepvir = 10;
 ULang_Fmt_Print_c(Atepvistepvir);
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

 void* Atepvir = &_Const_SpanString_Hello_World;
 int64_t Atepvistepvir = 11;
 ULang_StringSpan_t_char_ Atepvistepvistepvir;
 ULang_StringSpan_t_char_* Atepvistepvistepvistepvir = &Atepvistepvistepvir;
 ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(Atepvistepvistepvistepvir,Atepvir,Atepvistepvir);
 ULang_Fmt_Println_ULang_StringSpan_t_char_imut(Atepvistepvistepvir);

 /*Line:8*/

 int8_t Atepvistepvistepvistepvistepvir = 1;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int8_t ULang_Tests_Print2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/IOTest.uc*/
 /*Line:12*/

 int8_t Atepvir = 72;
 ULang_Fmt_Print_c(Atepvir);

 /*Line:13*/

 int8_t Atepvistepvir = 105;
 ULang_Fmt_Println_c(Atepvistepvir);

 /*Line:14*/

 int8_t Atepvistepvistepvir = 1;
 _ReturnVal = Atepvistepvistepvir;
 return _ReturnVal;

}
int8_t ULang_Tests_Max_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:7*/

 int32_t Atepvir = 5;
 int32_t Atepvistepvir = 5;
 int32_t Atepvistepvistepvir = 2;
 int32_t Atepvistepvistepvistepvir = ULang_Math_Max_sint32__i32_i32(Atepvistepvir,Atepvistepvistepvir);
 int8_t Atepvistepvistepvistepvistepvir = Atepvistepvistepvistepvir==Atepvir;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int8_t ULang_Tests_Max_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:11*/

 int32_t Atepvir = 5;
 int32_t Atepvistepvir = 2;
 int32_t Atepvistepvistepvir = 5;
 int32_t Atepvistepvistepvistepvir = ULang_Math_Max_sint32__i32_i32(Atepvistepvir,Atepvistepvistepvir);
 int8_t Atepvistepvistepvistepvistepvir = Atepvistepvistepvistepvir==Atepvir;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int8_t ULang_Tests_Min_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:15*/

 int32_t Atepvir = 2;
 int32_t Atepvistepvir = 5;
 int32_t Atepvistepvistepvir = 2;
 int32_t Atepvistepvistepvistepvir = ULang_Math_Min_sint32__i32_i32(Atepvistepvir,Atepvistepvistepvir);
 int8_t Atepvistepvistepvistepvistepvir = Atepvistepvistepvistepvir==Atepvir;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int8_t ULang_Tests_Min_2()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Tests/Math.uc*/
 /*Line:19*/

 int32_t Atepvir = 2;
 int32_t Atepvistepvir = 2;
 int32_t Atepvistepvistepvir = 5;
 int32_t Atepvistepvistepvistepvir = ULang_Math_Min_sint32__i32_i32(Atepvistepvir,Atepvistepvistepvir);
 int8_t Atepvistepvistepvistepvistepvir = Atepvistepvistepvistepvir==Atepvir;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
void ULang_Path____new___ULang_Path_(ULang_Path* __this__)
{ /*Block*/ 
 return;

}
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char__(ULang_StringSpan_t_char_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:8*/

 int64_t Atepvir = 0;
  __this__->__0 = (void*)Atepvir;

 /*Line:9*/

 int64_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_StringSpan_t_char_____new___ULang_StringSpan_t_char___c____uptr(ULang_StringSpan_t_char_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 void* Atepvir = data;
  __this__->__0 = Atepvir;

 /*Line:13*/

 int64_t Atepvistepvir = size;
  __this__->__1 = Atepvistepvir;
 return;

}
int64_t ULang_StringSpan_t_char__Size_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:15*/

 int64_t Atepvir = __this__->__1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void* ULang_StringSpan_t_char__Data_ULang_StringSpan_t_char__imut(ULang_StringSpan_t_char_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:16*/

 void* Atepvir = __this__->__0;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void ULang_String_t_char_____new___ULang_String_t_char__(ULang_String_t_char_* __this__)
{ /*Block*/ 
 return;

}
void ULang_Vector_uint8_____new___ULang_Vector_uint8__(ULang_Vector_uint8_* __this__)
{ /*Block*/ 
 return;

}
int32_t ULang_Math_Max_sint32__i32_i32(int32_t A,int32_t B)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Math.uc*/
 /*Line:6*/

 int32_t Atepvir = B;
 int32_t Atepvistepvir = A;
 int8_t Atepvistepvistepvir = Atepvistepvir>Atepvir;
 int8_t Atepvistepvistepvistepvir = !Atepvistepvistepvir;
 if (Atepvistepvistepvistepvir){goto _label0; };
 int32_t Atepvistepvistepvistepvistepvir = A;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 goto _label1;
 _label0:

 /*Line:7*/

 int32_t Atepvistepvistepvistepvistepvistepvir = B;
 _ReturnVal = Atepvistepvistepvistepvistepvistepvir;
 _label1:
 return _ReturnVal;

}
int32_t ULang_Math_Min_sint32__i32_i32(int32_t A,int32_t B)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/Helper/Math.uc*/
 /*Line:10*/

 int32_t Atepvir = B;
 int32_t Atepvistepvir = A;
 int8_t Atepvistepvistepvir = Atepvistepvir<Atepvir;
 int8_t Atepvistepvistepvistepvir = !Atepvistepvistepvir;
 if (Atepvistepvistepvistepvir){goto _label0; };
 int32_t Atepvistepvistepvistepvistepvir = A;
 _ReturnVal = Atepvistepvistepvistepvistepvir;
 goto _label1;
 _label0:

 /*Line:11*/

 int32_t Atepvistepvistepvistepvistepvistepvir = B;
 _ReturnVal = Atepvistepvistepvistepvistepvistepvir;
 _label1:
 return _ReturnVal;

}
void ULang_String_t_uft8_____new___ULang_String_t_uft8__(ULang_String_t_uft8_* __this__)
{ /*Block*/ 
 return;

}
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8__(ULang_StringSpan_t_uft8_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:8*/

 int64_t Atepvir = 0;
  __this__->__0 = (void*)Atepvir;

 /*Line:9*/

 int64_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_StringSpan_t_uft8_____new___ULang_StringSpan_t_uft8_______uptr(ULang_StringSpan_t_uft8_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 void* Atepvir = data;
  __this__->__0 = Atepvir;

 /*Line:13*/

 int64_t Atepvistepvir = size;
  __this__->__1 = Atepvistepvir;
 return;

}
int64_t ULang_StringSpan_t_uft8__Size_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:15*/

 int64_t Atepvir = __this__->__1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft8__Data_ULang_StringSpan_t_uft8__imut(ULang_StringSpan_t_uft8_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:16*/

 void* Atepvir = __this__->__0;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void ULang_String_t_uft16_____new___ULang_String_t_uft16__(ULang_String_t_uft16_* __this__)
{ /*Block*/ 
 return;

}
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16__(ULang_StringSpan_t_uft16_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:8*/

 int64_t Atepvir = 0;
  __this__->__0 = (void*)Atepvir;

 /*Line:9*/

 int64_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_StringSpan_t_uft16_____new___ULang_StringSpan_t_uft16_______uptr(ULang_StringSpan_t_uft16_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 void* Atepvir = data;
  __this__->__0 = Atepvir;

 /*Line:13*/

 int64_t Atepvistepvir = size;
  __this__->__1 = Atepvistepvir;
 return;

}
int64_t ULang_StringSpan_t_uft16__Size_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:15*/

 int64_t Atepvir = __this__->__1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft16__Data_ULang_StringSpan_t_uft16__imut(ULang_StringSpan_t_uft16_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:16*/

 void* Atepvir = __this__->__0;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void ULang_String_t_uft32_____new___ULang_String_t_uft32__(ULang_String_t_uft32_* __this__)
{ /*Block*/ 
 return;

}
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32__(ULang_StringSpan_t_uft32_* __this__)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:8*/

 int64_t Atepvir = 0;
  __this__->__0 = (void*)Atepvir;

 /*Line:9*/

 int64_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_StringSpan_t_uft32_____new___ULang_StringSpan_t_uft32_______uptr(ULang_StringSpan_t_uft32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/Types/String.uc*/
 /*Line:12*/

 void* Atepvir = data;
  __this__->__0 = Atepvir;

 /*Line:13*/

 int64_t Atepvistepvir = size;
  __this__->__1 = Atepvistepvir;
 return;

}
int64_t ULang_StringSpan_t_uft32__Size_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__)
{
 int64_t _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:15*/

 int64_t Atepvir = __this__->__1;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void* ULang_StringSpan_t_uft32__Data_ULang_StringSpan_t_uft32__imut(ULang_StringSpan_t_uft32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/Types/String.uc*/
 /*Line:16*/

 void* Atepvir = __this__->__0;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32__(ULang_Vec2f_t_float32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:7*/

 float32_t Atepvir = 0.000000;
  __this__->__0 = Atepvir;

 /*Line:8*/

 float32_t Atepvistepvir = 0.000000;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_Vec2f_t_float32_____new___ULang_Vec2f_t_float32___f32_f32(ULang_Vec2f_t_float32_* __this__,float32_t x,float32_t y)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:11*/

 float32_t Atepvir = x;
  __this__->__0 = Atepvir;

 /*Line:12*/

 float32_t Atepvistepvir = y;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32__(ULang_Vec2f_t_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:7*/

 int32_t Atepvir = 0;
  __this__->__0 = Atepvir;

 /*Line:8*/

 int32_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_Vec2f_t_sint32_____new___ULang_Vec2f_t_sint32___i32_i32(ULang_Vec2f_t_sint32_* __this__,int32_t x,int32_t y)
{ /*Block*/ 

 /*File:/Types/Vec2.uc*/
 /*Line:11*/

 int32_t Atepvir = x;
  __this__->__0 = Atepvir;

 /*Line:12*/

 int32_t Atepvistepvir = y;
  __this__->__1 = Atepvistepvir;
 return;

}
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32__(ULang_Vec3f_t_float32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:8*/

 float32_t Atepvir = 0.000000;
  __this__->__0 = Atepvir;

 /*Line:9*/

 float32_t Atepvistepvir = 0.000000;
  __this__->__1 = Atepvistepvir;

 /*Line:10*/

 float32_t Atepvistepvistepvir = 0.000000;
  __this__->__2 = Atepvistepvistepvir;
 return;

}
void ULang_Vec3f_t_float32_____new___ULang_Vec3f_t_float32___f32_f32_f32(ULang_Vec3f_t_float32_* __this__,float32_t x,float32_t y,float32_t z)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:13*/

 float32_t Atepvir = x;
  __this__->__0 = Atepvir;

 /*Line:14*/

 float32_t Atepvistepvir = y;
  __this__->__1 = Atepvistepvir;

 /*Line:15*/

 float32_t Atepvistepvistepvir = z;
  __this__->__2 = Atepvistepvistepvir;
 return;

}
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32__(ULang_Vec3f_t_sint32_* __this__)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:8*/

 int32_t Atepvir = 0;
  __this__->__0 = Atepvir;

 /*Line:9*/

 int32_t Atepvistepvir = 0;
  __this__->__1 = Atepvistepvir;

 /*Line:10*/

 int32_t Atepvistepvistepvir = 0;
  __this__->__2 = Atepvistepvistepvir;
 return;

}
void ULang_Vec3f_t_sint32_____new___ULang_Vec3f_t_sint32___i32_i32_i32(ULang_Vec3f_t_sint32_* __this__,int32_t x,int32_t y,int32_t z)
{ /*Block*/ 

 /*File:/Types/Vec3.uc*/
 /*Line:13*/

 int32_t Atepvir = x;
  __this__->__0 = Atepvir;

 /*Line:14*/

 int32_t Atepvistepvir = y;
  __this__->__1 = Atepvistepvir;

 /*Line:15*/

 int32_t Atepvistepvistepvir = z;
  __this__->__2 = Atepvistepvistepvir;
 return;

}
void ULang_Native_OS_PrintChar_c(int8_t Value)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:7*/

 int8_t Atepvir = Value;
 putchar(Atepvir);
 return;

}
void ULang_Native_OS_PrintSpan_c___imut_uptr(void* Buffer,int64_t Size)
{ /*Block*/ 


 int64_t Atepvir;
 int64_t Atepvistepvir = 0;
 Atepvir = Atepvistepvir;
 _label1:
 int64_t Atepvistepvistepvir = Size;
 int64_t Atepvistepvistepvistepvir = Atepvir;
 int8_t Atepvistepvistepvistepvistepvir = Atepvistepvistepvistepvir<Atepvistepvistepvir;
 int8_t Atepvistepvistepvistepvistepvistepvir = !Atepvistepvistepvistepvistepvir;
 if (Atepvistepvistepvistepvistepvistepvir){goto _label0; };

 /*Line:17*/

 void* Atepvistepvistepvistepvistepvistepvistepvir = Buffer;
 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvir = Atepvir;
 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvir = 1;
 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = Atepvistepvistepvistepvistepvistepvistepvistepvir*Atepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 void* Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = (int64_t)Atepvistepvistepvistepvistepvistepvistepvir+Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 int8_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = *(int8_t*)Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 ULang_Native_OS_PrintChar_c(Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir);

 /*Line:16*/

 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = Atepvir;
 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = 1;
 int64_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir+Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 Atepvir = Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir;
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

 int64_t Atepvir = Size;
 void* Atepvistepvir = malloc(Atepvir);
 return;

}
void ULang_Native_OS_Free_u8___(void* Block)
{ /*Block*/ 

 /*File:/OS_Calls.uc*/
 /*Line:34*/

 void* Atepvir = Block;
 free(Atepvir);
 return;

}
int main(int argc, char** argv)
{
__StaticInit__();
__threadInit__();

_Entry();

__threadUnload__();
__StaticUnload__();

 return 0;
}
