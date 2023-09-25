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
 int32_t __0; 

} Optional_sint32__Option_;

typedef union 
{
 Optional_sint32__Option_ __0; 

} Optional_sint32_u;

typedef struct 
{
 int8_t __0; 
 Optional_sint32_u __1; 

} Optional_sint32_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int32_t _Entry();
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
 /*Line:110*/

 Optional_sint32_ A;
 _label3:
 int8_t B = 0;
 Optional_sint32_ C;
  C.__0 = B;
   int32_t D = 5;
  C.__1.__0.__0 = D;
 A = C;

 /*Line:112*/

 Optional_sint32_ E = A;

 /*Line:113*/
 /*Line:112*/

 Optional_sint32_* F = &A;
 int8_t G = 0;
   int8_t H = F->__0==G;
   int8_t I = !H;
 if (I){goto _label0; };

 /*Line:113*/

 int32_t J = F->__1.__0.__0;
 _ReturnVal = J;
 goto _label1;
 goto _label2;
 _label0:
 int8_t K = 1;
 Optional_sint32_ L;
  L.__0 = K;
   int8_t M = E.__0==L.__0;
 int8_t N = !M;
 if (N){goto _label3; };

 /*Line:114*/

 int32_t O = 0;
 _ReturnVal = O;
 goto _label1;
 _label1:
 goto _label2;
 _label2:
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
