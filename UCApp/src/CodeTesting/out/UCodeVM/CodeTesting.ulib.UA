[ClassData]-- 
$[_global]:
.size:0

 |main[] -> sint32;//main
 |main2[sint32, sint32, sint32, sint32] -> sint32;//main2^i32,i32,i32,i32



[Instructions:UCodeVm]-- 
---main: 
   0 :Debug_FuncStart 

   //File:/Main.uc
   //Line:33
   //Source Line:[Cant Find Line]

   1 :Store32 [A], 7
   3 :Debug_LineEnter 
   4 :Store32 [B], 0
   6 :Add32 [B],[A],[F]
   7 :StoreRegToReg32 [F],[D]
   8 :StoreRegToReg32 [D],[C]

   //Line:34
   //Source Line:[Cant Find Line]

   9 :Store32 [E], 5
   11 :Debug_LineEnter 
   12 :Add32 [C],[E],[F]
   13 :StoreRegToReg32 [F],[A]
   14 :StoreRegToReg32 [A],[C]

   //Line:35
   //Source Line:[Cant Find Line]

   15 :StoreRegToReg32 [C],[F]
   16 :Debug_LineEnter 
   17 :Debug_FuncEnd 
   18 :Ret 
---main2^i32,i32,i32,i32: 
   19 :Debug_FuncStart 

   //File:/Main.uc
   //Line:41
   //Source Line:[Cant Find Line]

   20 :StoreRegToReg32 [D],[F]
   21 :Debug_LineEnter 
   22 :Debug_FuncEnd 
   23 :Ret 
