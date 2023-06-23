[ClassData]-- 
$[_global]:
.size:0

 |Testing:Func[sint32] -> sint32;//Testing:Func^i32
 |Testing:Test[sint32] -> Testing:Vec3;//Testing:Test^i32
 |Testing:main2[Testing:Vec3&] -> Testing:Vec3;//Testing:main2^[]&
 |Testing:main[] -> sint32;//Testing:main
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:Object:
.size:0



$Testing:Vec3:
.size:12
 sint32 x;//Offset 0
 sint32 y;//Offset 4
 sint32 z;//Offset 8



$Testing:Vec2:
.size:8
 sint32 x;//Offset 0
 sint32 y;//Offset 4




[Instructions]-- 
---(&StaticInit&): 
   0 :Store64 [A],8
   1 :IncrementStackPointer [A]
   2 :GetPointerOfStackSub [A],8
   3 :StoreRegToReg64 [A],[D]
   4 :Call {Testing:Vec2:(&_new&)^[]&}
   5 :GetFromStackSub64 8,[B]
   6 :GetPointerOfStaticMem [C],0
   7 :StoreRegToPtr64 [B],[C]
   8 :Store64 [C],8
   9 :DecrementStackPointer [C]
   10 :Ret 
---(&threadInit&): 
   11 :Store64 [A],8
   12 :IncrementStackPointer [A]
   13 :GetPointerOfStackSub [A],8
   14 :StoreRegToReg64 [A],[D]
   15 :Call {Testing:Vec2:(&_new&)^[]&}
   16 :GetFromStackSub64 8,[B]
   17 :GetPointerOfThreadMem [C],8
   18 :StoreRegToPtr64 [B],[C]
   19 :Store64 [C],8
   20 :DecrementStackPointer [C]
   21 :Ret 
---Testing:Func^i32: 
   22 :StoreRegToReg32 [D],[F]
   23 :Ret 
---Testing:Object:(&_new&)^[]&: 
   24 :Ret 
---Testing:Vec3:(&_init&): 
   25 :Store32 [A],1
   26 :StoreRegToPtr32 [A],[D]
   27 :Store32 [B],2
   28 :Load32 [Ptr:[D]],[Target:[B]],4
   29 :Store32 [C],3
   30 :Load32 [Ptr:[D]],[Target:[C]],8
   31 :Ret 
---Testing:Vec3:(&_new&)^[]&: 
   32 :Call {Testing:Vec3:(&_init&)}
   33 :Ret 
---Testing:Vec2:(&_init&): 
   34 :Store32 [A],1
   35 :StoreRegToPtr32 [A],[D]
   36 :Store32 [B],2
   37 :Load32 [Ptr:[D]],[Target:[B]],4
   38 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   39 :Call {Testing:Vec2:(&_init&)}
   40 :Ret 
---Testing:Test^i32: 
   41 :CppCallNamed "Testing:Test"
   42 :Ret 
---Testing:main2^[]&: 
   43 :Store64 [A],12
   44 :IncrementStackPointer [A]
   45 :StoreFromPtrToReg64 [D],[A]
   46 :StoreRegOnStackSub64 [A],12
   47 :Read32 [Ptr:[D]],[Target:[A]],8
   48 :StoreRegOnStackSub32 [A],4
   49 :GetPointerOfStackSub [B],12
   50 :StoreRegToReg64 [B],[F]
   51 :Store64 [B],12
   52 :DecrementStackPointer [B]
   53 :Ret 
---Testing:main: 
   54 :Store32 [A],1
   55 :StoreRegToReg32 [A],[F]
   56 :Ret 
---Testing:Hello:OtherFuncion: 
   57 :Ret 
