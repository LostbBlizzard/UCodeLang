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

} StringSpan_t_char_;

typedef struct 
{
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} String_t_char_;

static int8_t _Const_SpanString_Hello;
static int8_t _Const_SpanString__World;

void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int64_t _Entry();
void StringSpan_t_char_____new______(StringSpan_t_char_* __this__);
void StringSpan_t_char_____new_______c____uptr(StringSpan_t_char_* __this__,void* Data,int64_t Size);
void String_t_char_____new______(String_t_char_* __this__);
void String_t_char_____new_________imut(String_t_char_* __this__,StringSpan_t_char_ string);
void String_t_char_____drop______(String_t_char_* __this__);
void String_t_char__realloc_____uptr(String_t_char_* __this__,int64_t size);
void String_t_char____Compound_Plus_________imut(String_t_char_* __this__,StringSpan_t_char_ string);
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
 /*Line:65*/

 StringSpan_t_char_ A;
 void* B = &_Const_SpanString_Hello;
 int64_t C = 5;
 StringSpan_t_char_ D;
 StringSpan_t_char_* E = &D;
 StringSpan_t_char_____new_______c____uptr(E,B,C);
 A = D;

 /*Line:66*/

 StringSpan_t_char_ F;
 void* G = &_Const_SpanString__World;
 int64_t H = 6;
 StringSpan_t_char_ I;
 StringSpan_t_char_* J = &I;
 StringSpan_t_char_____new_______c____uptr(J,G,H);
 F = I;

 /*Line:68*/

 String_t_char_ K;
 StringSpan_t_char_ L = A;
 String_t_char_ M;
 String_t_char_* N = &M;
 String_t_char_____new_________imut(N,L);
 K = M;

 /*Line:69*/

 String_t_char_* O = &K;
 StringSpan_t_char_ P = F;
 String_t_char____Compound_Plus_________imut(O,P);

 /*Line:70*/

  int64_t Q = K.__1;
 _ReturnVal = Q;

 /*Line:0*/

 String_t_char_* R = &K;
 String_t_char_____drop______(R);
 return _ReturnVal;

}
void StringSpan_t_char_____new______(StringSpan_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:7*/

 int32_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:8*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void StringSpan_t_char_____new_______c____uptr(StringSpan_t_char_* __this__,void* Data,int64_t Size)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:11*/

 void* A = Data;
  __this__->__0 = A;

 /*Line:12*/

 int64_t B = Size;
  __this__->__1 = B;
 return;

}
void String_t_char_____new______(String_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:23*/

 int32_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:24*/

 int64_t B = 0;
  __this__->__1 = B;

 /*Line:25*/

 int64_t C = 0;
  __this__->__2 = C;
 return;

}
void String_t_char_____new_________imut(String_t_char_* __this__,StringSpan_t_char_ string)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:27*/

  int64_t A = string.__1;
 int64_t B = 1;
 int64_t C = A*B;
 void* D = malloc(C);
 int64_t E = 0;
 _label1:
 int8_t F = E==A;
 if (F){goto _label0; };
 int64_t G = E*B;
 void* H = (int64_t)D+G;

 /*Line:0*/

 int8_t I = 0;
 *(int8_t*)H = I;
 int32_t J = 1;
 int64_t K = E+(int64_t)J;
 E = K;
 goto _label1;
 _label0:
  __this__->__0 = D;

 /*Line:28*/

  int64_t L = string.__1;
  __this__->__1 = L;

 /*Line:29*/

  int64_t M = string.__1;
 
 /*Line:31*/

 __this__->__2 = M;


 int64_t N;
 int64_t O = 0;
 N = O;
 _label3:
  int64_t P = string.__1;
 int64_t Q = N;
 int8_t R = Q<P;
 int8_t S = !R;
 if (S){goto _label2; };
 int64_t T = N;
 int64_t U = 1;
 int64_t V = T+U;
 N = V;
 goto _label3;
 _label2:
 return;

}
void String_t_char_____drop______(String_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:35*/

 int64_t A;
  void* B = __this__->__0;
 A = (int64_t)B;

 /*Line:36*/

 int64_t C = 0;
 int64_t D = A;
 int8_t E = D==C;
 int8_t F = !E;
 if (F){goto _label0; };
 
 /*Line:37*/

 void* G = __this__->__0;
 free(G);
 _label0:
 return;

}
void String_t_char__realloc_____uptr(String_t_char_* __this__,int64_t size)
{ /*Block*/ 


 
 /*File:/src.uc*/
 /*Line:41*/

 int64_t A = __this__->__2;
 int64_t B = size;
 int8_t C = B>A;
 int8_t D = !C;
 if (D){goto _label0; };

 /*Line:42*/

 int64_t E;
  int64_t F = __this__->__1;
 E = F;

 /*Line:43*/

 void* G;
  void* H = __this__->__0;
 G = H;

 /*Line:45*/

 int64_t I = size;
  __this__->__2 = I;

 /*Line:46*/

 int64_t J = size;
 int64_t K = 1;
 int64_t L = J*K;
 void* M = malloc(L);
 int64_t N = 0;
 _label2:
 int8_t O = N==J;
 if (O){goto _label1; };
 int64_t P = N*K;
 void* Q = (int64_t)M+P;

 /*Line:0*/

 int8_t R = 0;
 *(int8_t*)Q = R;
 int32_t S = 1;
 int64_t T = N+(int64_t)S;
 N = T;
 goto _label2;
 _label1:
  __this__->__0 = M;
 _label0:
 return;

}
void String_t_char____Compound_Plus_________imut(String_t_char_* __this__,StringSpan_t_char_ string)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:51*/

 int64_t A;
  int64_t B = string.__1;
  int64_t C = __this__->__1;
 int64_t D = C+B;
 A = D;

 /*Line:52*/

 String_t_char_* E = __this__;
 int64_t F = A;
 String_t_char__realloc_____uptr(E,F);

 /*Line:54*/

 int64_t G;
  int64_t H = __this__->__1;
 G = H;

 /*Line:55*/

 int64_t I = A;
 
 /*Line:57*/

 __this__->__1 = I;


 int64_t J;
 int64_t K = 0;
 J = K;
 _label1:
 int64_t L = A;
 int64_t M = J;
 int8_t N = M<L;
 int8_t O = !N;
 if (O){goto _label0; };
 int64_t P = J;
 int64_t Q = 1;
 int64_t R = P+Q;
 J = R;
 goto _label1;
 _label0:
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
