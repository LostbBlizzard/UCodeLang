[ClassData]-- 
$[_global]:
.size:0

 |Test[] -> sint32;//Test^sint32
 |__Log[] -> void;//__Log^umut char[&]
 |__LogChar[] -> void;//__LogChar^char
 |__LogBuffer[] -> void;//__LogBuffer^char[&],uintptr
 |__ReadChar[] -> char;//__ReadChar
 |main[] -> void;//main



[Instructions]-- 
---Test^sint32: 
   0 :CppCallNamed "Test^sint32"
   1 :Ret 
---__Log^umut char[&]: 
   2 :SysCall 0,[D]
   3 :Ret 
---__LogChar^char: 
   4 :SysCall 1,[D]
   5 :Ret 
---__LogBuffer^char[&],uintptr: 
   6 :SysCall 2,[D]
   7 :Ret 
---__ReadChar: 
   8 :SysCall 3,[F]
   9 :Ret 
---main: 
   10 :Store8 [A],72
   11 :StoreRegToReg64 [A],[D]
   12 :Call {__LogChar^char}
   13 :Ret 
