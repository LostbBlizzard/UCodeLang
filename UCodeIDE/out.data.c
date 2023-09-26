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

typedef struct 
{
 int64_t __0; 
 int64_t __1; 
 int64_t __2; 
 int64_t __3; 

} Vec4_t_sint64_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
Vec4_t_sint64_ CopyV_Vec4_t_sint64_imut(Vec4_t_sint64_ Val);
Vec4_t_sint64_ main2_Vec4_t_sint64_(Vec4_t_sint64_ Val);
int64_t Get_X_Vec4_t_sint64_(Vec4_t_sint64_ Val);
int64_t Get_Y_Vec4_t_sint64_(Vec4_t_sint64_ Val);
int64_t Get_Z_Vec4_t_sint64_(Vec4_t_sint64_ Val);
int64_t Get_W_Vec4_t_sint64_(Vec4_t_sint64_ Val);
void Vec4_t_sint64_____init__(Vec4_t_sint64_* __this__);
void Vec4_t_sint64_____new___Vec4_t_sint64__(Vec4_t_sint64_* __this__);


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
Vec4_t_sint64_ CopyV_Vec4_t_sint64_imut(Vec4_t_sint64_ Val)
{
 Vec4_t_sint64_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:131*/

 Vec4_t_sint64_ A = Val;
 _ReturnVal = A;
 return _ReturnVal;

}
Vec4_t_sint64_ main2_Vec4_t_sint64_(Vec4_t_sint64_ Val)
{
 Vec4_t_sint64_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:134*/

 Vec4_t_sint64_ A = Val;
 Vec4_t_sint64_ B = CopyV_Vec4_t_sint64_imut(A);
 _ReturnVal = B;
 return _ReturnVal;

}
int64_t Get_X_Vec4_t_sint64_(Vec4_t_sint64_ Val)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:137*/

  int64_t A = Val.__0;
 _ReturnVal = A;
 return _ReturnVal;

}
int64_t Get_Y_Vec4_t_sint64_(Vec4_t_sint64_ Val)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:140*/

  int64_t A = Val.__1;
 _ReturnVal = A;
 return _ReturnVal;

}
int64_t Get_Z_Vec4_t_sint64_(Vec4_t_sint64_ Val)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:143*/

  int64_t A = Val.__2;
 _ReturnVal = A;
 return _ReturnVal;

}
int64_t Get_W_Vec4_t_sint64_(Vec4_t_sint64_ Val)
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:146*/

  int64_t A = Val.__3;
 _ReturnVal = A;
 return _ReturnVal;

}
void Vec4_t_sint64_____init__(Vec4_t_sint64_* __this__)
{ /*Block*/ 
 int64_t A = 1;
 
 /*Line:124*/

 __this__->__0 = A;
 int64_t B = 2;
 
 /*Line:125*/

 __this__->__1 = B;
 int64_t C = 3;
 
 /*Line:126*/

 __this__->__2 = C;
 int64_t D = 4;
  __this__->__3 = D;
 return;

}
void Vec4_t_sint64_____new___Vec4_t_sint64__(Vec4_t_sint64_* __this__)
{ /*Block*/ 
 Vec4_t_sint64_* A = __this__;
 Vec4_t_sint64_____init__(A);
 return;

}
