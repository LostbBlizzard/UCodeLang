[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> Testing:CoolEnum;//Testing:main
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:CoolEnum enum[uint8]:
 Red = 0,
 Blue = 1,
 Green = 2

$Testing:CoolEnum2 enum[uint8]:
 Red[sint32] = 0,
 Blue[Testing:CoolEnum2:Blue!] = 1,
 Green = 2
//Union = #Testing:CoolEnum2u

$#Testing:CoolEnum2u:
.size:4
 sint32 Red;//Offset 0
 Testing:CoolEnum2:Blue! Blue;//Offset 0



$Testing:CoolEnum2:Blue!:
.size:2
 bool x;//Offset 0
 bool y;//Offset 1




[Instructions]-- 
---Testing:main: 
   0 :Store8 [A],0
   1 :StoreRegToReg64 [A],[F]
   2 :Ret 
---Testing:Hello:OtherFuncion: 
   3 :Ret 
