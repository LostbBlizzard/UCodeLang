[ClassData]-- 
$[_global]:
.size:0

 |Testing:Func[sint32] -> sint32;//Testing:Func^i32
 |Testing:Test[sint32] -> Testing:Vec2;//Testing:Test^i32
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:Vec2:
.size:4
 uint8 x;//Offset 0
 uint8 y;//Offset 1
 uint8 z;//Offset 2
 uint8 w;//Offset 3




[Instructions]-- 
---(&StaticInit&): 
   0 :Store64 [A],4
   1 :IncrementStackPointer [A]
   2 :GetPointerOfStackSub [A],4
   3 :StoreRegToReg64 [A],[D]
   4 :Call {Testing:Vec2:(&_new&)^[]&}
   5 :GetFromStackSub32 4,[B]
   6 :GetPointerOfStaticMem [C],0
   7 :StoreRegToPtr32 [B],[C]
   8 :Store64 [C],4
   9 :DecrementStackPointer [C]
   10 :Ret 
---(&threadInit&): 
   11 :Store64 [A],4
   12 :IncrementStackPointer [A]
   13 :GetPointerOfStackSub [A],4
   14 :StoreRegToReg64 [A],[D]
   15 :Call {Testing:Vec2:(&_new&)^[]&}
   16 :GetFromStackSub32 4,[B]
   17 :GetPointerOfThreadMem [C],4
   18 :StoreRegToPtr32 [B],[C]
   19 :Store64 [C],4
   20 :DecrementStackPointer [C]
   21 :Ret 
---Testing:Func^i32: 
   22 :StoreRegToReg32 [D],[F]
   23 :Ret 
---Testing:Vec2:(&_init&): 
   24 :Store8 [A],1
   25 :StoreRegToPtr8 [A],[D]
   26 :Store8 [B],2
   27 :Load8 [Ptr:[D]],[Target:[B]],1
   28 :Store8 [C],3
   29 :Load8 [Ptr:[D]],[Target:[C]],2
   30 :Store8 [E],4
   31 :Load8 [Ptr:[D]],[Target:[E]],3
   32 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   33 :Call {Testing:Vec2:(&_init&)}
   34 :Ret 
---Testing:Test^i32: 
   35 :CppCallNamed "Testing:Test"
   36 :Ret 
---Testing:Hello:OtherFuncion: 
   37 :Ret 
