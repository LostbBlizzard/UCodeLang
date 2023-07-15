[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main


$Testing:Vec2i:
.size:8
 sint32 X;//Offset 0
 sint32 Y;//Offset 4




[Instructions]-- 
---Testing:Vec2i:(&_init&): 
   0 :Store32 [A],0
   1 :StoreRegToPtr32 [A],[D]
   2 :Load32 [Ptr:[D]],[Target:[A]],4
   3 :Ret 
---Testing:Vec2i:(&_new&)^[]&: 
   4 :Call {Testing:Vec2i:(&_init&)}
   5 :Ret 
---Testing:main: 
   6 :Store64 [A],8
   7 :IncrementStackPointer [A]
   8 :GetPointerOfStackSub [A],8
   9 :StoreRegToReg64 [A],[D]
   10 :Call {Testing:Vec2i:(&_new&)^[]&}
   11 :Store32 [A],0
   12 :StoreRegToReg32 [A],[F]
   13 :Store64 [B],8
   14 :DecrementStackPointer [B]
   15 :Ret 
