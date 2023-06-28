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
   3 : StoreRegToReg64 [A],[D]
   4 :Call {Testing:Vec2:(&_new&)^[]&}
   5 :GetFromStackSub64 8,[B]
   6 :GetPointerOfStaticMem [C],0
   7 :StoreRegToPtr64 [B],[C]
   8 :Store64 [C],8
   9 :DecrementStackPointer [C]
   10 :Ret 
---Testing:Vec2:(&_init&): 
   11 :Store32 [A],1
   12 :StoreRegToPtr32 [A],[D]
   13 :Store32 [B],2
   14 :Load32 [Ptr:[D]],[Target:[B]],4
   15 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   16 :Call {Testing:Vec2:(&_init&)}
   17 :Ret 
---Testing:main: 
   18 :Store32 [A],12
   19 :StoreRegToReg64 [A],[B]
   20 :GetPointerOfStaticMem [C],0
   21 :StoreRegToPtr64 [B],[C]
   22 :Store32 [C],13
   23 :StoreRegToReg64 [C],[D]
   24 :GetPointerOfStaticMem [E],4
   25 :StoreRegToPtr64 [D],[E]
   26 :StoreRegToReg64 [A],[F]
   27 :Ret 
