[ClassData]-- 
$[_global]:
.size:0

 |main3[] -> float32;//main3


$[_global]:
.size:0

 [ULang:Test]|Print1[] -> bool;//ULang:Tests:Print1
 [ULang:Test]|Print2[] -> bool;//ULang:Tests:Print2
 [ULang:Test]|Max_1[] -> bool;//ULang:Tests:Max_1
 [ULang:Test]|Max_2[] -> bool;//ULang:Tests:Max_2
 [ULang:Test]|Min_1[] -> bool;//ULang:Tests:Min_1
 [ULang:Test]|Min_2[] -> bool;//ULang:Tests:Min_2


$ULang:Compiler:
.size:0

 |Is64Bit[] -> bool;//ULang:Compiler:Is64Bit
 |Is32Bit[] -> bool;//ULang:Compiler:Is32Bit
 |IsBackendC89[] -> bool;//ULang:Compiler:IsBackendC89
 |IsBackendUCodeVM[] -> bool;//ULang:Compiler:IsBackendUCodeVM
 |IsBackendWasm[] -> bool;//ULang:Compiler:IsBackendWasm
 |IsBackendVM[] -> bool;//ULang:Compiler:IsBackendVM


$ULang:Empty:
.size:0



$ULang:LowLevel:
.size:0

 |Malloc[uIntPtr] -> void;//ULang:LowLevel:Malloc^uptr
 |Free[uint8[&]] -> void;//ULang:LowLevel:Free^u8[&]
 |Memcopy[uint8[&], uint8[&], uIntPtr] -> void;//ULang:LowLevel:Memcopy^u8[&],u8[&],uptr
 |Memmove[uint8[&], uint8[&], uIntPtr] -> void;//ULang:LowLevel:Memmove^u8[&],u8[&],uptr


$ULang:Math:
.size:0

 |Max<sint32>[sint32, sint32] -> sint32;//ULang:Math:Max<sint32>^i32,i32
 |Min<sint32>[sint32, sint32] -> sint32;//ULang:Math:Min<sint32>^i32,i32
 |Min<float32>[float32, float32] -> float32;//ULang:Math:Min<float32>^f32,f32


---

|PI<T>[] => 3.14;
 ---
---

|Max<T>[T A,T B]:
  if A > B:ret A;
  ret B;

 
 ---
---

|Min<T>[T A,T B]:
  if A < B:ret A;
  ret B;
 
 
 ---
---

|Pow<T>[T val,T power] => val;
 ---
$ULang:Test tag:
---

|ToString<T,Char_t>[imut T& Object] -> String_t<Char_t>:
 ret [];
 ---
$ULang:Fmt:
.size:0

 |Print[char] -> void;//ULang:Fmt:Print^c
 |Print[imut ULang:StringSpan_t<char>] -> void;//ULang:Fmt:Print^ULang:StringSpan_t<char>imut
 |Print[imut ULang:String_t<char>&] -> void;//ULang:Fmt:Print^ULang:String_t<char>&imut
 |Println[char] -> void;//ULang:Fmt:Println^c
 |Println[imut ULang:StringSpan_t<char>] -> void;//ULang:Fmt:Println^ULang:StringSpan_t<char>imut
 |Println[imut ULang:String_t<char>&] -> void;//ULang:Fmt:Println^ULang:String_t<char>&imut


$ULang:Debug trait:
$ULang:OpenedFile:
.size:0



$ULang:OpenFileError:
.size:0



$ULang:File:
.size:0

 |Open[ULang:StringSpan_t<char>] -> ULang:Result<ULang:OpenedFile,ULang:OpenFileError>;//ULang:File:Open^ULang:StringSpan_t<char>
 |Open[imut ULang:String_t<char>&] -> ULang:Result<ULang:OpenedFile,ULang:OpenFileError>;//ULang:File:Open^ULang:String_t<char>&imut
 |GetString[ULang:StringSpan_t<char>] -> ULang:Result<ULang:String_t<char>,ULang:OpenFileError>;//ULang:File:GetString^ULang:StringSpan_t<char>
 |GetString[imut ULang:String_t<char>&] -> ULang:Result<ULang:String_t<char>,ULang:OpenFileError>;//ULang:File:GetString^ULang:String_t<char>&imut
 |GetBytes[ULang:StringSpan_t<char>] -> ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>;//ULang:File:GetBytes^ULang:StringSpan_t<char>
 |GetBytes[imut ULang:String_t<char>&] -> ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>;//ULang:File:GetBytes^ULang:String_t<char>&imut


$ULang:Writer:
.size:0



---

$AtomicShared_ptr<T>;

//Atomic<T == Shared_ptr<T>> =

---
$ULang:Future:
.size:0



---

$Mutex<T>;

/*

|Mut[T func]:
 Lock();
 func();
 UnLock();

*/

---
$ULang:Thread:
.size:0



---

$Array<T,(Size)>:
 T[/Size] _Data;
---
---

$Optional<T> enum:
 Option[T Value],
 Null,
---
$ULang:PathChar = char;

$ULang:PathString = ULang:String_t<char>;

$ULang:PathSpan = ULang:StringSpan_t<char>;

$ULang:Path:
.size:0



---

|ptr<T>[imut T& V] => 0;
 ---
$ULang:VoidPtr = sint32;

---

$Range_t<T>:
  T _start;
  T _end;
  |new[this&]:
   _start = [];
   _end = [];
  |new[this&,T start,T end]:
   _start = start;
   _end = end;


---
---

$RangeInclusive_t<T>:
 T _start;
 T _end;
 |new[this&,T Start,T End]:
  _start = Start;
  _end = End;


---
---

|Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];


 ---
---

|RangeInclusive<T>[T start,T end]  -> RangeInclusive_t<T>:
 ret [start,end];
 ---
---

$Result<T,E> enum:
 Value[T val],
 Error[E err],
---
---

$Shared_ptr<T>;

---
---

$Span<T>:
 T[&] _data;
 uintptr _size;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;

 unsafe |new[this&,T[&] data,uintptr size]:
  _data = data;
  _size = size;

 |Size[imut this&] => _size;
 unsafe |Data[imut this&] => _data;
---
---

$StringSpan_t<T>:
 //$String = String_t<T>;
 T[&] _data;
 uintptr _size;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;

 unsafe |new[this&,T[&] data,uintptr size]:
  _data = data;
  _size = size;

 |Size[imut this&] => _size;
 unsafe |Data[imut this&] => _data;


---
---

$String_t<T>:
 //$StringSpan = StringSpan_t<T>;


---
$ULang:String = ULang:String_t<char>;

$ULang:StringSpan = ULang:StringSpan_t<char>;

$ULang:String8 = ULang:String_t<uft8>;

$ULang:StringSpan8 = ULang:StringSpan_t<uft8>;

$ULang:String16 = ULang:String_t<uft16>;

$ULang:StringSpan16 = ULang:StringSpan_t<uft16>;

$ULang:String32 = ULang:String_t<uft32>;

$ULang:StringSpan32 = ULang:StringSpan_t<uft32>;

---

$Unique_ptr<T>:
 private:
  T& _ptr;
 public:
  |new[this&] -> void:
   _ptr =: unsafe bitcast<T&>(0);
  
  |new[this&,imut this& Other] = invalid;

  |new[this&,moved this& Value] -> void:
   _ptr =: Value._ptr;
   Value._ptr =: unsafe bitcast<T&>(0);
  
  |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
    unsafe drop(_ptr);

  |Get[this&] -> T&:ret _ptr;
  |Make[] -> this:
   this r = [];
   r._ptr =: unsafe new T();
   ret r;

---
---

$Vec2f_t<T>:
 T X;
 T Y;
 
 |new[this&]:
  X = 0;
  Y = 0;

 |new[this&,T x,T y]:
  X = x;
  Y = y;


---
---

$Vec2i_t<T>:
 T X;
 T Y;
 |new[this&]:
  X = 0;
  Y = 0;
 |new[this&,T x,T y]:
  X = x;
  Y = y;


---
$ULang:Vec2 = ULang:Vec2f_t<float32>;

$ULang:Vec2i = ULang:Vec2f_t<sint32>;

---

$Vec3f_t<T>:
 T X;
 T Y;
 T Z;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;
 
 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z;


---
---

$Vec3i_t<T>:
 T X;
 T Y;
 T Z;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;
 
 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z;


---
$ULang:Vec3 = ULang:Vec3f_t<float32>;

$ULang:Vec3i = ULang:Vec3f_t<sint32>;

---

$Vector<T>:
 T[&] _data;
 uintptr _size;
 uintptr _capacity;
 //$Span = Span<T>;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;

 |Push[imut T& Val] => 0;
 //|Push[moved T Val] => 0;
 
 |Push[imut T[:] Val] => 0;
 //|Push[moved T[:] Val] => 0;
---
$ULang:StringSpan_t<char>:
.size:16
 char[&] _data;//Offset 0
 uIntPtr _size;//Offset 8

 |(&_new&)[this&] -> void;//ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&
 |(&_new&)[this&, char[&], uIntPtr] -> void;//ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&,c[&],uptr
 |Size[umut this&] -> uIntPtr;//ULang:StringSpan_t<char>:Size^ULang:StringSpan_t<char>&imut
 |Data[umut this&] -> char[&];//ULang:StringSpan_t<char>:Data^ULang:StringSpan_t<char>&imut


$ULang:String_t<char>:
.size:0



$ULang:Result<ULang:OpenedFile,ULang:OpenFileError> enum[uint8]:
 Value[ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Value!] = 0,
 Error[ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Error!] = 1
//Union = #ULang:Result<ULang:OpenedFile,ULang:OpenFileError>u

$#ULang:Result<ULang:OpenedFile,ULang:OpenFileError>u:
.size:0
 ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Value! Value;//Offset 0
 ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Error! Error;//Offset 0



$ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Value!:
.size:0
 ULang:OpenedFile val;//Offset 0



$ULang:Result<ULang:OpenedFile,ULang:OpenFileError>:Error!:
.size:0
 ULang:OpenFileError err;//Offset 0



$ULang:Result<ULang:String_t<char>,ULang:OpenFileError> enum[uint8]:
 Value[ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Value!] = 0,
 Error[ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Error!] = 1
//Union = #ULang:Result<ULang:String_t<char>,ULang:OpenFileError>u

$#ULang:Result<ULang:String_t<char>,ULang:OpenFileError>u:
.size:0
 ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Value! Value;//Offset 0
 ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Error! Error;//Offset 0



$ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Value!:
.size:0
 ULang:String_t<char> val;//Offset 0



$ULang:Result<ULang:String_t<char>,ULang:OpenFileError>:Error!:
.size:0
 ULang:OpenFileError err;//Offset 0



$ULang:Span<uint8>:
.size:16
 uint8[&] _data;//Offset 0
 uIntPtr _size;//Offset 8

 |(&_new&)[this&] -> void;//ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&
 |(&_new&)[this&, uint8[&], uIntPtr] -> void;//ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&,u8[&],uptr
 |Size[umut this&] -> uIntPtr;//ULang:Span<uint8>:Size^ULang:Span<uint8>&imut
 |Data[umut this&] -> uint8[&];//ULang:Span<uint8>:Data^ULang:Span<uint8>&imut


$ULang:Vector<uint8>:
.size:24
 uint8[&] _data;//Offset 0
 uIntPtr _size;//Offset 8
 uIntPtr _capacity;//Offset 16

 |(&_new&)[this&] -> void;//ULang:Vector<uint8>:(&_new&)^ULang:Vector<uint8>&
 |Push[imut uint8&] -> sint32;//ULang:Vector<uint8>:Push^u8&imut
 |Push[imut ULang:Span<uint8>] -> sint32;//ULang:Vector<uint8>:Push^ULang:Span<uint8>imut


$ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError> enum[uint8]:
 Value[ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Value!] = 0,
 Error[ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Error!] = 1
//Union = #ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>u

$#ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>u:
.size:24
 ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Value! Value;//Offset 0
 ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Error! Error;//Offset 0



$ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Value!:
.size:24
 ULang:Vector<uint8> val;//Offset 0



$ULang:Result<ULang:Vector<uint8>,ULang:OpenFileError>:Error!:
.size:0
 ULang:OpenFileError err;//Offset 0



$ULang:Ptr<ULang:Empty> = sint32;

$ULang:String_t<uft8>:
.size:0



$ULang:StringSpan_t<uft8>:
.size:16
 uft8[&] _data;//Offset 0
 uIntPtr _size;//Offset 8

 |(&_new&)[this&] -> void;//ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&
 |(&_new&)[this&, uft8[&], uIntPtr] -> void;//ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&,[&],uptr
 |Size[umut this&] -> uIntPtr;//ULang:StringSpan_t<uft8>:Size^ULang:StringSpan_t<uft8>&imut
 |Data[umut this&] -> uft8[&];//ULang:StringSpan_t<uft8>:Data^ULang:StringSpan_t<uft8>&imut


$ULang:String_t<uft16>:
.size:0



$ULang:StringSpan_t<uft16>:
.size:16
 uft16[&] _data;//Offset 0
 uIntPtr _size;//Offset 8

 |(&_new&)[this&] -> void;//ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&
 |(&_new&)[this&, uft16[&], uIntPtr] -> void;//ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&,[&],uptr
 |Size[umut this&] -> uIntPtr;//ULang:StringSpan_t<uft16>:Size^ULang:StringSpan_t<uft16>&imut
 |Data[umut this&] -> uft16[&];//ULang:StringSpan_t<uft16>:Data^ULang:StringSpan_t<uft16>&imut


$ULang:String_t<uft32>:
.size:0



$ULang:StringSpan_t<uft32>:
.size:16
 uft32[&] _data;//Offset 0
 uIntPtr _size;//Offset 8

 |(&_new&)[this&] -> void;//ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&
 |(&_new&)[this&, uft32[&], uIntPtr] -> void;//ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&,[&],uptr
 |Size[umut this&] -> uIntPtr;//ULang:StringSpan_t<uft32>:Size^ULang:StringSpan_t<uft32>&imut
 |Data[umut this&] -> uft32[&];//ULang:StringSpan_t<uft32>:Data^ULang:StringSpan_t<uft32>&imut


$ULang:Vec2f_t<float32>:
.size:8
 float32 X;//Offset 0
 float32 Y;//Offset 4

 |(&_new&)[this&] -> void;//ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&
 |(&_new&)[this&, float32, float32] -> void;//ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&,f32,f32


$ULang:Vec2f_t<sint32>:
.size:8
 sint32 X;//Offset 0
 sint32 Y;//Offset 4

 |(&_new&)[this&] -> void;//ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&
 |(&_new&)[this&, sint32, sint32] -> void;//ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&,i32,i32


$ULang:Vec3f_t<float32>:
.size:12
 float32 X;//Offset 0
 float32 Y;//Offset 4
 float32 Z;//Offset 8

 |(&_new&)[this&] -> void;//ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&
 |(&_new&)[this&, float32, float32, float32] -> void;//ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&,f32,f32,f32


$ULang:Vec3f_t<sint32>:
.size:12
 sint32 X;//Offset 0
 sint32 Y;//Offset 4
 sint32 Z;//Offset 8

 |(&_new&)[this&] -> void;//ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&
 |(&_new&)[this&, sint32, sint32, sint32] -> void;//ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&,i32,i32,i32


$[_global]:
.size:0

 |OS_PrintChar[char] -> void;//ULang:Native:OS_PrintChar^c
 |OS_PrintSpan[imut char[&], uIntPtr] -> void;//ULang:Native:OS_PrintSpan^c[&]imut,uptr
 |OS_PrintClear[] -> void;//ULang:Native:OS_PrintClear
 |OS_Malloc[uIntPtr] -> void;//ULang:Native:OS_Malloc^uptr
 |OS_Free[uint8[&]] -> void;//ULang:Native:OS_Free^u8[&]
 |__Log[imut char[&]] -> void;//__Log^c[&]imut
 |__LogChar[char] -> void;//__LogChar^c
 |__LogBuffer[imut char[&], uIntPtr] -> void;//__LogBuffer^c[&]imut,uptr
 |__ReadChar[] -> char;//__ReadChar
 |__Malloc[uIntPtr] -> uint8[&];//__Malloc^uptr
 |__Free[uint8[&]] -> void;//__Free^u8[&]
 |__OpenFile[char[&], uIntPtr] -> uIntPtr;//__OpenFile^c[&],uptr
 |__CloseFile[uIntPtr] -> void;//__CloseFile^uptr
 |__IsFileOpen[uIntPtr] -> void;//__IsFileOpen^uptr


$ULang:Native:PathChar = char;

$FilePathChar = char;

$FileHandle = uIntPtr;

$Socket = uIntPtr;

---

|Hello<T>[] => 8;
 ---

[Instructions:UCodeVm]-- 
---main3: 
   0 :Debug_FuncStart 
   1 :Storef32 [A], 5.000000
   3 :Storef32 [B], 8.000000

   //File:/Main.uc
   //Line:8
   //Source Line:[Cant Find Line]

   5 :Debug_LineEnter 
   6 :StoreRegToReg32 [A],[D]
   7 :StoreRegToReg32 [B],[E]
   8 :Call {ULang:Math:Min<float32>^f32,f32}
   12 :Debug_FuncEnd 
   13 :Ret 
---ULang:Math:Min<float32>^f32,f32: 
   14 :Debug_FuncStart 
   15 :Debug_LineEnter 
   16 :lessthan8 [D],[E],[F]

   //File:StandardLibarary.ulib
   //Line:2
   //Source Line:[Cant Find Line]

   17 :LogicalNot8 [F],[A]
   18 :StoreRegToReg8 [A],[B]
   19 :Jumpif [B],{28}
   23 :StoreRegToReg32 [D],[F]
   24 :Jump {30}
   28 :StoreRegToReg32 [E],[F]
   29 :Debug_LineEnter 

   //Line:3
   //Source Line:[Cant Find Line]

   30 :Debug_FuncEnd 
   31 :Ret 
---ULang:Compiler:Is64Bit: 
   32 :Debug_FuncStart 
   33 :Store8 [A],1
   34 :StoreRegToReg8 [A],[F]
   35 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:3
   //Source Line:[Cant Find Line]

   36 :Debug_FuncEnd 
   37 :Ret 
---ULang:Compiler:Is32Bit: 
   38 :Debug_FuncStart 
   39 :Store8 [A],1
   40 :StoreRegToReg8 [A],[F]
   41 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:4
   //Source Line:[Cant Find Line]

   42 :Debug_FuncEnd 
   43 :Ret 
---ULang:Compiler:IsBackendC89: 
   44 :Debug_FuncStart 
   45 :Store8 [A],1
   46 :StoreRegToReg8 [A],[F]
   47 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:6
   //Source Line:[Cant Find Line]

   48 :Debug_FuncEnd 
   49 :Ret 
---ULang:Compiler:IsBackendUCodeVM: 
   50 :Debug_FuncStart 
   51 :Store8 [A],1
   52 :StoreRegToReg8 [A],[F]
   53 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:7
   //Source Line:[Cant Find Line]

   54 :Debug_FuncEnd 
   55 :Ret 
---ULang:Compiler:IsBackendWasm: 
   56 :Debug_FuncStart 
   57 :Store8 [A],1
   58 :StoreRegToReg8 [A],[F]
   59 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:8
   //Source Line:[Cant Find Line]

   60 :Debug_FuncEnd 
   61 :Ret 
---ULang:Compiler:IsBackendVM: 
   62 :Debug_FuncStart 
   63 :Store8 [A],1
   64 :StoreRegToReg8 [A],[F]
   65 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:10
   //Source Line:[Cant Find Line]

   66 :Debug_FuncEnd 
   67 :Ret 
---ULang:Compiler:(&_new&)^ULang:Compiler&: 
   68 :Debug_FuncStart 
   69 :Debug_FuncEnd 
   70 :Ret 
---ULang:Empty:(&_new&)^ULang:Empty&: 
   71 :Debug_FuncStart 
   72 :Debug_FuncEnd 
   73 :Ret 
---ULang:LowLevel:Malloc^uptr: 
   74 :Debug_FuncStart 
   75 :Debug_LineEnter 
   76 :Call {ULang:Native:OS_Malloc^uptr}
   80 :Debug_FuncEnd 
   81 :Ret 
---ULang:LowLevel:Free^u8[&]: 
   82 :Debug_FuncStart 
   83 :Debug_LineEnter 
   84 :Call {ULang:Native:OS_Free^u8[&]}
   88 :Debug_FuncEnd 
   89 :Ret 
---ULang:LowLevel:Memcopy^u8[&],u8[&],uptr: 
   90 :Debug_FuncStart 
   91 :Debug_FuncEnd 
   92 :Ret 
---ULang:LowLevel:Memmove^u8[&],u8[&],uptr: 
   93 :Debug_FuncStart 
   94 :Debug_FuncEnd 
   95 :Ret 
---ULang:LowLevel:(&_new&)^ULang:LowLevel&: 
   96 :Debug_FuncStart 
   97 :Debug_FuncEnd 
   98 :Ret 
---ULang:Math:(&_new&)^ULang:Math&: 
   99 :Debug_FuncStart 
   100 :Debug_FuncEnd 
   101 :Ret 
---ULang:Fmt:Print^c: 
   102 :Debug_FuncStart 
   103 :Debug_LineEnter 
   104 :Call {ULang:Native:OS_PrintChar^c}
   108 :Debug_FuncEnd 
   109 :Ret 
---ULang:Fmt:Print^ULang:StringSpan_t<char>imut: 
   110 :Debug_FuncStart 
   111 :GetPointerOfStackSub [A],24
   112 :StoreRegToReg64 [A],[D]
   113 :Debug_LineEnter 

   //File:/IO/Console/Fmt.uc
   //Line:9
   //Source Line:[Cant Find Line]

   114 :Call {ULang:StringSpan_t<char>:Data^ULang:StringSpan_t<char>&imut}
   118 :GetPointerOfStackSub [A],24
   119 :StoreRegToReg64 [A],[D]
   120 :StoreRegToReg64 [F],[B]
   121 :Push64 [B]
   122 :Call {ULang:StringSpan_t<char>:Size^ULang:StringSpan_t<char>&imut}
   126 :Pop64 [B]
   127 :StoreRegToReg64 [B],[D]
   128 :StoreRegToReg64 [F],[E]
   129 :Call {ULang:Native:OS_PrintSpan^c[&]imut,uptr}
   133 :Debug_FuncEnd 
   134 :Ret 
---ULang:Fmt:Print^ULang:String_t<char>&imut: 
   135 :Debug_FuncStart 
   136 :Debug_FuncEnd 
   137 :Ret 
---ULang:Fmt:Println^c: 
   138 :Debug_FuncStart 
   139 :Debug_LineEnter 
   140 :Call {ULang:Fmt:Print^c}
   144 :Store8 [A],10
   145 :StoreRegToReg8 [A],[D]
   146 :Debug_LineEnter 

   //Line:15
   //Source Line:[Cant Find Line]

   147 :Call {ULang:Fmt:Print^c}
   151 :Debug_FuncEnd 
   152 :Ret 
---ULang:Fmt:Println^ULang:StringSpan_t<char>imut: 
   153 :Debug_FuncStart 
   154 :GetFromStackSub64 [A],24
   155 :StoreRegOnStackSub64 [A],24
   156 :GetFromStackSub64 [A],32
   157 :StoreRegOnStackSub64 [A],32
   158 :GetFromStackSub64 [B],24
   159 :Push64 [B]

   //File:/IO/Console/Fmt.uc
   //Line:18
   //Source Line:[Cant Find Line]

   160 :GetFromStackSub64 [B],24
   161 :Push64 [B]
   162 :Debug_LineEnter 
   163 :Call {ULang:Fmt:Print^ULang:StringSpan_t<char>imut}
   167 :Pop64 [A]
   168 :Pop64 [A]
   169 :Store8 [A],10
   170 :StoreRegToReg8 [A],[D]
   171 :Debug_LineEnter 

   //Line:19
   //Source Line:[Cant Find Line]

   172 :Call {ULang:Fmt:Print^c}
   176 :Debug_FuncEnd 
   177 :Ret 
---ULang:Fmt:Println^ULang:String_t<char>&imut: 
   178 :Debug_FuncStart 
   179 :Debug_LineEnter 
   180 :Call {ULang:Fmt:Print^ULang:String_t<char>&imut}
   184 :Store8 [A],10
   185 :StoreRegToReg8 [A],[D]
   186 :Debug_LineEnter 

   //Line:23
   //Source Line:[Cant Find Line]

   187 :Call {ULang:Fmt:Print^c}
   191 :Debug_FuncEnd 
   192 :Ret 
---ULang:Fmt:(&_new&)^ULang:Fmt&: 
   193 :Debug_FuncStart 
   194 :Debug_FuncEnd 
   195 :Ret 
---ULang:OpenedFile:(&_new&)^ULang:OpenedFile&: 
   196 :Debug_FuncStart 
   197 :Debug_FuncEnd 
   198 :Ret 
---ULang:OpenFileError:(&_new&)^ULang:OpenFileError&: 
   199 :Debug_FuncStart 
   200 :Debug_FuncEnd 
   201 :Ret 
---ULang:File:Open^ULang:StringSpan_t<char>: 
   202 :Debug_FuncStart 
   203 :Debug_FuncEnd 
   204 :Ret 
---ULang:File:Open^ULang:String_t<char>&imut: 
   205 :Debug_FuncStart 
   206 :Debug_FuncEnd 
   207 :Ret 
---ULang:File:GetString^ULang:StringSpan_t<char>: 
   208 :Debug_FuncStart 
   209 :Debug_FuncEnd 
   210 :Ret 
---ULang:File:GetString^ULang:String_t<char>&imut: 
   211 :Debug_FuncStart 
   212 :Debug_FuncEnd 
   213 :Ret 
---ULang:File:GetBytes^ULang:StringSpan_t<char>: 
   214 :Debug_FuncStart 
   215 :Debug_FuncEnd 
   216 :Ret 
---ULang:File:GetBytes^ULang:String_t<char>&imut: 
   217 :Debug_FuncStart 
   218 :Debug_FuncEnd 
   219 :Ret 
---ULang:File:(&_new&)^ULang:File&: 
   220 :Debug_FuncStart 
   221 :Debug_FuncEnd 
   222 :Ret 
---ULang:Writer:(&_new&)^ULang:Writer&: 
   223 :Debug_FuncStart 
   224 :Debug_FuncEnd 
   225 :Ret 
---ULang:Tests:Print1: 

   //StackFrameSize:16

   226 :Store64 [A], 16

   //File:/Tests/IOTest.uc
   //Line:7
   //Source Line:[Cant Find Line]

   230 :IncrementStackPointer [A]
   231 :Debug_FuncStart 
   232 :GetPointerOfStaticMem [A],0
   233 :Store64 [B], 11
   237 :Debug_LineEnter 
   238 :GetPointerOfStackSub [C],16
   239 :StoreRegToReg64 [C],[D]
   240 :StoreRegToReg64 [A],[E]
   241 :StoreRegToReg64 [B],[F]
   242 :Call {ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&,c[&],uptr}
   246 :GetFromStackSub64 [A],16
   247 :Push64 [A]
   248 :GetFromStackSub64 [A],16
   249 :Push64 [A]
   250 :Call {ULang:Fmt:Println^ULang:StringSpan_t<char>imut}

   //Line:8
   //Source Line:[Cant Find Line]

   254 :Pop64 [A]
   255 :Pop64 [A]
   256 :Store8 [A],1
   257 :StoreRegToReg8 [A],[F]
   258 :Debug_LineEnter 
   259 :Store64 [B], 16
   263 :DecrementStackPointer [B]
   264 :Debug_FuncEnd 
   265 :Ret 
---ULang:Tests:Print2: 
   266 :Debug_FuncStart 
   267 :Store8 [A],72

   //File:/Tests/IOTest.uc
   //Line:12
   //Source Line:[Cant Find Line]

   268 :StoreRegToReg8 [A],[D]
   269 :Debug_LineEnter 
   270 :Call {ULang:Fmt:Print^c}
   274 :Store8 [A],105

   //Line:13
   //Source Line:[Cant Find Line]

   275 :StoreRegToReg8 [A],[D]
   276 :Debug_LineEnter 
   277 :Call {ULang:Fmt:Println^c}
   281 :Store8 [A],1

   //Line:14
   //Source Line:[Cant Find Line]

   282 :StoreRegToReg8 [A],[F]
   283 :Debug_LineEnter 
   284 :Debug_FuncEnd 
   285 :Ret 
---ULang:Tests:Max_1: 
   286 :Debug_FuncStart 
   287 :Store32 [A], 5

   //File:/Tests/Math.uc
   //Line:7
   //Source Line:[Cant Find Line]

   289 :Debug_LineEnter 
   290 :Store32 [B], 2
   292 :StoreRegToReg32 [A],[D]
   293 :StoreRegToReg32 [B],[E]
   294 :Call {ULang:Math:Max<sint32>^i32,i32}
   298 :Store32 [A], 5
   300 :equalto8 [F],[A],[F]
   301 :Debug_FuncEnd 
   302 :Ret 
---ULang:Tests:Max_2: 
   303 :Debug_FuncStart 
   304 :Store32 [A], 5

   //File:/Tests/Math.uc
   //Line:11
   //Source Line:[Cant Find Line]

   306 :Store32 [B], 2
   308 :Debug_LineEnter 
   309 :StoreRegToReg32 [B],[D]
   310 :StoreRegToReg32 [A],[E]
   311 :Call {ULang:Math:Max<sint32>^i32,i32}
   315 :Store32 [A], 5
   317 :equalto8 [F],[A],[F]
   318 :Debug_FuncEnd 
   319 :Ret 
---ULang:Tests:Min_1: 
   320 :Debug_FuncStart 
   321 :Store32 [A], 2

   //File:/Tests/Math.uc
   //Line:15
   //Source Line:[Cant Find Line]

   323 :Store32 [B], 5
   325 :Debug_LineEnter 
   326 :StoreRegToReg32 [B],[D]
   327 :StoreRegToReg32 [A],[E]
   328 :Call {ULang:Math:Min<sint32>^i32,i32}
   332 :Store32 [A], 2
   334 :equalto8 [F],[A],[F]
   335 :Debug_FuncEnd 
   336 :Ret 
---ULang:Tests:Min_2: 
   337 :Debug_FuncStart 
   338 :Store32 [A], 2

   //File:/Tests/Math.uc
   //Line:19
   //Source Line:[Cant Find Line]

   340 :Debug_LineEnter 
   341 :Store32 [B], 5
   343 :StoreRegToReg32 [A],[D]
   344 :StoreRegToReg32 [B],[E]
   345 :Call {ULang:Math:Min<sint32>^i32,i32}
   349 :Store32 [A], 2
   351 :equalto8 [F],[A],[F]
   352 :Debug_FuncEnd 
   353 :Ret 
---ULang:Future:(&_new&)^ULang:Future&: 
   354 :Debug_FuncStart 
   355 :Debug_FuncEnd 
   356 :Ret 
---ULang:Thread:(&_new&)^ULang:Thread&: 
   357 :Debug_FuncStart 
   358 :Debug_FuncEnd 
   359 :Ret 
---ULang:Path:(&_new&)^ULang:Path&: 
   360 :Debug_FuncStart 
   361 :Debug_FuncEnd 
   362 :Ret 
---ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&: 
   363 :Debug_FuncStart 
   364 :Store64 [A], 0

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   368 :Debug_LineEnter 
   369 :StoreRegToPtr64 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   370 :Debug_LineEnter 
   371 :Load64 [D],[A],8
   372 :Debug_FuncEnd 
   373 :Ret 
---ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&,c[&],uptr: 
   374 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   375 :Debug_LineEnter 
   376 :StoreRegToPtr64 [E],[D]

   //Line:13
   //Source Line:[Cant Find Line]

   377 :Debug_LineEnter 
   378 :Load64 [D],[F],8
   379 :Debug_FuncEnd 
   380 :Ret 
---ULang:StringSpan_t<char>:Size^ULang:StringSpan_t<char>&imut: 
   381 :Debug_FuncStart 
   382 :Read64 [D],[A],8

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   383 :StoreRegToReg64 [A],[F]
   384 :Debug_LineEnter 
   385 :Debug_FuncEnd 
   386 :Ret 
---ULang:StringSpan_t<char>:Data^ULang:StringSpan_t<char>&imut: 
   387 :Debug_FuncStart 
   388 :StoreFromPtrToReg64 [D],[A]

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   389 :StoreRegToReg64 [A],[F]
   390 :Debug_LineEnter 
   391 :Debug_FuncEnd 
   392 :Ret 
---ULang:String_t<char>:(&_new&)^ULang:String_t<char>&: 
   393 :Debug_FuncStart 
   394 :Debug_FuncEnd 
   395 :Ret 
---ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&: 
   396 :Debug_FuncStart 
   397 :Store64 [A], 0

   //File:/Types/Span.uc
   //Line:5
   //Source Line:[Cant Find Line]

   401 :Debug_LineEnter 
   402 :StoreRegToPtr64 [A],[D]

   //Line:6
   //Source Line:[Cant Find Line]

   403 :Debug_LineEnter 
   404 :Load64 [D],[A],8
   405 :Debug_FuncEnd 
   406 :Ret 
---ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&,u8[&],uptr: 
   407 :Debug_FuncStart 

   //File:/Types/Span.uc
   //Line:9
   //Source Line:[Cant Find Line]

   408 :Debug_LineEnter 
   409 :StoreRegToPtr64 [E],[D]

   //Line:10
   //Source Line:[Cant Find Line]

   410 :Debug_LineEnter 
   411 :Load64 [D],[F],8
   412 :Debug_FuncEnd 
   413 :Ret 
---ULang:Span<uint8>:Size^ULang:Span<uint8>&imut: 
   414 :Debug_FuncStart 
   415 :Read64 [D],[A],8

   //File:/Types/Span.uc
   //Line:12
   //Source Line:[Cant Find Line]

   416 :StoreRegToReg64 [A],[F]
   417 :Debug_LineEnter 
   418 :Debug_FuncEnd 
   419 :Ret 
---ULang:Span<uint8>:Data^ULang:Span<uint8>&imut: 
   420 :Debug_FuncStart 
   421 :StoreFromPtrToReg64 [D],[A]

   //File:/Types/Span.uc
   //Line:13
   //Source Line:[Cant Find Line]

   422 :StoreRegToReg64 [A],[F]
   423 :Debug_LineEnter 
   424 :Debug_FuncEnd 
   425 :Ret 
---ULang:Vector<uint8>:(&_new&)^ULang:Vector<uint8>&: 
   426 :Debug_FuncStart 
   427 :Store64 [A], 0

   //File:/Types/Vector.uc
   //Line:8
   //Source Line:[Cant Find Line]

   431 :Debug_LineEnter 
   432 :StoreRegToPtr64 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   433 :Debug_LineEnter 
   434 :Load64 [D],[A],8

   //Line:10
   //Source Line:[Cant Find Line]

   435 :Debug_LineEnter 
   436 :Load64 [D],[A],16
   437 :Debug_FuncEnd 
   438 :Ret 
---ULang:Vector<uint8>:Push^u8&imut: 
   439 :Debug_FuncStart 
   440 :Store32 [A], 0

   //File:/Types/Vector.uc
   //Line:12
   //Source Line:[Cant Find Line]

   442 :StoreRegToReg32 [A],[F]
   443 :Debug_LineEnter 
   444 :Debug_FuncEnd 
   445 :Ret 
---ULang:Vector<uint8>:Push^ULang:Span<uint8>imut: 

   //[A] = ULang:Vector<uint8>:Push:Val

   446 :Debug_FuncStart 
   447 :Store32 [A], 0

   //File:/Types/Vector.uc
   //Line:15
   //Source Line:[Cant Find Line]

   449 :StoreRegToReg32 [A],[F]
   450 :Debug_LineEnter 
   451 :Debug_FuncEnd 
   452 :Ret 
---ULang:Math:Max<sint32>^i32,i32: 

   //[A] = ULang:Vector<uint8>:Push:Val

   453 :Debug_FuncStart 

   //File:/Helper/Math.uc
   //Line:6
   //Source Line:[Cant Find Line]

   454 :Debug_LineEnter 
   455 :greaterthan8 [D],[E],[F]
   456 :LogicalNot8 [F],[A]
   457 :StoreRegToReg8 [A],[B]
   458 :Jumpif [B],{467}
   462 :StoreRegToReg32 [D],[F]
   463 :Jump {469}

   //Line:7
   //Source Line:[Cant Find Line]

   467 :StoreRegToReg32 [E],[F]
   468 :Debug_LineEnter 
   469 :Debug_FuncEnd 
   470 :Ret 
---ULang:Math:Min<sint32>^i32,i32: 
   471 :Debug_FuncStart 

   //File:/Helper/Math.uc
   //Line:10
   //Source Line:[Cant Find Line]

   472 :Debug_LineEnter 
   473 :lessthan8 [D],[E],[F]
   474 :LogicalNot8 [F],[A]
   475 :StoreRegToReg8 [A],[B]
   476 :Jumpif [B],{485}
   480 :StoreRegToReg32 [D],[F]
   481 :Jump {487}

   //Line:11
   //Source Line:[Cant Find Line]

   485 :StoreRegToReg32 [E],[F]
   486 :Debug_LineEnter 
   487 :Debug_FuncEnd 
   488 :Ret 
---ULang:String_t<uft8>:(&_new&)^ULang:String_t<uft8>&: 
   489 :Debug_FuncStart 
   490 :Debug_FuncEnd 
   491 :Ret 
---ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&: 
   492 :Debug_FuncStart 
   493 :Store64 [A], 0

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   497 :Debug_LineEnter 
   498 :StoreRegToPtr64 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   499 :Debug_LineEnter 
   500 :Load64 [D],[A],8
   501 :Debug_FuncEnd 
   502 :Ret 
---ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&,[&],uptr: 
   503 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   504 :Debug_LineEnter 
   505 :StoreRegToPtr64 [E],[D]

   //Line:13
   //Source Line:[Cant Find Line]

   506 :Debug_LineEnter 
   507 :Load64 [D],[F],8
   508 :Debug_FuncEnd 
   509 :Ret 
---ULang:StringSpan_t<uft8>:Size^ULang:StringSpan_t<uft8>&imut: 
   510 :Debug_FuncStart 
   511 :Read64 [D],[A],8

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   512 :StoreRegToReg64 [A],[F]
   513 :Debug_LineEnter 
   514 :Debug_FuncEnd 
   515 :Ret 
---ULang:StringSpan_t<uft8>:Data^ULang:StringSpan_t<uft8>&imut: 
   516 :Debug_FuncStart 
   517 :StoreFromPtrToReg64 [D],[A]

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   518 :StoreRegToReg64 [A],[F]
   519 :Debug_LineEnter 
   520 :Debug_FuncEnd 
   521 :Ret 
---ULang:String_t<uft16>:(&_new&)^ULang:String_t<uft16>&: 
   522 :Debug_FuncStart 
   523 :Debug_FuncEnd 
   524 :Ret 
---ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&: 
   525 :Debug_FuncStart 
   526 :Store64 [A], 0

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   530 :Debug_LineEnter 
   531 :StoreRegToPtr64 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   532 :Debug_LineEnter 
   533 :Load64 [D],[A],8
   534 :Debug_FuncEnd 
   535 :Ret 
---ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&,[&],uptr: 
   536 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   537 :Debug_LineEnter 
   538 :StoreRegToPtr64 [E],[D]

   //Line:13
   //Source Line:[Cant Find Line]

   539 :Debug_LineEnter 
   540 :Load64 [D],[F],8
   541 :Debug_FuncEnd 
   542 :Ret 
---ULang:StringSpan_t<uft16>:Size^ULang:StringSpan_t<uft16>&imut: 
   543 :Debug_FuncStart 
   544 :Read64 [D],[A],8

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   545 :StoreRegToReg64 [A],[F]
   546 :Debug_LineEnter 
   547 :Debug_FuncEnd 
   548 :Ret 
---ULang:StringSpan_t<uft16>:Data^ULang:StringSpan_t<uft16>&imut: 
   549 :Debug_FuncStart 
   550 :StoreFromPtrToReg64 [D],[A]

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   551 :StoreRegToReg64 [A],[F]
   552 :Debug_LineEnter 
   553 :Debug_FuncEnd 
   554 :Ret 
---ULang:String_t<uft32>:(&_new&)^ULang:String_t<uft32>&: 
   555 :Debug_FuncStart 
   556 :Debug_FuncEnd 
   557 :Ret 
---ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&: 
   558 :Debug_FuncStart 
   559 :Store64 [A], 0

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   563 :Debug_LineEnter 
   564 :StoreRegToPtr64 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   565 :Debug_LineEnter 
   566 :Load64 [D],[A],8
   567 :Debug_FuncEnd 
   568 :Ret 
---ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&,[&],uptr: 
   569 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   570 :Debug_LineEnter 
   571 :StoreRegToPtr64 [E],[D]

   //Line:13
   //Source Line:[Cant Find Line]

   572 :Debug_LineEnter 
   573 :Load64 [D],[F],8
   574 :Debug_FuncEnd 
   575 :Ret 
---ULang:StringSpan_t<uft32>:Size^ULang:StringSpan_t<uft32>&imut: 
   576 :Debug_FuncStart 
   577 :Read64 [D],[A],8

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   578 :StoreRegToReg64 [A],[F]
   579 :Debug_LineEnter 
   580 :Debug_FuncEnd 
   581 :Ret 
---ULang:StringSpan_t<uft32>:Data^ULang:StringSpan_t<uft32>&imut: 
   582 :Debug_FuncStart 
   583 :StoreFromPtrToReg64 [D],[A]

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   584 :StoreRegToReg64 [A],[F]
   585 :Debug_LineEnter 
   586 :Debug_FuncEnd 
   587 :Ret 
---ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&: 
   588 :Debug_FuncStart 
   589 :Storef32 [A], 0.000000

   //File:/Types/Vec2.uc
   //Line:7
   //Source Line:[Cant Find Line]

   591 :Debug_LineEnter 
   592 :StoreRegToPtr32 [A],[D]

   //Line:8
   //Source Line:[Cant Find Line]

   593 :Debug_LineEnter 
   594 :Load32 [D],[A],4
   595 :Debug_FuncEnd 
   596 :Ret 
---ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&,f32,f32: 
   597 :Debug_FuncStart 

   //File:/Types/Vec2.uc
   //Line:11
   //Source Line:[Cant Find Line]

   598 :Debug_LineEnter 
   599 :StoreRegToPtr32 [E],[D]

   //Line:12
   //Source Line:[Cant Find Line]

   600 :Debug_LineEnter 
   601 :Load32 [D],[F],4
   602 :Debug_FuncEnd 
   603 :Ret 
---ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&: 
   604 :Debug_FuncStart 
   605 :Store32 [A], 0

   //File:/Types/Vec2.uc
   //Line:7
   //Source Line:[Cant Find Line]

   607 :Debug_LineEnter 
   608 :StoreRegToPtr32 [A],[D]

   //Line:8
   //Source Line:[Cant Find Line]

   609 :Debug_LineEnter 
   610 :Load32 [D],[A],4
   611 :Debug_FuncEnd 
   612 :Ret 
---ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&,i32,i32: 
   613 :Debug_FuncStart 

   //File:/Types/Vec2.uc
   //Line:11
   //Source Line:[Cant Find Line]

   614 :Debug_LineEnter 
   615 :StoreRegToPtr32 [E],[D]

   //Line:12
   //Source Line:[Cant Find Line]

   616 :Debug_LineEnter 
   617 :Load32 [D],[F],4
   618 :Debug_FuncEnd 
   619 :Ret 
---ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&: 
   620 :Debug_FuncStart 
   621 :Storef32 [A], 0.000000

   //File:/Types/Vec3.uc
   //Line:8
   //Source Line:[Cant Find Line]

   623 :Debug_LineEnter 
   624 :StoreRegToPtr32 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   625 :Debug_LineEnter 
   626 :Load32 [D],[A],4

   //Line:10
   //Source Line:[Cant Find Line]

   627 :Debug_LineEnter 
   628 :Load32 [D],[A],8
   629 :Debug_FuncEnd 
   630 :Ret 
---ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&,f32,f32,f32: 
   631 :Debug_FuncStart 

   //File:/Types/Vec3.uc
   //Line:13
   //Source Line:[Cant Find Line]

   632 :Debug_LineEnter 
   633 :StoreRegToPtr32 [E],[D]

   //Line:14
   //Source Line:[Cant Find Line]

   634 :Debug_LineEnter 
   635 :Load32 [D],[F],4
   636 :GetFromStackSub32 [A],12
   637 :StoreRegOnStackSub32 [A],12

   //Line:15
   //Source Line:[Cant Find Line]

   638 :Debug_LineEnter 
   639 :GetFromStackSub32 [B],12
   640 :Load32 [D],[B],8
   641 :Debug_FuncEnd 
   642 :Ret 
---ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&: 
   643 :Debug_FuncStart 
   644 :Store32 [A], 0

   //File:/Types/Vec3.uc
   //Line:8
   //Source Line:[Cant Find Line]

   646 :Debug_LineEnter 
   647 :StoreRegToPtr32 [A],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   648 :Debug_LineEnter 
   649 :Load32 [D],[A],4

   //Line:10
   //Source Line:[Cant Find Line]

   650 :Debug_LineEnter 
   651 :Load32 [D],[A],8
   652 :Debug_FuncEnd 
   653 :Ret 
---ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&,i32,i32,i32: 
   654 :Debug_FuncStart 

   //File:/Types/Vec3.uc
   //Line:13
   //Source Line:[Cant Find Line]

   655 :Debug_LineEnter 
   656 :StoreRegToPtr32 [E],[D]

   //Line:14
   //Source Line:[Cant Find Line]

   657 :Debug_LineEnter 
   658 :Load32 [D],[F],4
   659 :GetFromStackSub32 [A],12
   660 :StoreRegOnStackSub32 [A],12

   //Line:15
   //Source Line:[Cant Find Line]

   661 :Debug_LineEnter 
   662 :GetFromStackSub32 [B],12
   663 :Load32 [D],[B],8
   664 :Debug_FuncEnd 
   665 :Ret 
---ULang:Native:OS_PrintChar^c: 
   666 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:9
   //Source Line:[Cant Find Line]

   667 :Debug_LineEnter 
   668 :Call {__LogChar^c}
   672 :Debug_FuncEnd 
   673 :Ret 
---ULang:Native:OS_PrintSpan^c[&]imut,uptr: 
   674 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:20
   //Source Line:[Cant Find Line]

   675 :Debug_LineEnter 
   676 :Call {__LogBuffer^c[&]imut,uptr}
   680 :Debug_FuncEnd 
   681 :Ret 
---ULang:Native:OS_PrintClear: 
   682 :Debug_FuncStart 
   683 :Debug_FuncEnd 
   684 :Ret 
---ULang:Native:OS_Malloc^uptr: 
   685 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:29
   //Source Line:[Cant Find Line]

   686 :Debug_LineEnter 
   687 :Call {__Malloc^uptr}
   691 :Debug_FuncEnd 
   692 :Ret 
---ULang:Native:OS_Free^u8[&]: 
   693 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:36
   //Source Line:[Cant Find Line]

   694 :Debug_LineEnter 
   695 :Call {__Free^u8[&]}
   699 :Debug_FuncEnd 
   700 :Ret 
---__Log^c[&]imut: 
   701 :Debug_FuncStart 
   702 :CppCallNamed "__Log"
   703 :Debug_FuncEnd 
   704 :Ret 
---__LogChar^c: 
   705 :Debug_FuncStart 
   706 :Ins 209:3,204,204
   707 :Debug_FuncEnd 
   708 :Ret 
---__LogBuffer^c[&]imut,uptr: 
   709 :Debug_FuncStart 
   710 :Ins 210:3,4,204
   711 :Debug_FuncEnd 
   712 :Ret 
---__ReadChar: 
   713 :Debug_FuncStart 
   714 :Ins 211:5,204,204
   715 :Debug_FuncEnd 
   716 :Ret 
---__Malloc^uptr: 
   717 :Debug_FuncStart 
   718 :Malloc [D],[F]
   719 :Debug_FuncEnd 
   720 :Ret 
---__Free^u8[&]: 
   721 :Debug_FuncStart 
   722 :Free [D]
   723 :Debug_FuncEnd 
   724 :Ret 
---__OpenFile^c[&],uptr: 
   725 :Debug_FuncStart 
   726 :CppCallNamed "__OpenFile"
   727 :Debug_FuncEnd 
   728 :Ret 
---__CloseFile^uptr: 
   729 :Debug_FuncStart 
   730 :CppCallNamed "__CloseFile"
   731 :Debug_FuncEnd 
   732 :Ret 
---__IsFileOpen^uptr: 
   733 :Debug_FuncStart 
   734 :CppCallNamed "__IsFileOpen"
   735 :Debug_FuncEnd 
   736 :Ret 
