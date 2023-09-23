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
 void* __0; 
 int64_t __1; 

} Span_sint32_;

typedef struct 
{
 int64_t __0; 
 int64_t __1; 

} Range_t_uintptr_;

typedef struct 
{
 int32_t __0; 
 int32_t __1; 

} Range_t_sint32_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} Vector_sint32_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int64_t _Entry();
void Span_sint32_____new___Span_sint32__(Span_sint32_* __this__);
void Span_sint32_____new___Span_sint32___i32____uptr(Span_sint32_* __this__,void* data,int64_t size);
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__);
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end);
void Range_t_sint32_____new___Range_t_sint32__(Range_t_sint32_* __this__);
void Range_t_sint32_____new___Range_t_sint32___i32_i32(Range_t_sint32_* __this__,int32_t start,int32_t end);
void Vector_sint32_____new___Vector_sint32__(Vector_sint32_* __this__);
Span_sint32_ Vector_sint32____Index___Vector_sint32___Range_t_uintptr_(Vector_sint32_* __this__,Range_t_uintptr_ V);
Span_sint32_ Vector_sint32____Index___Vector_sint32___Range_t_sint32_(Vector_sint32_* __this__,Range_t_sint32_ V);
Range_t_sint32_ Range_sint32__i32_i32(int32_t start,int32_t end);
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
 /*Line:48*/

 Vector_sint32_ A;

 /*Line:0*/

 Vector_sint32_* B = &A;

 /*Line:48*/

 Vector_sint32_____new___Vector_sint32__(B);

 /*Line:50*/

 Span_sint32_ C;

 /*Line:52*/
 /*Line:50*/

 Vector_sint32_* D = &A;
 int32_t E = 5;
 int32_t F = 10;
 Range_t_sint32_ G = Range_sint32__i32_i32(E,F);
 Span_sint32_ H = Vector_sint32____Index___Vector_sint32___Range_t_sint32_(D,G);
 C = H;

 /*Line:52*/

  int64_t I = C.__1;
 _ReturnVal = I;
 return _ReturnVal;

}
void Span_sint32_____new___Span_sint32__(Span_sint32_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:21*/

 int64_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:22*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void Span_sint32_____new___Span_sint32___i32____uptr(Span_sint32_* __this__,void* data,int64_t size)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:25*/

 void* A = data;
  __this__->__0 = A;

 /*Line:26*/

 int64_t B = size;
  __this__->__1 = B;
 return;

}
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:7*/
 /*Line:0*/

 int64_t A = 0;
  __this__->__0 = A;

 /*Line:8*/
 /*Line:0*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:10*/

 int64_t A = start;
  __this__->__0 = A;

 /*Line:11*/

 int64_t B = end;
  __this__->__1 = B;
 return;

}
void Range_t_sint32_____new___Range_t_sint32__(Range_t_sint32_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:7*/
 /*Line:0*/

 int32_t A = 0;
  __this__->__0 = A;

 /*Line:8*/
 /*Line:0*/

 int32_t B = 0;
  __this__->__1 = B;
 return;

}
void Range_t_sint32_____new___Range_t_sint32___i32_i32(Range_t_sint32_* __this__,int32_t start,int32_t end)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:10*/

 int32_t A = start;
  __this__->__0 = A;

 /*Line:11*/

 int32_t B = end;
  __this__->__1 = B;
 return;

}
void Vector_sint32_____new___Vector_sint32__(Vector_sint32_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:34*/

 int64_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:35*/

 int64_t B = 0;
  __this__->__1 = B;

 /*Line:36*/

 int64_t C = 0;
  __this__->__2 = C;
 return;

}
Span_sint32_ Vector_sint32____Index___Vector_sint32___Range_t_uintptr_(Vector_sint32_* __this__,Range_t_uintptr_ V)
{
 Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:0*/

 Span_sint32_ A;
 Span_sint32_* B = &A;
 
 /*Line:41*/

 void* C = __this__->__0;
  int64_t D = V.__0;
 int64_t E = 4;
 int64_t F = D*E;
 void* G = (int64_t)C+F;
  int64_t H = V.__0;
  int64_t I = V.__1;
 int64_t J = I-H;
 Span_sint32_____new___Span_sint32___i32____uptr(B,G,J);
 _ReturnVal = A;
 return _ReturnVal;

}
Span_sint32_ Vector_sint32____Index___Vector_sint32___Range_t_sint32_(Vector_sint32_* __this__,Range_t_sint32_ V)
{
 Span_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:0*/

 Span_sint32_ A;
 Span_sint32_* B = &A;
 
 /*Line:44*/

 void* C = __this__->__0;
  int32_t D = V.__0;
 int64_t E = (int64_t)D;
 uint64_t F_tep = (uint64_t)E;
 int64_t F = *(int64_t*)&F_tep;
 int64_t G = 4;
 int64_t H = F*G;
 void* I = (int64_t)C+H;
  int32_t J = V.__0;
  int32_t K = V.__1;
 int32_t L = K-J;
 int64_t M = (int64_t)L;
 uint64_t N_tep = (uint64_t)M;
 int64_t N = *(int64_t*)&N_tep;
 Span_sint32_____new___Span_sint32___i32____uptr(B,I,N);
 _ReturnVal = A;
 return _ReturnVal;

}
Range_t_sint32_ Range_sint32__i32_i32(int32_t start,int32_t end)
{
 Range_t_sint32_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:0*/

 Range_t_sint32_ A;
 Range_t_sint32_* B = &A;

 /*Line:14*/

 int32_t C = start;
 int32_t D = end;
 Range_t_sint32_____new___Range_t_sint32___i32_i32(B,C,D);
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
