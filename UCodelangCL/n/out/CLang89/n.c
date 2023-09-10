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

//includes

#include <inttypes.h>
//Types
typedef float float32_t;
typedef double float64_t;



//file.h


void _StaticInit();
void _Static_deInit();
void _threadInit();
void _thread_deInit();
int32_t _Entry();
int32_t func_i32(int32_t a);
int main(int argc, char** argv);

//file.cpp

void _StaticInit()
{
}
void _Static_deInit()
{
}
void _threadInit()
{
}
void _thread_deInit()
{
}
int32_t _Entry()
{ //Block 
 int32_t A = 0;
 return A;
 
}
int32_t func_i32(int32_t a)
{ //Block 
 int32_t B = a;
 return B;
 
}
int main(int argc, char** argv)
{
_StaticInit();
_threadInit();

int exitcode = _Entry();

_thread_deInit();
_Static_deInit();

 return exitcode;
}