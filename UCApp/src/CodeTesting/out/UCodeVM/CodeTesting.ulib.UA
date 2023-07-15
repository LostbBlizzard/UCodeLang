[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> float32;//Testing:main


$Testing:Vec2:
.size:8
 float32 X;//Offset 0
 float32 Y;//Offset 4

 |Testing:Vec2:(&_new&)[Testing:Vec2&float32float32] -> void;//Testing:Vec2:(&_new&)^[]&,f32,f32



[Instructions]-- 
---Testing:Vec2:(&_new&)^[]&,f32,f32: 
   0 :StoreRegToPtr32 [E],[D]
   1 :Load32 [Ptr:[D]],[Target:[F]],4
   2 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   3 :Ret 
---Testing:main: 
   4 :Store64 [A],8
   5 :IncrementStackPointer [A]
   6 :GetPointerOfStackSub [A],8
   7 :Store32f [B],0.000000
   8 :StoreRegToReg64 [A],[D]
   9 :StoreRegToReg32 [B],[E]
   10 :StoreRegToReg32 [B],[F]
   11 :Call {Testing:Vec2:(&_new&)^[]&,f32,f32}
   12 :GetFromStackSub32 8,[F]
   13 :Store64 [A],8
   14 :DecrementStackPointer [A]
   15 :Ret 
