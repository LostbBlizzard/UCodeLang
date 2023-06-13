[ClassData]-- 
$ = [];
$:
.size:0

 |Testing:main[] -> i32;//Testing:main
 |Testing:Hello:OtherFuncion[] -> v;//Testing:Hello:OtherFuncion


$ = [];
$ = [];
$ = [];
$:
.size:0
 i32 Red;//Offset 0
 [] Blue;//Offset 0



$ = [];
$:
.size:2
 b x;//Offset 0
 b y;//Offset 1




[Instructions]-- 
---Testing:main: 
   0 :Store32 [A],0
   1 :StoreRegToReg32 [A],[F]
   2 :Ret 
---Testing:Hello:OtherFuncion: 
   3 :Ret 
