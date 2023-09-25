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
 void* __0; 

} Optional_sint32___Option_;

typedef struct 
{
 void* __0; 
 int64_t __1; 

} Span_sint32_;

typedef struct 
{
 int64_t __0; 
 int64_t __1; 

} Range_t_uintptr_;

typedef int8_t(*Span_sint32__FindFunc)(void*);

typedef union 
{
 Optional_sint32___Option_ __0; 

} Optional_sint32__u;

typedef struct 
{
 int8_t __0; 
 Optional_sint32__u __1; 

} Optional_sint32__;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int32_t _Entry();
void Span_sint32_____new___Span_sint32__(Span_sint32_* __this__);
void Span_sint32_____new___Span_sint32___i32____uptr(Span_sint32_* __this__,void* data,int64_t size);
Optional_sint32__ Span_sint32__Find_Span_sint32___Span_sint32__FindFunc(Span_sint32_* __this__,Span_sint32__FindFunc Func);
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__);
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end);
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
int32_t _Entry()
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:93*/

 Optional_sint32__ A;
 int8_t B = 1;
 Optional_sint32__ C;
  C.__0 = B;
 A = C;

 /*Line:96*/

 Optional_sint32__ D = A;
 int8_t E = 1;
 Optional_sint32__ F;
  F.__0 = E;
   int8_t G = D.__0==F.__0;
 int8_t H = !G;
 if (H){goto _label0; };

 /*Line:97*/

 int32_t I = 0;
 _ReturnVal = I;
 goto _label1;
 goto _label2;
 _label0:

 /*Line:98*/
 /*Line:96*/

 Optional_sint32__* J = &A;
 int8_t K = 0;
   int8_t L = J->__0==K;
   int8_t M = !L;
 if (M){goto _label3; };

 /*Line:98*/

 int32_t N = *(int32_t*)J->__1.__0.__0;
 _ReturnVal = N;
 goto _label1;
 goto _label2;
 _label3:
 UCIR_Unreachable;
 _label1:
 goto _label2;
 _label2:
 return _ReturnVal;

}
void Span_sint32_____new___Span_sint32__(Span_sint32_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:20*/

 int64_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:21*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void Span_sint32_____new___Span_sint32___i32____uptr(Span_sint32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:24*/

 void* A = data;
  __this__->__0 = A;

 /*Line:25*/

 int64_t B = size;
  __this__->__1 = B;
 return;

}
Optional_sint32__ Span_sint32__Find_Span_sint32___Span_sint32__FindFunc(Span_sint32_* __this__,Span_sint32__FindFunc Func)
{
 Optional_sint32__ _ReturnVal;
 /*Block*/ 


 int8_t A = 1;
 Optional_sint32__ B;
  B.__0 = A;
 _ReturnVal = B;
 return _ReturnVal;

}
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:6*/
 /*Line:0*/

 int64_t A = 0;
  __this__->__0 = A;

 /*Line:7*/
 /*Line:0*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:9*/

 int64_t A = start;
  __this__->__0 = A;

 /*Line:10*/

 int64_t B = end;
  __this__->__1 = B;
 return;

}
int main(int argc, char** argv)
{
__StaticInit__();
__threadInit__();

int exitcode = _Entry();

__threadUnload__();
__StaticUnload__();

 return exitcode;
}
