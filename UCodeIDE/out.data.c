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
 void* __0; 
 int64_t __1; 
 int64_t __2; 

} Vector_sint32_;


void __StaticInit__();
void __StaticUnload__();
void __threadInit__();
void __threadUnload__();
int32_t _Entry();
void Span_sint32_____new___Span_sint32__(Span_sint32_* __this__);
void Span_sint32_____new___Span_sint32___i32____uptr(Span_sint32_* __this__,void* data,int64_t size);
void Range_t_uintptr_____new___Range_t_uintptr__(Range_t_uintptr_* __this__);
void Range_t_uintptr_____new___Range_t_uintptr___uptr_uptr(Range_t_uintptr_* __this__,int64_t start,int64_t end);
void Vector_sint32_____new___Vector_sint32__(Vector_sint32_* __this__);
void Vector_sint32__Resize_Vector_sint32___uptr(Vector_sint32_* __this__,int64_t size);
Span_sint32_ Vector_sint32____Index___Vector_sint32___Range_t_uintptr_(Vector_sint32_* __this__,Range_t_uintptr_ V);
void Vector_sint32_____new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32__Vector_sint32______new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32___Pack(Vector_sint32_* __this__,int32_t Elems0,int32_t Elems1,int32_t Elems2,int32_t Elems3,int32_t Elems4,int32_t Elems5,int32_t Elems6,int32_t Elems7,int32_t Elems8,int32_t Elems9,int32_t Elems10);
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
 /*Line:70*/

 Vector_sint32_ A;

 /*Line:0*/

 Vector_sint32_* B = &A;

 /*Line:70*/

 int32_t C = 0;
 int32_t D = 1;
 int32_t E = 2;
 int32_t F = 3;
 int32_t G = 4;
 int32_t H = 5;
 int32_t I = 6;
 int32_t J = 7;
 int32_t K = 8;
 int32_t L = 9;
 int32_t M = 10;
 Vector_sint32_____new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32__Vector_sint32______new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32___Pack(B,C,D,E,F,G,H,I,J,K,L,M);

 /*Line:72*/

 int32_t N;
 int32_t O = 0;

 /*Line:73*/

 N = O;


 int64_t P;
 int64_t Q = 0;
 P = Q;
 _label1:
  int64_t R = A.__1;
 int64_t S = P;
 int8_t T = S<R;
 int8_t U = !T;
 if (U){goto _label0; };

 /*Line:74*/

 int32_t V = N;
  void* W = A.__0;
 int64_t X = P;
 int64_t Y = 4;
 int64_t Z = X*Y;
 void* ZB = (int64_t)W+Z;
 int32_t ZC = *(int32_t*)ZB;
 int32_t ZD = V+ZC;
 N = ZD;

 /*Line:73*/

 int64_t ZE = P;
 int64_t ZF = 1;
 int64_t ZG = ZE+ZF;
 P = ZG;
 goto _label1;
 _label0:

 /*Line:76*/

 int32_t ZH = N;
 _ReturnVal = ZH;
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
void Vector_sint32__Resize_Vector_sint32___uptr(Vector_sint32_* __this__,int64_t size)
{ /*Block*/ 


 
 /*File:/src.uc*/
 /*Line:51*/

 int64_t A = __this__->__2;
 int64_t B = size;
 int8_t C = B>A;
 int8_t D = !C;
 if (D){goto _label0; };

 /*Line:52*/

 int64_t E;
  int64_t F = __this__->__1;
 E = F;

 /*Line:53*/

 void* G;
  void* H = __this__->__0;
 G = H;

 /*Line:55*/

 int64_t I = size;
  __this__->__2 = I;

 /*Line:56*/

 int64_t J = size;
  __this__->__1 = J;

 /*Line:57*/

 int64_t K = size;
 int64_t L = 4;
 int64_t M = K*L;
 void* N = malloc(M);
 int64_t O = 0;
 _label2:
 int8_t P = O==K;
 if (P){goto _label1; };
 int64_t Q = O*L;
 void* R = (int64_t)N+Q;

 /*Line:0*/

 int32_t S = 0;
 *(int32_t*)R = S;
 int32_t T = 1;
 int64_t U = O+(int64_t)T;
 O = U;
 goto _label2;
 _label1:
 
 /*Line:58*/

 __this__->__0 = N;


 int64_t V;
 int64_t W = 0;
 V = W;
 _label4:
 int64_t X = E;
 int64_t Y = V;
 int8_t Z = Y<X;
 int8_t ZB = !Z;
 if (ZB){goto _label3; };

 /*Line:59*/

 void* ZC = G;
 int64_t ZD = V;
 int64_t ZE = 4;
 int64_t ZF = ZD*ZE;
 void* ZG = (int64_t)ZC+ZF;
 int32_t ZH = *(int32_t*)ZG;
  void* ZI = __this__->__0;
 int64_t ZJ = V;
 int64_t ZK = 4;
 int64_t ZL = ZJ*ZK;
 void* ZM = (int64_t)ZI+ZL;
 *(int32_t*)ZM = ZH;

 /*Line:58*/

 int64_t ZN = V;
 int64_t ZO = 1;
 int64_t ZP = ZN+ZO;
 V = ZP;
 goto _label4;
 _label3:

 /*Line:61*/

 int64_t ZQ;
 void* ZR = G;
 ZQ = (int64_t)ZR;

 /*Line:62*/

 int64_t ZS = 0;
 int64_t ZT = ZQ;
 int8_t ZU = ZT==ZS;
 int8_t ZV = !ZU;
 if (ZV){goto _label0; };

 /*Line:63*/

 void* ZW = G;
 free(ZW);
 _label0:
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
 
 /*Line:66*/

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
void Vector_sint32_____new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32__Vector_sint32______new___sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32_sint32___Pack(Vector_sint32_* __this__,int32_t Elems0,int32_t Elems1,int32_t Elems2,int32_t Elems3,int32_t Elems4,int32_t Elems5,int32_t Elems6,int32_t Elems7,int32_t Elems8,int32_t Elems9,int32_t Elems10)
{ /*Block*/ 

 /*File:/src.uc*/
 /*Line:39*/

 int64_t A = 0;
  __this__->__0 = (void*)A;

 /*Line:40*/

 int64_t B = 0;
  __this__->__1 = B;

 /*Line:41*/

 int64_t C = 0;
  __this__->__2 = C;

 /*Line:43*/

 Vector_sint32_* D = __this__;
 int64_t E = 11;
 Vector_sint32__Resize_Vector_sint32___uptr(D,E);

 /*Line:45*/

 int64_t F;
 int64_t G = 0;
 F = G;

 /*Line:47*/

 int32_t H = Elems0;
  void* I = __this__->__0;
 int64_t J = F;
 int64_t K = 4;
 int64_t L = J*K;
 void* M = (int64_t)I+L;
 *(int32_t*)M = H;

 /*Line:48*/

 int64_t N = F;
 int64_t O = 1;
 int64_t P = N+O;
 F = P;

 /*Line:47*/

 int32_t Q = Elems1;
  void* R = __this__->__0;
 int64_t S = F;
 int64_t T = 4;
 int64_t U = S*T;
 void* V = (int64_t)R+U;
 *(int32_t*)V = Q;

 /*Line:48*/

 int64_t W = F;
 int64_t X = 1;
 int64_t Y = W+X;
 F = Y;

 /*Line:47*/

 int32_t Z = Elems2;
  void* ZB = __this__->__0;
 int64_t ZC = F;
 int64_t ZD = 4;
 int64_t ZE = ZC*ZD;
 void* ZF = (int64_t)ZB+ZE;
 *(int32_t*)ZF = Z;

 /*Line:48*/

 int64_t ZG = F;
 int64_t ZH = 1;
 int64_t ZI = ZG+ZH;
 F = ZI;

 /*Line:47*/

 int32_t ZJ = Elems3;
  void* ZK = __this__->__0;
 int64_t ZL = F;
 int64_t ZM = 4;
 int64_t ZN = ZL*ZM;
 void* ZO = (int64_t)ZK+ZN;
 *(int32_t*)ZO = ZJ;

 /*Line:48*/

 int64_t ZP = F;
 int64_t ZQ = 1;
 int64_t ZR = ZP+ZQ;
 F = ZR;

 /*Line:47*/

 int32_t ZS = Elems4;
  void* ZT = __this__->__0;
 int64_t ZU = F;
 int64_t ZV = 4;
 int64_t ZW = ZU*ZV;
 void* ZX = (int64_t)ZT+ZW;
 *(int32_t*)ZX = ZS;

 /*Line:48*/

 int64_t ZY = F;
 int64_t ZZ = 1;
 int64_t ZZB = ZY+ZZ;
 F = ZZB;

 /*Line:47*/

 int32_t ZZC = Elems5;
  void* ZZD = __this__->__0;
 int64_t ZZE = F;
 int64_t ZZF = 4;
 int64_t ZZG = ZZE*ZZF;
 void* ZZH = (int64_t)ZZD+ZZG;
 *(int32_t*)ZZH = ZZC;

 /*Line:48*/

 int64_t ZZI = F;
 int64_t ZZJ = 1;
 int64_t ZZK = ZZI+ZZJ;
 F = ZZK;

 /*Line:47*/

 int32_t ZZL = Elems6;
  void* ZZM = __this__->__0;
 int64_t ZZN = F;
 int64_t ZZO = 4;
 int64_t ZZP = ZZN*ZZO;
 void* ZZQ = (int64_t)ZZM+ZZP;
 *(int32_t*)ZZQ = ZZL;

 /*Line:48*/

 int64_t ZZR = F;
 int64_t ZZS = 1;
 int64_t ZZT = ZZR+ZZS;
 F = ZZT;

 /*Line:47*/

 int32_t ZZU = Elems7;
  void* ZZV = __this__->__0;
 int64_t ZZW = F;
 int64_t ZZX = 4;
 int64_t ZZY = ZZW*ZZX;
 void* ZZZ = (int64_t)ZZV+ZZY;
 *(int32_t*)ZZZ = ZZU;

 /*Line:48*/

 int64_t ZZZB = F;
 int64_t ZZZC = 1;
 int64_t ZZZD = ZZZB+ZZZC;
 F = ZZZD;

 /*Line:47*/

 int32_t ZZZE = Elems8;
  void* ZZZF = __this__->__0;
 int64_t ZZZG = F;
 int64_t ZZZH = 4;
 int64_t ZZZI = ZZZG*ZZZH;
 void* ZZZJ = (int64_t)ZZZF+ZZZI;
 *(int32_t*)ZZZJ = ZZZE;

 /*Line:48*/

 int64_t ZZZK = F;
 int64_t ZZZL = 1;
 int64_t ZZZM = ZZZK+ZZZL;
 F = ZZZM;

 /*Line:47*/

 int32_t ZZZN = Elems9;
  void* ZZZO = __this__->__0;
 int64_t ZZZP = F;
 int64_t ZZZQ = 4;
 int64_t ZZZR = ZZZP*ZZZQ;
 void* ZZZS = (int64_t)ZZZO+ZZZR;
 *(int32_t*)ZZZS = ZZZN;

 /*Line:48*/

 int64_t ZZZT = F;
 int64_t ZZZU = 1;
 int64_t ZZZV = ZZZT+ZZZU;
 F = ZZZV;

 /*Line:47*/

 int32_t ZZZW = Elems10;
  void* ZZZX = __this__->__0;
 int64_t ZZZY = F;
 int64_t ZZZZ = 4;
 int64_t ZZZZB = ZZZY*ZZZZ;
 void* ZZZZC = (int64_t)ZZZX+ZZZZB;
 *(int32_t*)ZZZZC = ZZZW;

 /*Line:48*/

 int64_t ZZZZD = F;
 int64_t ZZZZE = 1;
 int64_t ZZZZF = ZZZZD+ZZZZE;
 F = ZZZZF;
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
