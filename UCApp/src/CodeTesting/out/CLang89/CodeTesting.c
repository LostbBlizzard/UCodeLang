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

} Ptr_sint32_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
void main2();
void Ptr_sint32_____new___Ptr_sint32___i32_(Ptr_sint32_* __this__,void* base);
void* Ptr_sint32__Get_Ptr_sint32__(Ptr_sint32_* __this__);
void* Ptr_sint32__Get_Ptr_sint32__imut(Ptr_sint32_* __this__);
void Ptr_sint32__Write_Ptr_sint32___i32_imut(Ptr_sint32_* __this__,void* newvalue);
void Ptr_sint32__Write_Ptr_sint32___i32mov(Ptr_sint32_* __this__,int32_t newvalue);
void* Ptr_sint32__ReadPtr_Ptr_sint32__imut(Ptr_sint32_* __this__);
void* Ptr_sint32____IndirectMember___Ptr_sint32__(Ptr_sint32_* __this__);
void* Ptr_sint32____IndirectMember___Ptr_sint32__imut(Ptr_sint32_* __this__);
void Ptr_sint32__ReassignTo_Ptr_sint32___i32_(Ptr_sint32_* __this__,void* newbase);
Ptr_sint32_ Ptr_sint32__Make_i32_imut(void* V);
Ptr_sint32_ Ptr_sint32__Make_i32_(void* V);
void Ptr_sint32_____new___Ptr_sint32__(Ptr_sint32_* __this__);
Ptr_sint32_ ptr_sint32__i32_(void* V);


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
void main2()
{ /*Block*/ 

 /*File:/Main.uc*/
 /*Line:9*/

 Ptr_sint32_ tep0;
 int32_t tep1 = 3;
 void* tep2 = &tep1;
 Ptr_sint32_ tep3 = ptr_sint32__i32_(tep2);
 tep0 = tep3;
 return;

}
void Ptr_sint32_____new___Ptr_sint32___i32_(Ptr_sint32_* __this__,void* base)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:12*/

 void* tep0 = base;
  __this__->__0 = tep0;
 return;

}
void* Ptr_sint32__Get_Ptr_sint32__(Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/OtherFile.uc*/
 /*Line:14*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* Ptr_sint32__Get_Ptr_sint32__imut(Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/OtherFile.uc*/
 /*Line:15*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void Ptr_sint32__Write_Ptr_sint32___i32_imut(Ptr_sint32_* __this__,void* newvalue)
{ /*Block*/ 


 return;

}
void Ptr_sint32__Write_Ptr_sint32___i32mov(Ptr_sint32_* __this__,int32_t newvalue)
{ /*Block*/ 


 return;

}
void* Ptr_sint32__ReadPtr_Ptr_sint32__imut(Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:20*/

 Ptr_sint32_* tep0 = __this__;
 void* tep1 = Ptr_sint32__Get_Ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* Ptr_sint32____IndirectMember___Ptr_sint32__(Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:22*/

 Ptr_sint32_* tep0 = __this__;
 void* tep1 = Ptr_sint32__Get_Ptr_sint32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* Ptr_sint32____IndirectMember___Ptr_sint32__imut(Ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:24*/

 Ptr_sint32_* tep0 = __this__;
 void* tep1 = Ptr_sint32__Get_Ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void Ptr_sint32__ReassignTo_Ptr_sint32___i32_(Ptr_sint32_* __this__,void* newbase)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:26*/

 void* tep0 = newbase;
  __this__->__0 = tep0;
 return;

}
Ptr_sint32_ Ptr_sint32__Make_i32_imut(void* V)
{
 Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:0*/

 Ptr_sint32_ tep0;
 Ptr_sint32_* tep1 = &tep0;

 /*Line:30*/

 int32_t tep2 = *(int32_t*)V;
 Ptr_sint32_____new___Ptr_sint32___i32_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
Ptr_sint32_ Ptr_sint32__Make_i32_(void* V)
{
 Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:0*/

 Ptr_sint32_ tep0;
 Ptr_sint32_* tep1 = &tep0;

 /*Line:31*/

 void* tep2 = V;
 Ptr_sint32_____new___Ptr_sint32___i32_(tep1,tep2);
 _ReturnVal = tep0;
 return _ReturnVal;

}
void Ptr_sint32_____new___Ptr_sint32__(Ptr_sint32_* __this__)
{ /*Block*/ 
 return;

}
Ptr_sint32_ ptr_sint32__i32_(void* V)
{
 Ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:5*/

 void* tep0 = V;
 Ptr_sint32_ tep1 = Ptr_sint32__Make_i32_(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
