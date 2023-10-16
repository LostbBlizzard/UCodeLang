[ClassData]-- 
$[_global]:
.size:0

 |main[] -> sint32;//main


$[_global]:
.size:0

 |Hello[] -> sint32;//Hello



[Instructions:UCodeVm]-- 
---main: 
   0 :Debug_FuncStart 
   1 :Call {Hello}

   //File:/Main.uc
   //Line:4
   //Source Line:[Cant Find Line]

   5 :Debug_LineEnter 
   6 :Debug_FuncEnd 
   7 :Ret 
---Hello: 
   8 :Debug_FuncStart 
   9 :Store32 [A], 8

   //File:/OtherFile.uc
   //Line:3
   //Source Line:[Cant Find Line]

   11 :StoreRegToReg32 [A],[F]
   12 :Debug_LineEnter 
   13 :Debug_FuncEnd 
   14 :Ret 
