[ClassData]-- 
$[_global]:
.size:0

 |Testing:Func[sint32] -> sint32;//Testing:Func^i32
 |Testing:Test[sint32] -> Testing:Vec2;//Testing:Test^i32
 |Testing:main[Testing:Vec2&] -> Testing:Vec2;//Testing:main^[]&
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:Vec2:
.size:8
 sint32 x;//Offset 0
 sint32 y;//Offset 4




[Instructions]-- 
---Testing:Func^i32: 
   0 :StoreRegToReg32 [D],[F]
   1 :Ret 
---Testing:Vec2:(&_new&)^[]&: 
   2 :Ret 
---Testing:Test^i32: 
   3 :CppCallNamed "Testing:Test^i32"
   4 :Ret 
---Testing:main^[]&: 
   5 :Store64 [A],8
   6 :IncrementStackPointer [A]
   7 :StoreFromPtrToReg64 [D],[F]
   8 :StoreRegOnStackSub64 [F],0
   9 :GetFromStackSub64 0,[A]
   10 :Store64 [A],8
   11 :DecrementStackPointer [A]
   12 :Ret 
---Testing:Hello:OtherFuncion: 
   13 :Ret 
