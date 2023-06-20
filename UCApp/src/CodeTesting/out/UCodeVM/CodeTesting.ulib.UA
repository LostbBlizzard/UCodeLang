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
   9 :Read32 [Ptr:[E]],[Target:[B]],4
   10 :Load32 [Ptr:[D]],[Target:[B]],4
   11 :Read32 [Ptr:[E]],[Target:[C]],8
   12 :Load32 [Ptr:[D]],[Target:[C]],8
   13 :Store32 [F],0
   14 :Ret 
---Testing:Hello:OtherFuncion: 
   15 :Ret 
