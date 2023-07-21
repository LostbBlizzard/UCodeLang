[ClassData]-- 
$[_global]:
.size:0

 |Testing:main3[] -> sint32&;//Testing:main3



[Instructions]-- 
---Testing:main3: 
   0 :Store64 [A],4
   1 :IncrementStackPointer [A]
   2 :Store32 [B],1
   3 :StoreRegToReg32 [B],[C]
   4 :StoreRegToReg32 [C],[A]
   5 :StoreRegToReg32 [A],[E]
   6 :StoreRegOnStackSub32 [E],4
   7 :GetPointerOfStackSub [E],4
   8 :StoreRegToReg64 [E],[A]
   9 :StoreRegToReg64 [A],[D]
   10 :StoreRegToReg32 [E],[B]
   11 :StoreRegToReg32 [B],[F]
   12 :StoreRegToReg64 [D],[F]
   13 :Store64 [D],4
   14 :DecrementStackPointer [D]
   15 :Ret 
