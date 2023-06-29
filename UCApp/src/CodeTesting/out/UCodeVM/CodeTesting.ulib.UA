[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> Testing:Vec2;//Testing:main


$Testing:Vec2:
.size:8
 sint32 X;//Offset 0
 sint32 Y;//Offset 4




[Instructions]-- 
---(&StaticInit&): 
   0 :Store64 [A],8
   1 :IncrementStackPointer [A]
   2 :GetPointerOfStackSub [A],8
   3 :StoreRegToReg64 [A],[D]
   4 :Call {Testing:Vec2:(&_new&)^[]&}
   5 :StoreRegToReg64 [A],[B]
   6 :GetPointerOfStaticMem [C],0
   7 :StoreRegToPtr64 [B],[C]
   8 :Store64 [C],8
   9 :DecrementStackPointer [C]
   10 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   11 :Ret 
---Testing:main: 
   12 :Store32 [A],12
   13 :StoreRegToReg32 [A],[B]
   14 :GetPointerOfStaticMem [C],0
   15 :StoreRegToPtr32 [B],[C]
   16 :Store32 [C],13
   17 :StoreRegToReg32 [C],[D]
   18 :GetPointerOfStaticMem [E],4
   19 :StoreRegToPtr32 [D],[E]
   20 :GetPointerOfStaticMem [F],0
   21 :StoreFromPtrToReg64 [F],[F]
   22 :Ret 
