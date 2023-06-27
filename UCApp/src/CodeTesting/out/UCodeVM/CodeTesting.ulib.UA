[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[sint32&] -> sint32&;//Testing:main^i32&



[Instructions]-- 
---Testing:main^i32&: 
   0 :StoreRegToReg64 [D],[B]
   1 :StoreRegToReg64 [B],[A]
   2 :StoreRegToReg64 [D],[C]
   3 :StoreRegToReg64 [C],[A]
   4 :StoreRegToReg64 [A],[F]
   5 :Ret 
