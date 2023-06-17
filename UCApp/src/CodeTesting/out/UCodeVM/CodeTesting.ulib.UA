[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main
 |Testing:Hello:OtherFuncion[] -> void;//Testing:Hello:OtherFuncion


$Testing:SomeClass:
.size:12
 uIntPtr Object;//Offset 0
 sint32 Other;//Offset 8




[Instructions]-- 
---(&StaticInit&): 
   0 :Ins 163:0,0
   1 :LoadFuncPtr {(&StaticInit&)},[B]
---ULang:Native:ConsoleOutChar^c: 
   2 :Ret 
---ULang:Native:ConsoleOutSpan^c[&]umut,uptr: 
   3 :Ret 
---ULang:Native:ConsoleOutClear: 
   4 :Ret 
---ULang:Native:ConsoleInReadChar: 
   5 :Ret 
---ULang:Native:ConsoleInReadLine^uptr&: 
   6 :Ret 
---__Debug^i32: 
   7 :CppCallNamed "__Debug^i32"
   8 :Ret 
---__Log^c[&]umut: 
   9 :SysCall 0,[D]
   10 :Ret 
---__LogChar^c: 
   11 :SysCall 1,[D]
   12 :Ret 
---__LogBuffer^c[&],uptr: 
   13 :SysCall 2,[D]
   14 :Ret 
---__ReadChar: 
   15 :SysCall 3,[F]
   16 :Ret 
---__Malloc^uptr: 
   17 :Malloc [D],[F]
   18 :Ret 
---__Free^u8[&]: 
   19 :Free [D]
   20 :Ret 
---__OpenFile^c[&],uptr: 
   21 :CppCallNamed "__OpenFile^c[&],uptr"
   22 :Ret 
---__CloseFile^uptr: 
   23 :CppCallNamed "__CloseFile^uptr"
   24 :Ret 
---__IsFileOpen^uptr: 
   25 :CppCallNamed "__IsFileOpen^uptr"
   26 :Ret 
---Testing:SomeClass:(&_drop&)^[]&: 
   27 :Ret 
---Testing:SomeClass:(&_new&)^[]&: 
   28 :Ret 
---Testing:main: 
   29 :Store32 [A],0
   30 :StoreRegToReg32 [A],[F]
   31 :Ret 
---Testing:Hello:OtherFuncion: 
   32 :Ret 
