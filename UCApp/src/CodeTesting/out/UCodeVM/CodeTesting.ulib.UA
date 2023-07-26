[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main
 |Testing:main2[sint32sint32sint32sint32] -> sint32;//Testing:main2^i32,i32,i32,i32



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
---Testing:main2^i32,i32,i32,i32: 

   //File:/Main.uc
   //Line:41
   //Source Line:  ret Par1;
   //[D] = Testing:main2:Par1
   //[D] = Testing:main2:Par2
   //[D] = Testing:main2:Par3
   //[D] = Testing:main2:Par4
   //[D] = Testing:main2:Par1
   //[D] = Testing:main2:Par2
   //[D] = Testing:main2:Par3
   //[D] = Testing:main2:Par4
   //[D] = Testing:main2:Par1
   //[D] = Testing:main2:Par2
   //[D] = Testing:main2:Par3
   //[D] = Testing:main2:Par4
   //[D] = Testing:main2:Par1
   //[D] = Testing:main2:Par2
   //[D] = Testing:main2:Par3
   //[D] = Testing:main2:Par4

   7 :StoreRegToReg32 [D],[F]
   8 :Ret 
