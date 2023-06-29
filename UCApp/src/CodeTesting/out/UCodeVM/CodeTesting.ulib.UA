[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> Testing:Vec2;//Testing:main


$Testing:Vec2:
.size:12
 sint32 X;//Offset 0
 sint32 Y;//Offset 4
 sint32 Z;//Offset 8




[Instructions]-- 
---(&StaticInit&): 
   0 :Store64 [A],12
   1 :IncrementStackPointer [A]
   2 :GetPointerOfStackSub [A],12
   3 :StoreRegToReg64 [A],[D]
   4 :Call {Testing:Vec2:(&_new&)^[]&}
   5 :GetFromStackSub64 12,[A]
   6 :GetPointerOfStaticMem [B],0
   7 :StoreRegToPtr64 [A],[B]
   8 :GetFromStackSub32 4,[A]
   9 :GetPointerOfStaticMem [B],8
   10 :StoreRegToPtr32 [A],[B]
   11 :Store64 [B],12
   12 :DecrementStackPointer [B]
   13 :Ret 
---Testing:Vec2:(&_init&): 
   14 :Store32 [A],0
   15 :StoreRegToPtr32 [A],[D]
   16 :Load32 [Ptr:[D]],[Target:[A]],4
   17 :Load32 [Ptr:[D]],[Target:[A]],8
   18 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   19 :Call {Testing:Vec2:(&_init&)}
   20 :Ret 
---Testing:main: 
   21 :Store32 [A],1
   22 :GetPointerOfStaticMem [B],0
   23 :StoreFromPtrToReg32 [B],[B]
   24 :Add32 [B],[A]
   25 :StoreRegToReg32 [F],[B]
   26 :GetPointerOfStaticMem [C],0
   27 :StoreRegToPtr32 [B],[C]
   28 :GetPointerOfStaticMem [C],4
   29 :StoreFromPtrToReg32 [C],[C]
   30 :Add32 [C],[A]
   31 :StoreRegToReg32 [F],[C]
   32 :GetPointerOfStaticMem [D],4
   33 :StoreRegToPtr32 [C],[D]
   34 :GetPointerOfStaticMem [D],8
   35 :StoreFromPtrToReg32 [D],[D]
   36 :Add32 [D],[A]
   37 :StoreRegToReg32 [F],[D]
   38 :GetPointerOfStaticMem [E],8
   39 :StoreRegToPtr32 [D],[E]
   40 :StoreRegToReg64 [E],[F]
   41 :Ret 
