[ClassData]-- 
$[_global]:
.size:0

 |Testing:Test[sint32sint32] -> sint32;//Testing:Test^i32,i32
 |Testing:InLineMe[sint32] -> sint32;//Testing:InLineMe^i32
 |Testing:HH[sint32] -> sint32;//Testing:HH^i32



[Instructions]-- 
---Testing:Test^i32,i32: 
   0 :CppCallNamed "Testing:Test"
   1 :Ret 
---Testing:InLineMe^i32: 
   2 :Store32 [A],9
   3 :Add32 [D],[A]
   4 :Ret 
---Testing:HH^i32: 
   5 :Store32 [A],1
   6 :StoreRegToReg32 [A],[B]
   7 :StoreRegToReg32 [B],[D]
   8 :Call {Testing:InLineMe^i32}
   9 :Ret 
