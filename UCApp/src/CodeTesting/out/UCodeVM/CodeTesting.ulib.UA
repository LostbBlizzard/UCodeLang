[ClassData]-- 
$[_global]:
.size:0

 |Testing:Func[sint32] -> sint32;//Testing:Func^i32
 |Testing:Test[sint32] -> Testing:Vec2;//Testing:Test^i32
 |Testing:main[Testing:Vec2&] -> Testing:Vec2;//Testing:main^[]&
 |Testing:main2[] -> Testing:Vec2;//Testing:main2
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:Vec2:
.size:12
 sint32 x;//Offset 0
 sint32 y;//Offset 4
 sint32 z;//Offset 8




[Instructions]-- 
---Testing:Func^i32: 
   0 :StoreRegToReg32 [D],[F]
   1 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   2 :Ret 
---Testing:Test^i32: 
   3 :CppCallNamed "Testing:Test"
   4 :Ret 
---Testing:main^[]&: 
   5 :Store64 [A],12
   6 :IncrementStackPointer [A]
   7 :StoreFromPtrToReg64 [D],[A]
   8 :StoreRegOnStackSub64 [A],0
   9 :Read32 [Ptr:[D]],[Target:[A]],8
   10 :StoreRegOnStackSub32 [A],8
   11 :GetPointerOfStack [B],0
   12 :StoreRegToReg64 [B],[F]
   13 :Store64 [B],12
   14 :DecrementStackPointer [B]
   15 :Ret 
---Testing:main2: 
   16 :Store64 [A],12
   17 :IncrementStackPointer [A]
   18 :Store32 [A],0
   19 :StoreRegToReg32 [A],[D]
   20 :Call {Testing:Test^i32}
   21 :StoreFromPtrToReg64 [F],[C]
   22 :StoreRegOnStackSub64 [C],0
   23 :Read32 [Ptr:[F]],[Target:[C]],8
   24 :StoreRegOnStackSub32 [C],8
   25 :GetPointerOfStack [E],0
   26 :StoreRegToReg64 [E],[F]
   27 :Store64 [E],12
   28 :DecrementStackPointer [E]
   29 :Ret 
---Testing:Hello:OtherFuncion: 
   30 :Ret 
