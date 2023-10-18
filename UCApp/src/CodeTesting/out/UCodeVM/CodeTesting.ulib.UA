[ClassData]-- 
$[_global]:
.size:0



$Test:
.size:4
 sint32 a;//Offset 0

 |(&Minus&)[umut this&, imut Test&] -> Test;//Test:(&Minus&)^Test&imut,Test&imut



[Instructions:UCodeVm]-- 
---(&threadInit&): 
   0 :Debug_FuncStart 
   1 :Store32 [A], 1
   3 :Debug_LineEnter 
   4 :equalto8 [A],[A],[F]

   //File:/Main.uc
   //Line:13
   //Source Line:[Cant Find Line]

   5 :Store8 [B],65
   6 :StoreRegToReg8 [F],[C]
   7 :equalto8 [B],[B],[F]
   8 :Ins 38:5,2,5
   9 :StoreRegToReg8 [F],[D]
   10 :GetPointerOfThreadMem [E],0
   11 :StoreRegToPtr8 [D],[E]
   12 :Store32 [E], 3
   14 :Store32 [A], 2

   //Line:15
   //Source Line:[Cant Find Line]

   16 :Debug_LineEnter 
   17 :MultS32 [A],[E],[F]
   18 :Store32 [A], 1
   20 :Add32 [A],[F],[F]
   21 :StoreRegToReg32 [F],[A]
   22 :GetPointerOfThreadMem [B],1
   23 :StoreRegToPtr32 [A],[B]
   24 :Store32 [B], 2
   26 :Store32 [C], 1

   //Line:17
   //Source Line:[Cant Find Line]

   28 :Debug_LineEnter 
   29 :Add32 [C],[B],[F]
   30 :MultS32 [E],[F],[F]
   31 :StoreRegToReg32 [F],[E]
   32 :GetPointerOfThreadMem [B],5
   33 :StoreRegToPtr32 [E],[B]
   34 :Debug_FuncEnd 
   35 :Ret 
---Test:(&Minus&)^Test&imut,Test&imut: 

   //StackFrameSize:28

   36 :Store64 [A], 28
   40 :IncrementStackPointer [A]
   41 :Debug_FuncStart 
   42 :Debug_LineEnter 
   43 :StoreFromPtrToReg64 [D],[A]
   44 :StoreRegOnStackSub64 [A],28
   45 :StoreFromPtrToReg64 [D],[B]
   46 :StoreRegOnStackSub64 [B],20
   47 :StoreFromPtrToReg64 [D],[C]
   48 :StoreRegOnStackSub64 [C],12
   49 :GetFromStackSub64 [F],12
   50 :StoreRegOnStackSub64 [F],20
   51 :GetFromStackSub32 [A],28

   //Line:10
   //Source Line:[Cant Find Line]

   52 :StoreRegOnStackSub32 [A],4
   53 :Store32 [A], 10
   55 :Debug_LineEnter 

   //Line:11
   //Source Line:[Cant Find Line]

   56 :StoreRegOnStackSub32 [A],4
   57 :GetFromStackSub32 [A],4
   58 :StoreRegOnStackSub32 [A],4
   59 :GetFromStackSub32 [F],4
   60 :Debug_LineEnter 
   61 :Store64 [A], 28
   65 :DecrementStackPointer [A]
   66 :Debug_FuncEnd 
   67 :Ret 
---Test:(&_new&)^Test&: 
   68 :Debug_FuncStart 
   69 :Debug_FuncEnd 
   70 :Ret 
