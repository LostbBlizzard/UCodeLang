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

static int8_t _Const_SpanString_Hello[] = {72,101,108,108,111};
static int8_t _Const_SpanString__World[] = {32,87,111,114,108,100};

void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
void putchar(int8_t V);
void Print_c(int8_t Str);
void Print___imut(StringSpan_t_char_ Str);
void Println_c(int8_t Str);
void Println___imut(StringSpan_t_char_ Str);
int8_t _Entry();
void StringSpan_t_char_____new______(StringSpan_t_char_* __this__);
void StringSpan_t_char_____new_______c____uptr(StringSpan_t_char_* __this__,void* Data,int64_t Size);
void String_t_char_____new______(String_t_char_* __this__);
void String_t_char_____new_________imut(String_t_char_* __this__,StringSpan_t_char_ string);
void String_t_char_____drop______(String_t_char_* __this__);
void String_t_char__realloc_____uptr(String_t_char_* __this__,int64_t size);
StringSpan_t_char_ String_t_char__AsSpan____(String_t_char_* __this__);
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
void Print_c(int8_t Str)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:75*/

 int8_t A = Str;
 putchar(A);
 return;

}
void Print___imut(StringSpan_t_char_ Str)
{ /*Block*/ 


 int64_t A;
 int64_t B = 0;
 A = B;
 _label1:
  int64_t C = Str.__1;
 int64_t D = A;
 int8_t E = D<C;
 int8_t F = !E;
 if (F){goto _label0; };

 /*Line:82*/

  void* G = Str.__0;
 int64_t H = A;
 int64_t I = 1;
 int64_t J = H*I;
 void* K = (int64_t)G+J;
 int8_t L = *(int8_t*)K;
 putchar(L);

 /*Line:81*/

 int64_t M = A;
 int64_t N = 1;
 int64_t O = M+N;
 A = O;
 goto _label1;
 _label0:
 return;

}
void Println_c(int8_t Str)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:86*/

 int8_t A = Str;
 Print_c(A);

 /*Line:87*/

 int8_t B = 10;
 Print_c(B);
 return;

}
void Println___imut(StringSpan_t_char_ Str)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:90*/

 StringSpan_t_char_ A = Str;
 Print___imut(A);

 /*Line:91*/

 int8_t B = 10;
 Print_c(B);
 return;

}
int8_t _Entry()
{
 int8_t _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:94*/

 StringSpan_t_char_ A;
 void* B = &_Const_SpanString_Hello;
 int64_t C = 5;
 StringSpan_t_char_ D;
 StringSpan_t_char_* E = &D;
 StringSpan_t_char_____new_______c____uptr(E,B,C);
 A = D;

 /*Line:95*/

 StringSpan_t_char_ F;
 void* G = &_Const_SpanString__World;
 int64_t H = 6;
 StringSpan_t_char_ I;
 StringSpan_t_char_* J = &I;
 StringSpan_t_char_____new_______c____uptr(J,G,H);
 F = I;

 /*Line:97*/

 String_t_char_ K;
 StringSpan_t_char_ L = A;
 String_t_char_ M;
 String_t_char_* N = &M;
 String_t_char_____new_________imut(N,L);
 K = M;

 /*Line:99*/

 String_t_char_* O = &K;
 StringSpan_t_char_ P = String_t_char__AsSpan____(O);
 Println___imut(P);

 /*Line:101*/

 String_t_char_* Q = &K;
 StringSpan_t_char_ R = F;
 String_t_char____Compound_Plus_________imut(Q,R);

 /*Line:103*/

 String_t_char_* S = &K;
 StringSpan_t_char_ T = String_t_char__AsSpan____(S);
 Println___imut(T);

 /*Line:105*/

 int8_t U;
 int64_t V = 11;
  int64_t W = K.__1;
 int8_t X = W==V;
 U = X;

 /*Line:106*/

 int8_t Y;
 int8_t Z = 100;
  void* ZB = K.__0;
 int64_t ZC = 1;
  int64_t ZD = K.__1;
 int64_t ZE = ZD-ZC;
 int64_t ZF = 1;
 int64_t ZG = ZE*ZF;
 void* ZH = (int64_t)ZB+ZG;
 int8_t ZI = *(int8_t*)ZH;
 int8_t ZJ = ZI==Z;
 Y = ZJ;

 /*Line:108*/

 int8_t ZK = Y;
 int8_t ZL = U;
 int8_t ZM = ZL&&ZK;
 _ReturnVal = ZM;

 /*Line:0*/

 String_t_char_* ZN = &K;
 String_t_char_____drop______(ZN);
 return _ReturnVal;

}
void StringSpan_t_char_____new______(StringSpan_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:8*/

 int32_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:9*/

 int64_t B = 0;
  __this__->__1 = B;
 return;

}
void StringSpan_t_char_____new_______c____uptr(StringSpan_t_char_* __this__,void* Data,int64_t Size)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:12*/

 void* A = Data;
  __this__->__0 = A;

 /*Line:13*/

 int64_t B = Size;
  __this__->__1 = B;
 return;

}
void String_t_char_____new______(String_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:24*/

 int32_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:25*/

 int64_t B = 0;
  __this__->__1 = B;

 /*Line:26*/

 int64_t C = 0;
  __this__->__2 = C;
 return;

}
void String_t_char_____new_________imut(String_t_char_* __this__,StringSpan_t_char_ string)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:28*/

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

 /*Line:29*/

  int64_t L = string.__1;
  __this__->__1 = L;

 /*Line:30*/

  int64_t M = string.__1;
 
 /*Line:32*/

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

 /*Line:33*/

  void* T = string.__0;
 int64_t U = N;
 int64_t V = 1;
 int64_t W = U*V;
 void* X = (int64_t)T+W;
 int8_t Y = *(int8_t*)X;
  void* Z = __this__->__0;
 int64_t ZB = N;
 int64_t ZC = 1;
 int64_t ZD = ZB*ZC;
 void* ZE = (int64_t)Z+ZD;
 *(int8_t*)ZE = Y;

 /*Line:32*/

 int64_t ZF = N;
 int64_t ZG = 1;
 int64_t ZH = ZF+ZG;
 N = ZH;
 goto _label3;
 _label2:
 return;

}
void String_t_char_____drop______(String_t_char_* __this__)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:36*/

 int64_t A;
  void* B = __this__->__0;
 A = (int64_t)B;

 /*Line:37*/

 int64_t C = 0;
 int64_t D = A;
 int8_t E = D==C;
 int8_t F = !E;
 if (F){goto _label0; };
 
 /*Line:38*/

 void* G = __this__->__0;
 free(G);
 _label0:
 return;

}
void String_t_char__realloc_____uptr(String_t_char_* __this__,int64_t size)
{ /*Block*/ 


 
 /*File:/src.uc*/
 /*Line:42*/

 int64_t A = __this__->__2;
 int64_t B = size;
 int8_t C = B>A;
 int8_t D = !C;
 if (D){goto _label0; };

 /*Line:43*/

 int64_t E;
  int64_t F = __this__->__1;
 E = F;

 /*Line:44*/

 void* G;
  void* H = __this__->__0;
 G = H;

 /*Line:46*/

 int64_t I = size;
  __this__->__2 = I;

 /*Line:47*/

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
 
 /*Line:48*/

 __this__->__0 = M;


 int64_t U;
 int64_t V = 0;
 U = V;
 _label4:
 int64_t W = E;
 int64_t X = U;
 int8_t Y = X<W;
 int8_t Z = !Y;
 if (Z){goto _label3; };

 /*Line:49*/

 void* ZB = G;
 int64_t ZC = U;
 int64_t ZD = 1;
 int64_t ZE = ZC*ZD;
 void* ZF = (int64_t)ZB+ZE;
 int8_t ZG = *(int8_t*)ZF;
  void* ZH = __this__->__0;
 int64_t ZI = U;
 int64_t ZJ = 1;
 int64_t ZK = ZI*ZJ;
 void* ZL = (int64_t)ZH+ZK;
 *(int8_t*)ZL = ZG;

 /*Line:48*/

 int64_t ZM = U;
 int64_t ZN = 1;
 int64_t ZO = ZM+ZN;
 U = ZO;
 goto _label4;
 _label3:

 /*Line:52*/

 void* ZP = G;
 free(ZP);
 _label0:
 return;

}
StringSpan_t_char_ String_t_char__AsSpan____(String_t_char_* __this__)
{
 StringSpan_t_char_ _ReturnVal;
 /*Block*/ 

 /*File:/src.uc*/
 /*Line:55*/

 StringSpan_t_char_ A;
 StringSpan_t_char_* B = &A;
  void* C = __this__->__0;
  int64_t D = __this__->__1;
 StringSpan_t_char_____new_______c____uptr(B,C,D);
 _ReturnVal = A;
 return _ReturnVal;

}
void String_t_char____Compound_Plus_________imut(String_t_char_* __this__,StringSpan_t_char_ string)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:59*/

 int64_t A;
  int64_t B = string.__1;
  int64_t C = __this__->__1;
 int64_t D = C+B;
 A = D;

 /*Line:60*/

 String_t_char_* E = __this__;
 int64_t F = A;
 String_t_char__realloc_____uptr(E,F);

 /*Line:62*/

 int64_t G;
  int64_t H = __this__->__1;
 G = H;

 /*Line:63*/

 int64_t I = A;
 
 /*Line:65*/

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

 /*Line:66*/

  void* P = string.__0;
 int64_t Q = J;
 int64_t R = 1;
 int64_t S = Q*R;
 void* T = (int64_t)P+S;
 int8_t U = *(int8_t*)T;
  void* V = __this__->__0;
 int64_t W = G;
 int64_t X = J;
 int64_t Y = X+W;
 int64_t Z = 1;
 int64_t ZB = Y*Z;
 void* ZC = (int64_t)V+ZB;
 *(int8_t*)ZC = U;

 /*Line:65*/

 int64_t ZD = J;
 int64_t ZE = 1;
 int64_t ZF = ZD+ZE;
 J = ZF;
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
