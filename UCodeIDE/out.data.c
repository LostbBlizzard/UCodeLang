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

} Range_t_uintptr_;

typedef struct 
{
 int8_t __0; 
 int8_t __1; 

} Range_t_uint8_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int64_t _Entry();
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__);
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end);
Range_t_uintptr_ Range_uintptr__uptr_uptr(int64_t start,int64_t end);
void Range_t_uint8_____new___Range_t_uint8__(Range_t_uint8_* __this__);
void Range_t_uint8_____new___Range_t_uint8___u8_u8(Range_t_uint8_* __this__,int8_t start,int8_t end);
Range_t_uint8_ Range_uint8__u8_u8(int8_t start,int8_t end);
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
int64_t _Entry()
{
 int64_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:72*/

 Range_t_uintptr_ A;
 int64_t B = 0;
 int64_t C = 5;
 Range_t_uintptr_ D = Range_uintptr__uptr_uptr(B,C);
 A = D;

 /*Line:73*/

 Range_t_uint8_ E;
 int8_t F = 0;
 int8_t G = 5;
 Range_t_uint8_ H = Range_uint8__u8_u8(F,G);
 E = H;

 /*Line:75*/

  int8_t I = E.__1;
 uint8_t J_tep = *(uint8_t*)&I;
 uint64_t J_tep2 = (uint64_t)J_tep;
 int64_t J = *(int64_t*)&J_tep2;
  int64_t K = A.__1;
 int64_t L = K+J;
 _ReturnVal = L;
 return _ReturnVal;

}
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:62*/
 /*Line:0*/

 int64_t A = 0;
  __this__->__0 = A;

 /*Line:63*/
 /*Line:0*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:65*/

 int64_t A = start;
  __this__->__0 = A;

 /*Line:66*/

 int64_t B = end;
  __this__->__1 = B;
 return;

}
Range_t_uintptr_ Range_uintptr__uptr_uptr(int64_t start,int64_t end)
{
 Range_t_uintptr_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:0*/

 Range_t_uintptr_ A;
 Range_t_uintptr_* B = &A;

 /*Line:69*/

 int64_t C = start;
 int64_t D = end;
 Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(B,C,D);
 _ReturnVal = A;
 return _ReturnVal;

}
void Range_t_uint8_____new___Range_t_uint8__(Range_t_uint8_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:62*/
 /*Line:0*/

 int8_t A = 0;
  __this__->__0 = A;

 /*Line:63*/
 /*Line:0*/

 int8_t B = 0;
  __this__->__1 = B;
 return;

}
void Range_t_uint8_____new___Range_t_uint8___u8_u8(Range_t_uint8_* __this__,int8_t start,int8_t end)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:65*/

 int8_t A = start;
  __this__->__0 = A;

 /*Line:66*/

 int8_t B = end;
  __this__->__1 = B;
 return;

}
Range_t_uint8_ Range_uint8__u8_u8(int8_t start,int8_t end)
{
 Range_t_uint8_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:0*/

 Range_t_uint8_ A;
 Range_t_uint8_* B = &A;

 /*Line:69*/

 int8_t C = start;
 int8_t D = end;
 Range_t_uint8_____new___Range_t_uint8___u8_u8(B,C,D);
 _ReturnVal = A;
 return _ReturnVal;

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
