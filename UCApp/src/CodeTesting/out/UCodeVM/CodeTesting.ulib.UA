[ClassData]-- 
$[_global]:
.size:0

 |Testing:Hello[] -> void;//Testing:Hello
 |Testing:Hello2[] -> void;//Testing:Hello2
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion



[Instructions]-- 
---ULang:Native:ConsoleOutChar^c: 
   0 :Ret 
---ULang:Native:ConsoleOutSpan^c[&]umut,uptr: 
   1 :Ret 
---ULang:Native:ConsoleOutClear: 
   2 :Ret 
---ULang:Native:ConsoleInReadChar: 
   3 :Ret 
---ULang:Native:ConsoleInReadLine^uptr&: 
   4 :Ret 
---__Debug^i32: 
   5 :CppCallNamed "__Debug^i32"
   6 :Ret 
---__Log^c[&]umut: 
   7 :SysCall 0,[D]
   8 :Ret 
---__LogChar^c: 
   9 :SysCall 1,[D]
   10 :Ret 
---__LogBuffer^c[&],uptr: 
   11 :SysCall 2,[D]
   12 :Ret 
---__ReadChar: 
   13 :SysCall 3,[F]
   14 :Ret 
---__Malloc^uptr: 
   15 :Malloc [D],[F]
   16 :Ret 
---__Free^u8[&]: 
   17 :Free [D]
   18 :Ret 
---__OpenFile^c[&],uptr: 
   19 :CppCallNamed "__OpenFile^c[&],uptr"
   20 :Ret 
---__CloseFile^uptr: 
   21 :CppCallNamed "__CloseFile^uptr"
   22 :Ret 
---__IsFileOpen^uptr: 
   23 :CppCallNamed "__IsFileOpen^uptr"
   24 :Ret 
---Testing:Hello: 
   25 :Ret 
---Testing:Hello2: 
   26 :Ret 
---Testing:Hello:OtherFuncion: 
   27 :Ret 
