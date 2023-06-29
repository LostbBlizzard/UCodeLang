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
   5 :GetFromStackSub64 8,[A]
   6 :GetPointerOfStaticMem [B],0
   7 :StoreRegToPtr64 [A],[B]
   8 :Store64 [B],8
   9 :DecrementStackPointer [B]
   10 :Ret 
---Testing:Vec2:(&_init&): 
   11 :Store32 [A],0
   12 :StoreRegToPtr32 [A],[D]
   13 :Load32 [Ptr:[D]],[Target:[A]],4
   14 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   15 :Call {Testing:Vec2:(&_init&)}
   16 :Ret 
---Testing:main: 
   17 :Store32 [A],1
   18 :GetPointerOfStaticMem [B],0
   19 :StoreFromPtrToReg64 [B],[B]
   20 :Add32 [B],[A]
   21 :StoreRegToReg32 [F],[B]
   22 :GetPointerOfStaticMem [C],0
   23 :StoreRegToPtr32 [B],[C]
   24 :GetPointerOfStaticMem [C],4
   25 :StoreFromPtrToReg64 [C],[C]
   26 :Add32 [C],[A]
   27 :StoreRegToReg32 [F],[C]
   28 :GetPointerOfStaticMem [D],4
   29 :StoreRegToPtr32 [C],[D]
   30 :GetPointerOfStaticMem [F],0
   31 :StoreFromPtrToReg64 [F],[F]
   32 :Ret 
