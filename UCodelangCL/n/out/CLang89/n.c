//Made using UCodeLang C89 Backend.
//
//
//defs

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
//includes

#include <inttypes.h>
//Types
typedef float float32_t;
typedef double float64_t;



//file.h

ThreadLocalKeyWord int32_t temv;
typedef struct 
{
 int64_t __0; 
 int64_t __1; 
 int64_t __2; 

} Vec_sint32_;

typedef struct 
{
 int64_t __0; 
 int64_t __1; 
 int64_t __2; 

} Vec_char_;


void _StaticInit();
void _Static_deInit();
void _threadInit();
void _thread_deInit();
void _Entry();
int8_t func();
int8_t func2();
void printf1(int8_t V);
int32_t printf(void* v);
int32_t putchar(int32_t v);
void free(void* ptr);
void* malloc(int64_t size);
void Vec_sint32_____init__(Vec_sint32_* __this__);
void Vec_sint32_____new______(Vec_sint32_* __this__);
void Vec_char_____init__(Vec_char_* __this__);
void Vec_char_____new______(Vec_char_* __this__);
int main(int argc, char** argv);

//file.cpp

void _StaticInit()
{
}
void _Static_deInit()
{
}
void _threadInit()
{ //Block 
 int32_t A = 7;
 temv = A;

}
void _thread_deInit()
{
}
void _Entry()
{ //Block 
 int8_t A = 104;
 printf1(A);
 int8_t B = 101;
 printf1(B);
 int8_t C = 108;
 printf1(C);
 int8_t D = 108;
 printf1(D);
 int8_t E = 111;
 printf1(E);
 int32_t F = temv;
 int32_t G = 5;
 int32_t H = F+G;
 temv = H;
 void* I;
 int64_t J = 4;
 void* K = malloc(J);
 int32_t L = 0;
 *(int32_t*)K = L;
 I = K;
 int32_t M = 5;
 int32_t N = temv;
 int8_t O = N==M;
 int8_t P = !O;
 if (P){goto _label0; };
 int32_t Q = *(int32_t*)I;
 int32_t R = 1;
 int32_t S = Q+R;
 *(int32_t*)I = S;
 _label0:
 void* T = I;
 free(T);
 return;

}
int8_t func()
{ //Block 
 int8_t A = 0;
 return A;
 
}
int8_t func2()
{ //Block 
 int8_t A = 1;
 return A;
 
}
void printf1(int8_t V)
{ //Block 
 void* A;
 void* B = &V;
 A = B;
 int8_t C = *(int8_t*)A;
 int32_t D = putchar(C);
 return;

}
void Vec_sint32_____init__(Vec_sint32_* __this__)
{ //Block 
 int64_t A = 0;
  __this__->__0 = A;
 int64_t B = 0;
  __this__->__1 = B;
 int64_t C = 0;
  __this__->__2 = C;
 return;

}
void Vec_sint32_____new______(Vec_sint32_* __this__)
{ //Block 
 Vec_sint32_* A = __this__;
 Vec_sint32_____init__(A);
 return;

}
void Vec_char_____init__(Vec_char_* __this__)
{ //Block 
 int64_t A = 0;
  __this__->__0 = A;
 int64_t B = 0;
  __this__->__1 = B;
 int64_t C = 0;
  __this__->__2 = C;
 return;

}
void Vec_char_____new______(Vec_char_* __this__)
{ //Block 
 Vec_char_* A = __this__;
 Vec_char_____init__(A);
 return;

}
int main(int argc, char** argv)
{
_StaticInit();
_threadInit();

_Entry();

_thread_deInit();
_Static_deInit();

 return 0;
}