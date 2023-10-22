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

} Unique_ptr_sint32_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int8_t Tests_Unique_ptr_1();
void Unique_ptr_sint32_____new___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__);
void Unique_ptr_sint32_____new___Unique_ptr_sint32___Unique_ptr_sint32__mov(Unique_ptr_sint32_* __this__,Unique_ptr_sint32_* Value);
void Unique_ptr_sint32_____drop___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__);
Unique_ptr_sint32_ Unique_ptr_sint32__Make();
void* Unique_ptr_sint32__Get_Unique_ptr_sint32__(Unique_ptr_sint32_* __this__);
void* Unique_ptr_sint32__Get_Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__);
void Unique_ptr_sint32__Write_Unique_ptr_sint32___i32_imut(Unique_ptr_sint32_* __this__,void* newvalue);
void Unique_ptr_sint32__Write_Unique_ptr_sint32___i32mov(Unique_ptr_sint32_* __this__,int32_t newvalue);
void* Unique_ptr_sint32__ReadPtr_Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__);
void* Unique_ptr_sint32____IndirectMember___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__);
void* Unique_ptr_sint32____IndirectMember___Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__);


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
int8_t Tests_Unique_ptr_1()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/Main.uc*/
 /*Line:11*/

 Unique_ptr_sint32_ tep0;
 Unique_ptr_sint32_ tep1 = Unique_ptr_sint32__Make();
 tep0 = tep1;

 /*Line:12*/

 Unique_ptr_sint32_* tep2 = &tep0;
 void* tep3 = Unique_ptr_sint32__Get_Unique_ptr_sint32__(tep2);
 int32_t tep4 = *(int32_t*)tep3;
 int32_t tep5 = 0;
 int8_t tep6 = (int8_t)(tep4==tep5);
 _ReturnVal = tep6;

 /*Line:0*/

 Unique_ptr_sint32_* tep7 = &tep0;
 Unique_ptr_sint32_____drop___Unique_ptr_sint32__(tep7);
 return _ReturnVal;

}
void Unique_ptr_sint32_____new___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:7*/

 int64_t tep0 = 0;
  __this__->__0 = (void*)tep0;
 return;

}
void Unique_ptr_sint32_____new___Unique_ptr_sint32___Unique_ptr_sint32__mov(Unique_ptr_sint32_* __this__,Unique_ptr_sint32_* Value)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:12*/

  void* tep0 = Value->__0;
  __this__->__0 = tep0;

 /*Line:13*/

 int64_t tep1 = 0;
  Value->__0 = (void*)tep1;
 return;

}
void Unique_ptr_sint32_____drop___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:16*/

 int64_t tep0;
  void* tep1 = __this__->__0;
 tep0 = (int64_t)tep1;

 /*Line:17*/

 int64_t tep2 = tep0;
 int64_t tep3 = 0;
 int8_t tep4 = (int8_t)(tep2!=tep3);
 int8_t tep5 = !tep4;
 if (tep5){goto _label0; };
 
 /*Line:18*/

 void* tep6 = __this__->__0;
 free(tep6);

 _label0:

 return;

}
Unique_ptr_sint32_ Unique_ptr_sint32__Make()
{
 Unique_ptr_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:21*/

 Unique_ptr_sint32_ tep0;

 /*Line:0*/

 Unique_ptr_sint32_* tep1 = &tep0;
 Unique_ptr_sint32_____new___Unique_ptr_sint32__(tep1);

 /*Line:21*/
 /*Line:22*/

 int64_t tep2 = 4;
 void* tep3 = malloc(tep2);

 /*Line:0*/

 int32_t tep4 = 0;
 *(int32_t*)tep3 = tep4;

 /*Line:22*/

  tep0.__0 = tep3;

 /*Line:23*/

 Unique_ptr_sint32_* tep5 = &tep0;
 Unique_ptr_sint32_ tep6;
 Unique_ptr_sint32_* tep7 = &tep6;
 Unique_ptr_sint32_____new___Unique_ptr_sint32___Unique_ptr_sint32__mov(tep7,tep5);
 _ReturnVal = tep6;

 /*Line:0*/

 Unique_ptr_sint32_* tep8 = &tep0;
 Unique_ptr_sint32_____drop___Unique_ptr_sint32__(tep8);
 return _ReturnVal;

}
void* Unique_ptr_sint32__Get_Unique_ptr_sint32__(Unique_ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/OtherFile.uc*/
 /*Line:26*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void* Unique_ptr_sint32__Get_Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 


 
 /*File:/OtherFile.uc*/
 /*Line:27*/

 void* tep0 = __this__->__0;
 _ReturnVal = tep0;
 return _ReturnVal;

}
void Unique_ptr_sint32__Write_Unique_ptr_sint32___i32_imut(Unique_ptr_sint32_* __this__,void* newvalue)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:29*/

 int32_t tep0 = *(int32_t*)newvalue;
 Unique_ptr_sint32_* tep1 = __this__;
 void* tep2 = Unique_ptr_sint32__Get_Unique_ptr_sint32__(tep1);
 *(int32_t*)tep2 = tep0;
 return;

}
void Unique_ptr_sint32__Write_Unique_ptr_sint32___i32mov(Unique_ptr_sint32_* __this__,int32_t newvalue)
{ /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:30*/

 int32_t tep0 = newvalue;
 Unique_ptr_sint32_* tep1 = __this__;
 void* tep2 = Unique_ptr_sint32__Get_Unique_ptr_sint32__(tep1);
 *(int32_t*)tep2 = tep0;
 return;

}
void* Unique_ptr_sint32__ReadPtr_Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:32*/

 Unique_ptr_sint32_* tep0 = __this__;
 void* tep1 = Unique_ptr_sint32__Get_Unique_ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* Unique_ptr_sint32____IndirectMember___Unique_ptr_sint32__(Unique_ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:34*/

 Unique_ptr_sint32_* tep0 = __this__;
 void* tep1 = Unique_ptr_sint32__Get_Unique_ptr_sint32__(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
void* Unique_ptr_sint32____IndirectMember___Unique_ptr_sint32__imut(Unique_ptr_sint32_* __this__)
{
 void* _ReturnVal;
 /*Block*/ 

 /*File:/OtherFile.uc*/
 /*Line:36*/

 Unique_ptr_sint32_* tep0 = __this__;
 void* tep1 = Unique_ptr_sint32__Get_Unique_ptr_sint32__imut(tep0);
 _ReturnVal = tep1;
 return _ReturnVal;

}
