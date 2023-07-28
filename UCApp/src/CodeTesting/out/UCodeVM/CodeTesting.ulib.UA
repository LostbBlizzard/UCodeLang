[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main
 |Testing:main2[sint32sint32sint32sint32] -> sint32;//Testing:main2^i32,i32,i32,i32



[Instructions:UCodeVm]-- 
---Testing:main: 
   0 :Debug_FuncStart 

   //File:/Main.uc
   //Line:33
   //Source Line:  int A = 0 + 7;

   1 :Debug_LineEnter 
   2 :Store32 [A],7
   3 :Store32 [B],0
   4 :Add32 [B],[A]

   //Line:34
   //Source Line:  A+=5;

   5 :StoreRegToReg32 [F],[C]

   //[C] = Testing:main:A

   6 :Debug_LineEnter 
   7 :Store32 [E],5
   8 :Add32 [C],[E]

   //Line:35
   //Source Line:  ret A;

   9 :StoreRegToReg32 [F],[C]
   10 :Debug_LineEnter 
   11 :StoreRegToReg32 [C],[F]
   12 :Debug_FuncEnd 
   13 :Ret 
---Testing:main2^i32,i32,i32,i32: 
   14 :Debug_FuncStart 

   //File:/Main.uc
   //Line:41
   //Source Line:  ret Par1;

   15 :Debug_LineEnter 

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

   16 :StoreRegToReg32 [D],[F]
   17 :Debug_FuncEnd 
   18 :Ret 
