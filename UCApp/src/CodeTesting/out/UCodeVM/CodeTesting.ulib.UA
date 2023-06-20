[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[Testing:Vec2&] -> sint32;//Testing:main^[]&
 |Testing:main2[Testing:Vec2&Testing:Vec2&] -> sint32;//Testing:main2^[]&,[]&
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:Vec2:
.size:12
 sint32 x;//Offset 0
 sint32 y;//Offset 4
 sint32 z;//Offset 8




[Instructions]-- 
---Testing:Vec2:(&_new&)^[]&: 
   0 :Ret 
---Testing:main^[]&: 
   1 :Store32 [A],0
   2 :StoreRegToPtr32 [A],[D]
   3 :Load32 [Ptr:[D]],[Target:[A]],4
   4 :Load32 [Ptr:[D]],[Target:[A]],8
   5 :StoreRegToReg32 [A],[F]
   6 :Ret 
---Testing:main2^[]&,[]&: 
   7 :StoreFromPtrToReg32 [E],[A]
   8 :StoreRegToPtr32 [A],[D]
   9 :Store32 [B],0
   10 :StoreRegToReg32 [B],[F]
   11 :Ret 
---Testing:Hello:OtherFuncion: 
   12 :Ret 
