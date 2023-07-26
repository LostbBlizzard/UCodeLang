[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main



[Instructions:UCodeVm]-- 
---Testing:main: 

   //File:/Main.uc
   //Line:33
   //Source Line:  int A = 0 + 7;

   0 :Store32 [A],7
   1 :Store32 [B],0
   2 :Add32 [B],[A]

   //Line:34
   //Source Line:  A+=5;

   3 :StoreRegToReg32 [F],[C]

   //[C] = Testing:main:A

   4 :Store32 [E],5
   5 :Add32 [C],[E]

   //Line:35
   //Source Line:  ret A;

   6 :Ret 
