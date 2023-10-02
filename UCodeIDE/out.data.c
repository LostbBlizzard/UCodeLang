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
 int32_t __0; 

} Object;

typedef struct 
{
 int32_t __0; 
 int32_t __1; 

} main___Lambda__1917362813248___Lambda__1917362815888class;

typedef struct 
{
 int32_t __0; 

} main___Lambda__1917362813248class;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int32_t _Entry();
int32_t main___Lambda__1917362813248class___Invoke___main___Lambda__1917362813248class__i32_i32(main___Lambda__1917362813248class* __this__,int32_t a,int32_t b);
int32_t main___Lambda__1917362813248___Lambda__1917362815888class___Invoke___main___Lambda__1917362813248___Lambda__1917362815888class__i32(main___Lambda__1917362813248___Lambda__1917362815888class* __this__,int32_t x);
void Object____init__(Object* __this__);
int32_t Object_Call2_Object_(Object* __this__);
void Object_Call_Object_(Object* __this__);
void Object____new___Object_(Object* __this__);
void main2();
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

 /*Line:24*/

 int32_t Atepvir;
 int32_t Atepvistepvir = 2;
 Atepvir = Atepvistepvir;

 /*Line:25*/

 main___Lambda__1917362813248class Atepvistepvistepvir;
 main___Lambda__1917362813248class Atepvistepvistepvistepvir;
 int32_t Atepvistepvistepvistepvistepvir = Atepvir;
 
 /*Line:25*/

 Atepvistepvistepvistepvir.__0 = Atepvistepvistepvistepvistepvir;
 Atepvistepvistepvir = Atepvistepvistepvistepvir;

 /*Line:30*/

 main___Lambda__1917362813248class* Atepvistepvistepvistepvistepvistepvir = &Atepvistepvistepvir;
 int32_t Atepvistepvistepvistepvistepvistepvistepvir = 14;
 int32_t Atepvistepvistepvistepvistepvistepvistepvistepvir = 6;
 int32_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvir = main___Lambda__1917362813248class___Invoke___main___Lambda__1917362813248class__i32_i32(Atepvistepvistepvistepvistepvistepvir,Atepvistepvistepvistepvistepvistepvistepvir,Atepvistepvistepvistepvistepvistepvistepvistepvir);
 _ReturnVal = Atepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int32_t main___Lambda__1917362813248class___Invoke___main___Lambda__1917362813248class__i32_i32(main___Lambda__1917362813248class* __this__,int32_t a,int32_t b)
{
 int32_t _ReturnVal;
 /*Block*/ 

 /*Line:26*/

 main___Lambda__1917362813248___Lambda__1917362815888class Atepvir;
 main___Lambda__1917362813248___Lambda__1917362815888class Atepvistepvir;
   Atepvistepvir.__0 = __this__->__0;
 int32_t Atepvistepvistepvir = a;
 
 /*Line:26*/

 Atepvistepvir.__1 = Atepvistepvistepvir;
 Atepvir = Atepvistepvir;

 /*Line:28*/

 int32_t Atepvistepvistepvistepvir = 2;
 main___Lambda__1917362813248___Lambda__1917362815888class* Atepvistepvistepvistepvistepvir = &Atepvir;
 int32_t Atepvistepvistepvistepvistepvistepvir = b;
 int32_t Atepvistepvistepvistepvistepvistepvistepvir = a;
 int32_t Atepvistepvistepvistepvistepvistepvistepvistepvir = Atepvistepvistepvistepvistepvistepvistepvir+Atepvistepvistepvistepvistepvistepvir;
 int32_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvir = main___Lambda__1917362813248___Lambda__1917362815888class___Invoke___main___Lambda__1917362813248___Lambda__1917362815888class__i32(Atepvistepvistepvistepvistepvir,Atepvistepvistepvistepvistepvistepvistepvistepvir);
 int32_t Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir = Atepvistepvistepvistepvistepvistepvistepvistepvistepvir/Atepvistepvistepvistepvir;
 _ReturnVal = Atepvistepvistepvistepvistepvistepvistepvistepvistepvistepvir;
 return _ReturnVal;

}
int32_t main___Lambda__1917362813248___Lambda__1917362815888class___Invoke___main___Lambda__1917362813248___Lambda__1917362815888class__i32(main___Lambda__1917362813248___Lambda__1917362815888class* __this__,int32_t x)
{
 int32_t _ReturnVal;
 /*Block*/ 
 
 /*Line:26*/

 int32_t Atepvir = __this__->__0;
  int32_t Atepvistepvir = __this__->__1;
 int32_t Atepvistepvistepvir = Atepvistepvir*Atepvir;
 _ReturnVal = Atepvistepvistepvir;
 return _ReturnVal;

}
void Object____init__(Object* __this__)
{ /*Block*/ 
 int32_t Atepvir = 10;
  __this__->__0 = Atepvir;
 return;

}
int32_t Object_Call2_Object_(Object* __this__)
{
 int32_t _ReturnVal;
 /*Block*/ 


 
 /*Line:35*/

 int32_t Atepvir = __this__->__0;
 _ReturnVal = Atepvir;
 return _ReturnVal;

}
void Object_Call_Object_(Object* __this__)
{ /*Block*/ 


 return;

}
void Object____new___Object_(Object* __this__)
{ /*Block*/ 
 Object* Atepvir = __this__;
 Object____init__(Atepvir);
 return;

}
void main2()
{ /*Block*/ 

 /*Line:45*/

 Object Atepvir;

 /*Line:0*/

 Object* Atepvistepvir = &Atepvir;

 /*Line:45*/

 Object____new___Object_(Atepvistepvir);

 /*Line:46*/

 Object* Atepvistepvistepvir = &Atepvir;
 Object_Call_Object_(Atepvistepvistepvir);
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
