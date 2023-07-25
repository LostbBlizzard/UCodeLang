[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main



[Instructions:UCodeVm]-- 
---Testing:main: 

   //File:/Main.uc
   //Line:32
   //Source Line:  int A = 0 + 7;
   //IR-Data:{
      int32 I;
      int32 J = 7;
      int32 K = 0;
      int32 L = K+J;
      I = L;
   //}

   0 :Store64 [A],4
   1 :IncrementStackPointer [A]
   2 :Store32 [B],7
   3 :Store32 [C],0
   4 :Add32 [C],[B]

   //Line:33
   //Source Line:  A+=5;
   //IR-Data:{
      int32 M = I;
      int32 N = 5;
      int32 O = M+N;
      I = O;
   //}

   5 :StoreRegToReg32 [F],[C]
   6 :StoreRegToReg32 [C],[A]
   7 :Store32 [D],5
   8 :Add32 [A],[D]

   //Line:34
   //Source Line:  ret A;
   //IR-Data:{
      int32 P = I;
      ret P;
      ;
   //}

   9 :StoreRegToReg32 [F],[A]
   10 :StoreRegOnStackSub32 [A],4
   11 :GetFromStackSub32 4,[F]
   12 :Store64 [E],4
   13 :DecrementStackPointer [E]
   14 :Ret 
