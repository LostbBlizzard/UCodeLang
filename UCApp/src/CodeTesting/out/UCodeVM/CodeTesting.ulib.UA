[ClassData]-- 
$[_global]:
.size:0

 |main[] -> sint32;//main


$[_global]:
.size:0

 [ULang:Test]|Print1[] -> bool;//ULang:Tests:Print1
 [ULang:Test]|Print2[] -> bool;//ULang:Tests:Print2
 [ULang:Test]|Max_1[] -> bool;//ULang:Tests:Max_1
 [ULang:Test]|Max_2[] -> bool;//ULang:Tests:Max_2
 [ULang:Test]|Min_1[] -> bool;//ULang:Tests:Min_1
 [ULang:Test]|Min_2[] -> bool;//ULang:Tests:Min_2
 [ULang:Test]|Ptr_1[] -> bool;//ULang:Tests:Ptr_1
 [ULang:Test]|Ptr_2[] -> bool;//ULang:Tests:Ptr_2
 [ULang:Test]|Ptr_3[] -> bool;//ULang:Tests:Ptr_3
 [ULang:Test]|Ptr_4[] -> bool;//ULang:Tests:Ptr_4
 [ULang:Test]|Ptr_5[] -> bool;//ULang:Tests:Ptr_5
 |ptr<sint32>[sint32&] -> ULang:Ptr<sint32>;//ULang:ptr<sint32>^i32&
 |ptr<ULang:Vec2f_t<float32>>[ULang:Vec2f_t<float32>&] -> ULang:Ptr<ULang:Vec2f_t<float32>>;//ULang:ptr<ULang:Vec2f_t<float32>>^ULang:Vec2f_t<float32>&


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

|iptr<T>[imut T& V] => Ptr<T>::Make(V);
 ---
---

|ptr<T>[T& V] => Ptr<T>::Make(V);
 ---
---

$Ptr<T>:
 private: 
  T& _base;
 public:
  |new[this&,T& base]:
   _base =: base;
  
  |Get[this&] -> T&:ret _base;
  |Get[imut this&] -> imut T&:ret _base;
  
  |Write[this&,imut T& newvalue] -> void;//Get() = newvalue;
  |Write[this&,moved T newvalue] -> void;//Get() = newvalue;

  |ReadPtr[imut this&] -> imut T&:ret Get();

  |~>[this&] -> T&:ret Get();

  |~>[imut this&] -> imut T&:ret Get();

  |ReassignTo[this&,T& newbase]:_base =: newbase;



  |Make[imut T& V] -> imut this:ret [unsafe bitcast<T&>(V)];
  |Make[T& V] -> this:ret [V];




---
$ULang:VoidPtr = ULang:Ptr<ULang:Empty>;

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

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


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

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);
 
 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


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

 |==[imut this&,this& Other]:
  ret this.X == Other.X && this.Y == Other.Y && this.Z == Other.Z;

 |!=[imut this&,this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;
  this.Z += Other.Z;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;
  this.Z -= Other.Z;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


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

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y && this.Z == Other.Z;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;
  this.Z += Other.Z;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;
  this.Z -= Other.Z;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


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



$ULang:Ptr<ULang:Empty>:
.size:8
 ULang:Empty& _base;//Offset 0

 |(&_new&)[this&, ULang:Empty&] -> void;//ULang:Ptr<ULang:Empty>:(&_new&)^ULang:Ptr<ULang:Empty>&,ULang:Empty&
 |Get[this&] -> ULang:Empty&;//ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&
 |Get[umut this&] -> imut ULang:Empty&;//ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&imut
 |Write[this&, imut ULang:Empty&] -> void;//ULang:Ptr<ULang:Empty>:Write^ULang:Ptr<ULang:Empty>&,ULang:Empty&imut
 |Write[this&, moved ULang:Empty] -> void;//ULang:Ptr<ULang:Empty>:Write^ULang:Ptr<ULang:Empty>&,ULang:Emptymov
 |ReadPtr[umut this&] -> imut ULang:Empty&;//ULang:Ptr<ULang:Empty>:ReadPtr^ULang:Ptr<ULang:Empty>&imut
 |(&IndirectMember&)[this&] -> ULang:Empty&;//ULang:Ptr<ULang:Empty>:(&IndirectMember&)^ULang:Ptr<ULang:Empty>&
 |(&IndirectMember&)[umut this&] -> imut ULang:Empty&;//ULang:Ptr<ULang:Empty>:(&IndirectMember&)^ULang:Ptr<ULang:Empty>&imut
 |ReassignTo[this&, ULang:Empty&] -> void;//ULang:Ptr<ULang:Empty>:ReassignTo^ULang:Ptr<ULang:Empty>&,ULang:Empty&
 |Make[imut ULang:Empty&] -> imut ULang:Ptr<ULang:Empty>;//ULang:Ptr<ULang:Empty>:Make^ULang:Empty&imut
 |Make[ULang:Empty&] -> ULang:Ptr<ULang:Empty>;//ULang:Ptr<ULang:Empty>:Make^ULang:Empty&


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
 |(&equal&)[umut this&, imut ULang:Vec2f_t<float32>&] -> bool;//ULang:Vec2f_t<float32>:(&equal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut
 |(&notequal&)[umut this&, imut ULang:Vec2f_t<float32>&] -> bool;//ULang:Vec2f_t<float32>:(&notequal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut
 |(&Compound_Plus&)[this&, imut ULang:Vec2f_t<float32>&] -> void;//ULang:Vec2f_t<float32>:(&Compound_Plus&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut
 |(&Compound_Sub&)[this&, imut ULang:Vec2f_t<float32>&] -> void;//ULang:Vec2f_t<float32>:(&Compound_Sub&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut
 |(&Plus&)[umut this&, imut ULang:Vec2f_t<float32>&] -> ULang:Vec2f_t<float32>;//ULang:Vec2f_t<float32>:(&Plus&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut
 |(&Minus&)[umut this&, imut ULang:Vec2f_t<float32>&] -> ULang:Vec2f_t<float32>;//ULang:Vec2f_t<float32>:(&Minus&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut


$ULang:Vec2f_t<sint32>:
.size:8
 sint32 X;//Offset 0
 sint32 Y;//Offset 4

 |(&_new&)[this&] -> void;//ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&
 |(&_new&)[this&, sint32, sint32] -> void;//ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&,i32,i32
 |(&equal&)[umut this&, imut ULang:Vec2f_t<sint32>&] -> bool;//ULang:Vec2f_t<sint32>:(&equal&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut
 |(&notequal&)[umut this&, imut ULang:Vec2f_t<sint32>&] -> bool;//ULang:Vec2f_t<sint32>:(&notequal&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut
 |(&Compound_Plus&)[this&, imut ULang:Vec2f_t<sint32>&] -> void;//ULang:Vec2f_t<sint32>:(&Compound_Plus&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut
 |(&Compound_Sub&)[this&, imut ULang:Vec2f_t<sint32>&] -> void;//ULang:Vec2f_t<sint32>:(&Compound_Sub&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut
 |(&Plus&)[umut this&, imut ULang:Vec2f_t<sint32>&] -> ULang:Vec2f_t<sint32>;//ULang:Vec2f_t<sint32>:(&Plus&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut
 |(&Minus&)[umut this&, imut ULang:Vec2f_t<sint32>&] -> ULang:Vec2f_t<sint32>;//ULang:Vec2f_t<sint32>:(&Minus&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut


$ULang:Vec3f_t<float32>:
.size:12
 float32 X;//Offset 0
 float32 Y;//Offset 4
 float32 Z;//Offset 8

 |(&_new&)[this&] -> void;//ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&
 |(&_new&)[this&, float32, float32, float32] -> void;//ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&,f32,f32,f32
 |(&equal&)[umut this&, ULang:Vec3f_t<float32>&] -> bool;//ULang:Vec3f_t<float32>:(&equal&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&
 |(&notequal&)[umut this&, ULang:Vec3f_t<float32>&] -> bool;//ULang:Vec3f_t<float32>:(&notequal&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&
 |(&Compound_Plus&)[this&, imut ULang:Vec3f_t<float32>&] -> void;//ULang:Vec3f_t<float32>:(&Compound_Plus&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut
 |(&Compound_Sub&)[this&, imut ULang:Vec3f_t<float32>&] -> void;//ULang:Vec3f_t<float32>:(&Compound_Sub&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut
 |(&Plus&)[umut this&, imut ULang:Vec3f_t<float32>&] -> ULang:Vec3f_t<float32>;//ULang:Vec3f_t<float32>:(&Plus&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&imut
 |(&Minus&)[umut this&, imut ULang:Vec3f_t<float32>&] -> ULang:Vec3f_t<float32>;//ULang:Vec3f_t<float32>:(&Minus&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&imut


$ULang:Vec3f_t<sint32>:
.size:12
 sint32 X;//Offset 0
 sint32 Y;//Offset 4
 sint32 Z;//Offset 8

 |(&_new&)[this&] -> void;//ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&
 |(&_new&)[this&, sint32, sint32, sint32] -> void;//ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&,i32,i32,i32
 |(&equal&)[umut this&, ULang:Vec3f_t<sint32>&] -> bool;//ULang:Vec3f_t<sint32>:(&equal&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&
 |(&notequal&)[umut this&, ULang:Vec3f_t<sint32>&] -> bool;//ULang:Vec3f_t<sint32>:(&notequal&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&
 |(&Compound_Plus&)[this&, imut ULang:Vec3f_t<sint32>&] -> void;//ULang:Vec3f_t<sint32>:(&Compound_Plus&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut
 |(&Compound_Sub&)[this&, imut ULang:Vec3f_t<sint32>&] -> void;//ULang:Vec3f_t<sint32>:(&Compound_Sub&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut
 |(&Plus&)[umut this&, imut ULang:Vec3f_t<sint32>&] -> ULang:Vec3f_t<sint32>;//ULang:Vec3f_t<sint32>:(&Plus&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&imut
 |(&Minus&)[umut this&, imut ULang:Vec3f_t<sint32>&] -> ULang:Vec3f_t<sint32>;//ULang:Vec3f_t<sint32>:(&Minus&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&imut


$ULang:Ptr<sint32>:
.size:8
 sint32& _base;//Offset 0

 |(&_new&)[this&, sint32&] -> void;//ULang:Ptr<sint32>:(&_new&)^ULang:Ptr<sint32>&,i32&
 |Get[this&] -> sint32&;//ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&
 |Get[umut this&] -> imut sint32&;//ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&imut
 |Write[this&, imut sint32&] -> void;//ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32&imut
 |Write[this&, moved sint32] -> void;//ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32mov
 |ReadPtr[umut this&] -> imut sint32&;//ULang:Ptr<sint32>:ReadPtr^ULang:Ptr<sint32>&imut
 |(&IndirectMember&)[this&] -> sint32&;//ULang:Ptr<sint32>:(&IndirectMember&)^ULang:Ptr<sint32>&
 |(&IndirectMember&)[umut this&] -> imut sint32&;//ULang:Ptr<sint32>:(&IndirectMember&)^ULang:Ptr<sint32>&imut
 |ReassignTo[this&, sint32&] -> void;//ULang:Ptr<sint32>:ReassignTo^ULang:Ptr<sint32>&,i32&
 |Make[imut sint32&] -> imut ULang:Ptr<sint32>;//ULang:Ptr<sint32>:Make^i32&imut
 |Make[sint32&] -> ULang:Ptr<sint32>;//ULang:Ptr<sint32>:Make^i32&


$ULang:Ptr<ULang:Vec2f_t<float32>>:
.size:8
 ULang:Vec2f_t<float32>& _base;//Offset 0

 |(&_new&)[this&, ULang:Vec2f_t<float32>&] -> void;//ULang:Ptr<ULang:Vec2f_t<float32>>:(&_new&)^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&
 |Get[this&] -> ULang:Vec2f_t<float32>&;//ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&
 |Get[umut this&] -> imut ULang:Vec2f_t<float32>&;//ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&imut
 |Write[this&, imut ULang:Vec2f_t<float32>&] -> void;//ULang:Ptr<ULang:Vec2f_t<float32>>:Write^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&imut
 |Write[this&, moved ULang:Vec2f_t<float32>] -> void;//ULang:Ptr<ULang:Vec2f_t<float32>>:Write^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>mov
 |ReadPtr[umut this&] -> imut ULang:Vec2f_t<float32>&;//ULang:Ptr<ULang:Vec2f_t<float32>>:ReadPtr^ULang:Ptr<ULang:Vec2f_t<float32>>&imut
 |(&IndirectMember&)[this&] -> ULang:Vec2f_t<float32>&;//ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&
 |(&IndirectMember&)[umut this&] -> imut ULang:Vec2f_t<float32>&;//ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&imut
 |ReassignTo[this&, ULang:Vec2f_t<float32>&] -> void;//ULang:Ptr<ULang:Vec2f_t<float32>>:ReassignTo^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&
 |Make[imut ULang:Vec2f_t<float32>&] -> imut ULang:Ptr<ULang:Vec2f_t<float32>>;//ULang:Ptr<ULang:Vec2f_t<float32>>:Make^ULang:Vec2f_t<float32>&imut
 |Make[ULang:Vec2f_t<float32>&] -> ULang:Ptr<ULang:Vec2f_t<float32>>;//ULang:Ptr<ULang:Vec2f_t<float32>>:Make^ULang:Vec2f_t<float32>&



[Instructions:UCodeVm]-- 
---main: 
   0 :Debug_FuncStart 
   1 :Store32 [A], 0
   3 :StoreRegToReg32 [A],[F]
   4 :Debug_LineEnter 
   5 :Debug_FuncEnd 
   6 :Ret 
---ULang:Compiler:Is64Bit: 
   7 :Debug_FuncStart 
   8 :Store8 [A],1
   9 :StoreRegToReg8 [A],[F]
   10 :Debug_LineEnter 
   11 :Debug_FuncEnd 
   12 :Ret 
---ULang:Compiler:Is32Bit: 
   13 :Debug_FuncStart 
   14 :Store8 [A],1
   15 :StoreRegToReg8 [A],[F]
   16 :Debug_LineEnter 
   17 :Debug_FuncEnd 
   18 :Ret 
---ULang:Compiler:IsBackendC89: 
   19 :Debug_FuncStart 
   20 :Store8 [A],1
   21 :StoreRegToReg8 [A],[F]
   22 :Debug_LineEnter 
   23 :Debug_FuncEnd 
   24 :Ret 
---ULang:Compiler:IsBackendUCodeVM: 
   25 :Debug_FuncStart 
   26 :Store8 [A],1
   27 :StoreRegToReg8 [A],[F]
   28 :Debug_LineEnter 
   29 :Debug_FuncEnd 
   30 :Ret 
---ULang:Compiler:IsBackendWasm: 
   31 :Debug_FuncStart 
   32 :Store8 [A],1
   33 :StoreRegToReg8 [A],[F]
   34 :Debug_LineEnter 
   35 :Debug_FuncEnd 
   36 :Ret 
---ULang:Compiler:IsBackendVM: 
   37 :Debug_FuncStart 
   38 :Store8 [A],1
   39 :StoreRegToReg8 [A],[F]
   40 :Debug_LineEnter 
   41 :Debug_FuncEnd 
   42 :Ret 
---ULang:Compiler:(&_new&)^ULang:Compiler&: 
   43 :Debug_FuncStart 
   44 :Debug_FuncEnd 
   45 :Ret 
---ULang:Empty:(&_new&)^ULang:Empty&: 
   46 :Debug_FuncStart 
   47 :Debug_FuncEnd 
   48 :Ret 
---ULang:LowLevel:Malloc^uptr: 
   49 :Debug_FuncStart 
   50 :Debug_LineEnter 
   51 :Call {ULang:Native:OS_Malloc^uptr}
   55 :Debug_FuncEnd 
   56 :Ret 
---ULang:LowLevel:Free^u8[&]: 
   57 :Debug_FuncStart 
   58 :Debug_LineEnter 

   //File:/Main.uc
   //Line:3
   //Source Line:[Cant Find Line]

   59 :Call {ULang:Native:OS_Free^u8[&]}
   63 :Debug_FuncEnd 
   64 :Ret 
---ULang:LowLevel:Memcopy^u8[&],u8[&],uptr: 

   //File:/Helper/Compiler.uc
   //Line:3
   //Source Line:[Cant Find Line]

   65 :Debug_FuncStart 
   66 :Debug_FuncEnd 
   67 :Ret 
---ULang:LowLevel:Memmove^u8[&],u8[&],uptr: 
   68 :Debug_FuncStart 
   69 :Debug_FuncEnd 
   70 :Ret 
---ULang:LowLevel:(&_new&)^ULang:LowLevel&: 

   //File:/Helper/Compiler.uc
   //Line:4
   //Source Line:[Cant Find Line]

   71 :Debug_FuncStart 
   72 :Debug_FuncEnd 
   73 :Ret 
---ULang:Math:(&_new&)^ULang:Math&: 
   74 :Debug_FuncStart 
   75 :Debug_FuncEnd 
   76 :Ret 
---ULang:Fmt:Print^c: 

   //File:/Helper/Compiler.uc
   //Line:6
   //Source Line:[Cant Find Line]

   77 :Debug_FuncStart 
   78 :Debug_LineEnter 
   79 :Call {ULang:Native:OS_PrintChar^c}

   //File:/Helper/Compiler.uc
   //Line:7
   //Source Line:[Cant Find Line]

   83 :Debug_FuncEnd 
   84 :Ret 
---ULang:Fmt:Print^ULang:StringSpan_t<char>imut: 
   85 :Debug_FuncStart 
   86 :GetPointerOfStackSub [A],24
   87 :StoreRegToReg64 [A],[D]
   88 :Debug_LineEnter 

   //File:/Helper/Compiler.uc
   //Line:8
   //Source Line:[Cant Find Line]

   89 :Call {ULang:StringSpan_t<char>:Data^ULang:StringSpan_t<char>&imut}
   93 :GetPointerOfStackSub [A],24
   94 :StoreRegToReg64 [A],[D]

   //File:/Helper/Compiler.uc
   //Line:10
   //Source Line:[Cant Find Line]

   95 :StoreRegToReg64 [F],[B]
   96 :Push64 [B]
   97 :Call {ULang:StringSpan_t<char>:Size^ULang:StringSpan_t<char>&imut}
   101 :Pop64 [B]
   102 :StoreRegToReg64 [B],[D]
   103 :StoreRegToReg64 [F],[E]
   104 :Call {ULang:Native:OS_PrintSpan^c[&]imut,uptr}
   108 :Debug_FuncEnd 
   109 :Ret 
---ULang:Fmt:Print^ULang:String_t<char>&imut: 
   110 :Debug_FuncStart 
   111 :Debug_FuncEnd 
   112 :Ret 
---ULang:Fmt:Println^c: 
   113 :Debug_FuncStart 

   //File:/Helper/LowLevel.uc
   //Line:4
   //Source Line:[Cant Find Line]

   114 :Debug_LineEnter 
   115 :Call {ULang:Fmt:Print^c}
   119 :Store8 [A],10
   120 :StoreRegToReg8 [A],[D]
   121 :Debug_LineEnter 
   122 :Call {ULang:Fmt:Print^c}
   126 :Debug_FuncEnd 
   127 :Ret 
---ULang:Fmt:Println^ULang:StringSpan_t<char>imut: 
   128 :Debug_FuncStart 
   129 :GetFromStackSub64 [A],24
   130 :StoreRegOnStackSub64 [A],24
   131 :GetFromStackSub64 [A],32
   132 :StoreRegOnStackSub64 [A],32
   133 :GetFromStackSub64 [B],24

   //File:/IO/Console/Fmt.uc
   //Line:5
   //Source Line:[Cant Find Line]

   134 :Push64 [B]
   135 :GetFromStackSub64 [B],24
   136 :Push64 [B]
   137 :Debug_LineEnter 
   138 :Call {ULang:Fmt:Print^ULang:StringSpan_t<char>imut}
   142 :Pop64 [A]

   //File:/IO/Console/Fmt.uc
   //Line:9
   //Source Line:[Cant Find Line]

   143 :Pop64 [A]
   144 :Store8 [A],10
   145 :StoreRegToReg8 [A],[D]
   146 :Debug_LineEnter 
   147 :Call {ULang:Fmt:Print^c}
   151 :Debug_FuncEnd 
   152 :Ret 
---ULang:Fmt:Println^ULang:String_t<char>&imut: 
   153 :Debug_FuncStart 
   154 :Debug_LineEnter 
   155 :Call {ULang:Fmt:Print^ULang:String_t<char>&imut}
   159 :Store8 [A],10
   160 :StoreRegToReg8 [A],[D]
   161 :Debug_LineEnter 
   162 :Call {ULang:Fmt:Print^c}
   166 :Debug_FuncEnd 
   167 :Ret 
---ULang:Fmt:(&_new&)^ULang:Fmt&: 
   168 :Debug_FuncStart 
   169 :Debug_FuncEnd 

   //File:/IO/Console/Fmt.uc
   //Line:14
   //Source Line:[Cant Find Line]

   170 :Ret 
---ULang:OpenedFile:(&_new&)^ULang:OpenedFile&: 
   171 :Debug_FuncStart 
   172 :Debug_FuncEnd 
   173 :Ret 
---ULang:OpenFileError:(&_new&)^ULang:OpenFileError&: 
   174 :Debug_FuncStart 
   175 :Debug_FuncEnd 

   //Line:15
   //Source Line:[Cant Find Line]

   176 :Ret 
---ULang:File:Open^ULang:StringSpan_t<char>: 
   177 :Debug_FuncStart 
   178 :Debug_FuncEnd 
   179 :Ret 
---ULang:File:Open^ULang:String_t<char>&imut: 
   180 :Debug_FuncStart 
   181 :Debug_FuncEnd 
   182 :Ret 
---ULang:File:GetString^ULang:StringSpan_t<char>: 
   183 :Debug_FuncStart 
   184 :Debug_FuncEnd 
   185 :Ret 
---ULang:File:GetString^ULang:String_t<char>&imut: 
   186 :Debug_FuncStart 
   187 :Debug_FuncEnd 
   188 :Ret 
---ULang:File:GetBytes^ULang:StringSpan_t<char>: 

   //File:/IO/Console/Fmt.uc
   //Line:18
   //Source Line:[Cant Find Line]

   189 :Debug_FuncStart 
   190 :Debug_FuncEnd 
   191 :Ret 
---ULang:File:GetBytes^ULang:String_t<char>&imut: 
   192 :Debug_FuncStart 
   193 :Debug_FuncEnd 
   194 :Ret 
---ULang:File:(&_new&)^ULang:File&: 
   195 :Debug_FuncStart 
   196 :Debug_FuncEnd 
   197 :Ret 
---ULang:Writer:(&_new&)^ULang:Writer&: 
   198 :Debug_FuncStart 
   199 :Debug_FuncEnd 
   200 :Ret 
---ULang:Tests:Print1: 

   //Line:19
   //Source Line:[Cant Find Line]

   201 :Store64 [A], 16
   205 :IncrementStackPointer [A]
   206 :Debug_FuncStart 
   207 :GetPointerOfStaticMem [A],0
   208 :Store64 [B], 11
   212 :Debug_LineEnter 
   213 :GetPointerOfStackSub [C],16
   214 :StoreRegToReg64 [C],[D]
   215 :StoreRegToReg64 [A],[E]

   //Line:23
   //Source Line:[Cant Find Line]

   216 :StoreRegToReg64 [B],[F]
   217 :Call {ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&,c[&],uptr}
   221 :GetFromStackSub64 [A],16
   222 :Push64 [A]
   223 :GetFromStackSub64 [A],16
   224 :Push64 [A]
   225 :Call {ULang:Fmt:Println^ULang:StringSpan_t<char>imut}
   229 :Pop64 [A]
   230 :Pop64 [A]
   231 :Store8 [A],1
   232 :StoreRegToReg8 [A],[F]
   233 :Debug_LineEnter 
   234 :Store64 [B], 16
   238 :DecrementStackPointer [B]
   239 :Debug_FuncEnd 
   240 :Ret 
---ULang:Tests:Print2: 
   241 :Debug_FuncStart 
   242 :Store8 [A],72
   243 :StoreRegToReg8 [A],[D]
   244 :Debug_LineEnter 
   245 :Call {ULang:Fmt:Print^c}
   249 :Store8 [A],105
   250 :StoreRegToReg8 [A],[D]
   251 :Debug_LineEnter 
   252 :Call {ULang:Fmt:Println^c}
   256 :Store8 [A],1
   257 :StoreRegToReg8 [A],[F]
   258 :Debug_LineEnter 

   //File:/Tests/IOTest.uc
   //Line:7
   //Source Line:[Cant Find Line]

   259 :Debug_FuncEnd 
   260 :Ret 
---ULang:Tests:Max_1: 
   261 :Debug_FuncStart 
   262 :Store32 [A], 5
   264 :Debug_LineEnter 
   265 :Store32 [B], 2
   267 :StoreRegToReg32 [A],[D]
   268 :StoreRegToReg32 [B],[E]
   269 :Call {ULang:Math:Max<sint32>^i32,i32}
   273 :Store32 [A], 5
   275 :equalto8 [F],[A],[F]
   276 :Debug_FuncEnd 
   277 :Ret 
---ULang:Tests:Max_2: 
   278 :Debug_FuncStart 
   279 :Store32 [A], 5
   281 :Store32 [B], 2

   //Line:8
   //Source Line:[Cant Find Line]

   283 :Debug_LineEnter 
   284 :StoreRegToReg32 [B],[D]
   285 :StoreRegToReg32 [A],[E]
   286 :Call {ULang:Math:Max<sint32>^i32,i32}
   290 :Store32 [A], 5
   292 :equalto8 [F],[A],[F]
   293 :Debug_FuncEnd 
   294 :Ret 
---ULang:Tests:Min_1: 
   295 :Debug_FuncStart 
   296 :Store32 [A], 2
   298 :Store32 [B], 5
   300 :Debug_LineEnter 
   301 :StoreRegToReg32 [B],[D]
   302 :StoreRegToReg32 [A],[E]
   303 :Call {ULang:Math:Min<sint32>^i32,i32}
   307 :Store32 [A], 2
   309 :equalto8 [F],[A],[F]
   310 :Debug_FuncEnd 

   //Line:14
   //Source Line:[Cant Find Line]

   311 :Ret 
---ULang:Tests:Min_2: 
   312 :Debug_FuncStart 
   313 :Store32 [A], 2
   315 :Debug_LineEnter 
   316 :Store32 [B], 5

   //File:/Tests/Math.uc
   //Line:7
   //Source Line:[Cant Find Line]

   318 :StoreRegToReg32 [A],[D]
   319 :StoreRegToReg32 [B],[E]
   320 :Call {ULang:Math:Min<sint32>^i32,i32}
   324 :Store32 [A], 2
   326 :equalto8 [F],[A],[F]
   327 :Debug_FuncEnd 
   328 :Ret 
---ULang:Future:(&_new&)^ULang:Future&: 
   329 :Debug_FuncStart 
   330 :Debug_FuncEnd 
   331 :Ret 
---ULang:Thread:(&_new&)^ULang:Thread&: 
   332 :Debug_FuncStart 
   333 :Debug_FuncEnd 
   334 :Ret 
---ULang:Path:(&_new&)^ULang:Path&: 

   //File:/Tests/Math.uc
   //Line:11
   //Source Line:[Cant Find Line]

   335 :Debug_FuncStart 
   336 :Debug_FuncEnd 
   337 :Ret 
---ULang:Tests:Ptr_1: 
   338 :Store64 [A], 12
   342 :IncrementStackPointer [A]
   343 :Debug_FuncStart 
   344 :Store32 [A], 5
   346 :Debug_LineEnter 
   347 :StoreRegToReg32 [A],[C]
   348 :StoreRegToReg32 [C],[B]
   349 :StoreRegToReg32 [B],[D]
   350 :StoreRegOnStackSub32 [D],12
   351 :GetPointerOfStackSub [D],12

   //File:/Tests/Math.uc
   //Line:15
   //Source Line:[Cant Find Line]

   352 :Debug_LineEnter 
   353 :Call {ULang:ptr<sint32>^i32&}
   357 :StoreRegToReg64 [F],[A]
   358 :StoreRegOnStackSub64 [A],8
   359 :Store32 [B], 10
   361 :GetPointerOfStackSub [C],8
   362 :Debug_LineEnter 
   363 :StoreRegToReg64 [C],[D]
   364 :Call {ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&}
   368 :Store32 [A], 10
   370 :StoreRegToPtr32 [A],[F]
   371 :Debug_LineEnter 
   372 :equalto8 [B],[A],[F]
   373 :Store64 [C], 12
   377 :DecrementStackPointer [C]
   378 :Debug_FuncEnd 
   379 :Ret 
---ULang:Tests:Ptr_2: 
   380 :Store64 [A], 12
   384 :IncrementStackPointer [A]
   385 :Debug_FuncStart 
   386 :Store32 [A], 5
   388 :Debug_LineEnter 
   389 :StoreRegToReg32 [A],[C]

   //StackFrameSize:12

   390 :StoreRegToReg32 [C],[B]
   391 :StoreRegToReg32 [B],[D]
   392 :StoreRegOnStackSub32 [D],12

   //File:/Tests/PtrTest.uc
   //Line:6
   //Source Line:[Cant Find Line]

   393 :GetPointerOfStackSub [D],12
   394 :Debug_LineEnter 
   395 :Call {ULang:ptr<sint32>^i32&}
   399 :StoreRegToReg64 [F],[A]
   400 :StoreRegOnStackSub64 [A],8
   401 :GetPointerOfStackSub [B],8
   402 :Store32 [C], 10
   404 :Debug_LineEnter 
   405 :StoreRegToReg64 [B],[D]
   406 :StoreRegToReg32 [C],[E]
   407 :Call {ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32&imut}
   411 :Store32 [A], 10
   413 :Debug_LineEnter 
   414 :equalto8 [B],[A],[F]
   415 :Store64 [C], 12
   419 :DecrementStackPointer [C]

   //Line:12
   //Source Line:[Cant Find Line]

   420 :Debug_FuncEnd 
   421 :Ret 
---ULang:Tests:Ptr_3: 
   422 :Store64 [A], 16
   426 :IncrementStackPointer [A]
   427 :Debug_FuncStart 
   428 :Store32 [A], 5

   //StackFrameSize:12

   430 :Debug_LineEnter 
   431 :StoreRegToReg32 [A],[C]
   432 :StoreRegToReg32 [C],[B]

   //File:/Tests/PtrTest.uc
   //Line:16
   //Source Line:[Cant Find Line]

   433 :Store32 [D], 8
   435 :Debug_LineEnter 
   436 :StoreRegToReg32 [D],[F]
   437 :StoreRegToReg32 [F],[E]

   //Line:17
   //Source Line:[Cant Find Line]

   438 :StoreRegToReg32 [B],[A]
   439 :StoreRegOnStackSub32 [A],16
   440 :GetPointerOfStackSub [A],16
   441 :Debug_LineEnter 
   442 :StoreRegToReg64 [A],[D]
   443 :Push32 [E]
   444 :Call {ULang:ptr<sint32>^i32&}
   448 :Pop32 [E]

   //Line:19
   //Source Line:[Cant Find Line]

   449 :StoreRegToReg64 [F],[A]
   450 :StoreRegOnStackSub64 [A],12
   451 :GetPointerOfStackSub [B],12
   452 :StoreRegToReg32 [E],[C]
   453 :StoreRegOnStackSub32 [C],4
   454 :GetPointerOfStackSub [C],4
   455 :Debug_LineEnter 
   456 :StoreRegToReg64 [B],[D]
   457 :StoreRegToReg64 [C],[E]
   458 :Call {ULang:Ptr<sint32>:ReassignTo^ULang:Ptr<sint32>&,i32&}
   462 :GetPointerOfStackSub [A],12
   463 :Store32 [B], 7
   465 :Debug_LineEnter 
   466 :StoreRegToReg64 [A],[D]
   467 :StoreRegToReg32 [B],[E]
   468 :Call {ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32&imut}
   472 :Store32 [A], 7
   474 :Debug_LineEnter 
   475 :equalto8 [B],[A],[F]
   476 :Store64 [C], 16
   480 :DecrementStackPointer [C]
   481 :Debug_FuncEnd 
   482 :Ret 
---ULang:Tests:Ptr_4: 

   //Line:29
   //Source Line:[Cant Find Line]

   483 :Store64 [A], 12
   487 :IncrementStackPointer [A]
   488 :Debug_FuncStart 
   489 :Store32 [A], 5
   491 :Debug_LineEnter 
   492 :StoreRegToReg32 [A],[C]
   493 :StoreRegToReg32 [C],[B]
   494 :StoreRegToReg32 [B],[D]
   495 :StoreRegOnStackSub32 [D],12
   496 :GetPointerOfStackSub [D],12

   //Line:31
   //Source Line:[Cant Find Line]

   497 :Debug_LineEnter 
   498 :Call {ULang:ptr<sint32>^i32&}
   502 :StoreRegToReg64 [F],[A]
   503 :StoreRegOnStackSub64 [A],8
   504 :Store32 [C], 5
   506 :Debug_LineEnter 
   507 :Add32 [B],[C],[F]

   //Line:33
   //Source Line:[Cant Find Line]

   508 :StoreRegToReg32 [F],[E]
   509 :StoreRegToReg32 [E],[B]
   510 :Store32 [B], 10
   512 :GetPointerOfStackSub [C],8
   513 :Debug_LineEnter 
   514 :StoreRegToReg64 [C],[D]
   515 :Push32 [B]
   516 :Call {ULang:Ptr<sint32>:ReadPtr^ULang:Ptr<sint32>&imut}
   520 :Pop32 [B]
   521 :StoreFromPtrToReg64 [F],[A]
   522 :equalto8 [A],[B],[F]
   523 :Store64 [C], 12
   527 :DecrementStackPointer [C]
   528 :Debug_FuncEnd 

   //StackFrameSize:12

   529 :Ret 
---ULang:Tests:Ptr_5: 
   530 :Store64 [A], 24
   534 :IncrementStackPointer [A]
   535 :Debug_FuncStart 
   536 :GetPointerOfStackSub [A],24

   //Line:40
   //Source Line:[Cant Find Line]

   537 :Debug_LineEnter 
   538 :Storef32 [B], 8.000000
   540 :Debug_LineEnter 
   541 :Storef32 [C], 6.000000
   543 :Debug_LineEnter 
   544 :StoreRegToReg64 [A],[D]
   545 :StoreRegToReg32 [B],[E]
   546 :StoreRegToReg32 [C],[F]

   //Line:42
   //Source Line:[Cant Find Line]

   547 :Call {ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&,f32,f32}
   551 :GetPointerOfStackSub [A],24
   552 :Debug_LineEnter 
   553 :StoreRegToReg64 [A],[D]
   554 :Call {ULang:ptr<ULang:Vec2f_t<float32>>^ULang:Vec2f_t<float32>&}
   558 :StoreRegToReg64 [F],[A]
   559 :StoreRegOnStackSub64 [A],16
   560 :Storef32 [B], 5.000000
   562 :GetFromStackSub64 [C],16
   563 :Push64 [C]
   564 :Debug_LineEnter 
   565 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&}
   569 :Debug_LineEnter 
   570 :Storef32 [A], 5.000000
   572 :StoreRegToPtr32 [A],[F]
   573 :Storef32 [B], 8.000000
   575 :GetFromStackSub64 [C],24
   576 :Push64 [C]

   //File:/Tests/PtrTest.uc
   //Line:50
   //Source Line:[Cant Find Line]

   577 :Debug_LineEnter 
   578 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&}

   //Line:50
   //Source Line:[Cant Find Line]

   582 :Debug_LineEnter 
   583 :Storef32 [A], 8.000000
   585 :Load32 [F],[A],4
   586 :GetPointerOfStackSub [B],24
   587 :Debug_LineEnter 
   588 :GetPointerOfStackSub [C],8
   589 :Storef32 [E], 5.000000
   591 :StoreRegToReg64 [C],[D]

   //Line:51
   //Source Line:[Cant Find Line]

   592 :StoreRegToReg32 [A],[F]
   593 :Push64 [B]
   594 :Call {ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&,f32,f32}
   598 :Pop64 [B]
   599 :GetPointerOfStackSub [A],8
   600 :StoreRegToReg64 [B],[D]
   601 :StoreRegToReg64 [A],[E]
   602 :Call {ULang:Vec2f_t<float32>:(&equal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut}

   //Line:0
   //Source Line:[Cant Find Line]

   606 :Store64 [A], 24
   610 :DecrementStackPointer [A]
   611 :Debug_FuncEnd 
   612 :Ret 
---ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&: 
   613 :Debug_FuncStart 
   614 :Store64 [A], 0
   618 :Debug_LineEnter 

   //Line:0
   //Source Line:[Cant Find Line]

   619 :StoreRegToPtr64 [A],[D]
   620 :Debug_LineEnter 
   621 :Load64 [D],[A],8
   622 :Debug_FuncEnd 
   623 :Ret 
---ULang:StringSpan_t<char>:(&_new&)^ULang:StringSpan_t<char>&,c[&],uptr: 
   624 :Debug_FuncStart 
   625 :Debug_LineEnter 
   626 :StoreRegToPtr64 [E],[D]
   627 :Debug_LineEnter 

   //Line:57
   //Source Line:[Cant Find Line]

   628 :Load64 [D],[F],8
   629 :Debug_FuncEnd 
   630 :Ret 
---ULang:StringSpan_t<char>:Size^ULang:StringSpan_t<char>&imut: 
   631 :Debug_FuncStart 
   632 :Read64 [D],[A],8
   633 :StoreRegToReg64 [A],[F]
   634 :Debug_LineEnter 
   635 :Debug_FuncEnd 
   636 :Ret 
---ULang:StringSpan_t<char>:Data^ULang:StringSpan_t<char>&imut: 
   637 :Debug_FuncStart 
   638 :StoreFromPtrToReg64 [D],[A]
   639 :StoreRegToReg64 [A],[F]
   640 :Debug_LineEnter 
   641 :Debug_FuncEnd 
   642 :Ret 
---ULang:String_t<char>:(&_new&)^ULang:String_t<char>&: 
   643 :Debug_FuncStart 
   644 :Debug_FuncEnd 
   645 :Ret 
---ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&: 
   646 :Debug_FuncStart 
   647 :Store64 [A], 0
   651 :Debug_LineEnter 
   652 :StoreRegToPtr64 [A],[D]
   653 :Debug_LineEnter 
   654 :Load64 [D],[A],8
   655 :Debug_FuncEnd 
   656 :Ret 
---ULang:Span<uint8>:(&_new&)^ULang:Span<uint8>&,u8[&],uptr: 
   657 :Debug_FuncStart 
   658 :Debug_LineEnter 
   659 :StoreRegToPtr64 [E],[D]
   660 :Debug_LineEnter 
   661 :Load64 [D],[F],8

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   662 :Debug_FuncEnd 
   663 :Ret 
---ULang:Span<uint8>:Size^ULang:Span<uint8>&imut: 

   //Line:9
   //Source Line:[Cant Find Line]

   664 :Debug_FuncStart 
   665 :Read64 [D],[A],8
   666 :StoreRegToReg64 [A],[F]
   667 :Debug_LineEnter 
   668 :Debug_FuncEnd 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   669 :Ret 
---ULang:Span<uint8>:Data^ULang:Span<uint8>&imut: 
   670 :Debug_FuncStart 

   //Line:13
   //Source Line:[Cant Find Line]

   671 :StoreFromPtrToReg64 [D],[A]
   672 :StoreRegToReg64 [A],[F]
   673 :Debug_LineEnter 
   674 :Debug_FuncEnd 
   675 :Ret 
---ULang:Vector<uint8>:(&_new&)^ULang:Vector<uint8>&: 
   676 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   677 :Store64 [A], 0
   681 :Debug_LineEnter 
   682 :StoreRegToPtr64 [A],[D]

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   683 :Debug_LineEnter 
   684 :Load64 [D],[A],8
   685 :Debug_LineEnter 
   686 :Load64 [D],[A],16
   687 :Debug_FuncEnd 
   688 :Ret 
---ULang:Vector<uint8>:Push^u8&imut: 
   689 :Debug_FuncStart 
   690 :Store32 [A], 0
   692 :StoreRegToReg32 [A],[F]
   693 :Debug_LineEnter 
   694 :Debug_FuncEnd 

   //File:/Types/Span.uc
   //Line:5
   //Source Line:[Cant Find Line]

   695 :Ret 
---ULang:Vector<uint8>:Push^ULang:Span<uint8>imut: 
   696 :Debug_FuncStart 

   //Line:6
   //Source Line:[Cant Find Line]

   697 :Store32 [A], 0
   699 :StoreRegToReg32 [A],[F]
   700 :Debug_LineEnter 
   701 :Debug_FuncEnd 

   //File:/Types/Span.uc
   //Line:9
   //Source Line:[Cant Find Line]

   702 :Ret 
---ULang:Math:Max<sint32>^i32,i32: 
   703 :Debug_FuncStart 

   //Line:10
   //Source Line:[Cant Find Line]

   704 :Debug_LineEnter 
   705 :greaterthan8 [D],[E],[F]
   706 :LogicalNot8 [F],[A]
   707 :StoreRegToReg8 [A],[B]
   708 :Jumpif [B],{717}
   712 :StoreRegToReg32 [D],[F]
   713 :Jump {719}
   717 :StoreRegToReg32 [E],[F]
   718 :Debug_LineEnter 
   719 :Debug_FuncEnd 
   720 :Ret 
---ULang:Math:Min<sint32>^i32,i32: 
   721 :Debug_FuncStart 
   722 :Debug_LineEnter 
   723 :lessthan8 [D],[E],[F]
   724 :LogicalNot8 [F],[A]

   //File:/Types/Vector.uc
   //Line:8
   //Source Line:[Cant Find Line]

   725 :StoreRegToReg8 [A],[B]
   726 :Jumpif [B],{735}
   730 :StoreRegToReg32 [D],[F]
   731 :Jump {737}
   735 :StoreRegToReg32 [E],[F]

   //File:/Types/Vector.uc
   //Line:12
   //Source Line:[Cant Find Line]

   736 :Debug_LineEnter 
   737 :Debug_FuncEnd 
   738 :Ret 
---ULang:Ptr<ULang:Empty>:(&_new&)^ULang:Ptr<ULang:Empty>&,ULang:Empty&: 
   739 :Debug_FuncStart 

   //[A] = ULang:Vector<uint8>:Push:Val

   740 :Debug_LineEnter 
   741 :StoreRegToPtr64 [E],[D]
   742 :Debug_FuncEnd 

   //File:/Types/Vector.uc
   //Line:15
   //Source Line:[Cant Find Line]

   743 :Ret 
---ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&: 
   744 :Debug_FuncStart 
   745 :StoreFromPtrToReg64 [D],[A]
   746 :StoreRegToReg64 [A],[F]

   //[A] = ULang:Vector<uint8>:Push:Val

   747 :Debug_LineEnter 

   //File:/Helper/Math.uc
   //Line:6
   //Source Line:[Cant Find Line]

   748 :Debug_FuncEnd 
   749 :Ret 
---ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&imut: 
   750 :Debug_FuncStart 
   751 :StoreFromPtrToReg64 [D],[A]
   752 :StoreRegToReg64 [A],[F]
   753 :Debug_LineEnter 
   754 :Debug_FuncEnd 
   755 :Ret 
---ULang:Ptr<ULang:Empty>:Write^ULang:Ptr<ULang:Empty>&,ULang:Empty&imut: 
   756 :Debug_FuncStart 
   757 :Debug_FuncEnd 
   758 :Ret 
---ULang:Ptr<ULang:Empty>:Write^ULang:Ptr<ULang:Empty>&,ULang:Emptymov: 
   759 :Debug_FuncStart 
   760 :Debug_FuncEnd 

   //Line:7
   //Source Line:[Cant Find Line]

   761 :Ret 
---ULang:Ptr<ULang:Empty>:ReadPtr^ULang:Ptr<ULang:Empty>&imut: 
   762 :Debug_FuncStart 
   763 :Debug_LineEnter 
   764 :Call {ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&imut}
   768 :Debug_FuncEnd 
   769 :Ret 
---ULang:Ptr<ULang:Empty>:(&IndirectMember&)^ULang:Ptr<ULang:Empty>&: 
   770 :Debug_FuncStart 
   771 :Debug_LineEnter 
   772 :Call {ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&}
   776 :Debug_FuncEnd 
   777 :Ret 
---ULang:Ptr<ULang:Empty>:(&IndirectMember&)^ULang:Ptr<ULang:Empty>&imut: 
   778 :Debug_FuncStart 

   //Line:11
   //Source Line:[Cant Find Line]

   779 :Debug_LineEnter 
   780 :Call {ULang:Ptr<ULang:Empty>:Get^ULang:Ptr<ULang:Empty>&imut}

   //File:/Types/Ptr.uc
   //Line:12
   //Source Line:[Cant Find Line]

   784 :Debug_FuncEnd 
   785 :Ret 
---ULang:Ptr<ULang:Empty>:ReassignTo^ULang:Ptr<ULang:Empty>&,ULang:Empty&: 
   786 :Debug_FuncStart 
   787 :Debug_LineEnter 
   788 :StoreRegToPtr64 [E],[D]
   789 :Debug_FuncEnd 

   //File:/Types/Ptr.uc
   //Line:14
   //Source Line:[Cant Find Line]

   790 :Ret 
---ULang:Ptr<ULang:Empty>:Make^ULang:Empty&imut: 
   791 :Store64 [A], 32
   795 :IncrementStackPointer [A]

   //File:/Types/Ptr.uc
   //Line:15
   //Source Line:[Cant Find Line]

   796 :Debug_FuncStart 
   797 :GetPointerOfStackSub [A],32
   798 :Debug_LineEnter 
   799 :StoreFromPtrToReg64 [D],[B]
   800 :StoreRegOnStackSub64 [B],24
   801 :StoreFromPtrToReg64 [D],[C]
   802 :StoreRegOnStackSub64 [C],16
   803 :StoreFromPtrToReg64 [D],[E]
   804 :StoreRegOnStackSub64 [E],8
   805 :GetFromStackSub64 [F],8
   806 :StoreRegOnStackSub64 [F],16

   //File:/Types/Ptr.uc
   //Line:20
   //Source Line:[Cant Find Line]

   807 :StoreRegToReg64 [A],[D]
   808 :Debug_LineEnter 
   809 :Call {ULang:Ptr<ULang:Empty>:(&_new&)^ULang:Ptr<ULang:Empty>&,ULang:Empty&}
   813 :GetFromStackSub64 [F],32
   814 :Store64 [A], 32
   818 :DecrementStackPointer [A]
   819 :Debug_FuncEnd 
   820 :Ret 
---ULang:Ptr<ULang:Empty>:Make^ULang:Empty&: 
   821 :Store64 [A], 8
   825 :IncrementStackPointer [A]
   826 :Debug_FuncStart 
   827 :GetPointerOfStackSub [A],8
   828 :Debug_LineEnter 
   829 :StoreRegToReg64 [A],[D]
   830 :Debug_LineEnter 

   //File:/Types/Ptr.uc
   //Line:26
   //Source Line:[Cant Find Line]

   831 :StoreRegToReg64 [D],[E]
   832 :Call {ULang:Ptr<ULang:Empty>:(&_new&)^ULang:Ptr<ULang:Empty>&,ULang:Empty&}

   //File:/Types/Ptr.uc
   //Line:0
   //Source Line:[Cant Find Line]

   836 :GetFromStackSub64 [F],8
   837 :Store64 [A], 8
   841 :DecrementStackPointer [A]
   842 :Debug_FuncEnd 
   843 :Ret 
---ULang:Ptr<ULang:Empty>:(&_new&)^ULang:Ptr<ULang:Empty>&: 
   844 :Debug_FuncStart 
   845 :Debug_FuncEnd 

   //Line:30
   //Source Line:[Cant Find Line]

   846 :Ret 
---ULang:String_t<uft8>:(&_new&)^ULang:String_t<uft8>&: 
   847 :Debug_FuncStart 
   848 :Debug_FuncEnd 
   849 :Ret 
---ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&: 
   850 :Debug_FuncStart 
   851 :Store64 [A], 0
   855 :Debug_LineEnter 
   856 :StoreRegToPtr64 [A],[D]
   857 :Debug_LineEnter 
   858 :Load64 [D],[A],8
   859 :Debug_FuncEnd 
   860 :Ret 
---ULang:StringSpan_t<uft8>:(&_new&)^ULang:StringSpan_t<uft8>&,[&],uptr: 

   //StackFrameSize:8

   861 :Debug_FuncStart 
   862 :Debug_LineEnter 
   863 :StoreRegToPtr64 [E],[D]

   //File:/Types/Ptr.uc
   //Line:0
   //Source Line:[Cant Find Line]

   864 :Debug_LineEnter 
   865 :Load64 [D],[F],8

   //Line:31
   //Source Line:[Cant Find Line]

   866 :Debug_FuncEnd 
   867 :Ret 
---ULang:StringSpan_t<uft8>:Size^ULang:StringSpan_t<uft8>&imut: 
   868 :Debug_FuncStart 
   869 :Read64 [D],[A],8
   870 :StoreRegToReg64 [A],[F]
   871 :Debug_LineEnter 
   872 :Debug_FuncEnd 
   873 :Ret 
---ULang:StringSpan_t<uft8>:Data^ULang:StringSpan_t<uft8>&imut: 
   874 :Debug_FuncStart 
   875 :StoreFromPtrToReg64 [D],[A]
   876 :StoreRegToReg64 [A],[F]
   877 :Debug_LineEnter 
   878 :Debug_FuncEnd 
   879 :Ret 
---ULang:String_t<uft16>:(&_new&)^ULang:String_t<uft16>&: 
   880 :Debug_FuncStart 
   881 :Debug_FuncEnd 
   882 :Ret 
---ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&: 
   883 :Debug_FuncStart 
   884 :Store64 [A], 0
   888 :Debug_LineEnter 
   889 :StoreRegToPtr64 [A],[D]
   890 :Debug_LineEnter 
   891 :Load64 [D],[A],8
   892 :Debug_FuncEnd 
   893 :Ret 
---ULang:StringSpan_t<uft16>:(&_new&)^ULang:StringSpan_t<uft16>&,[&],uptr: 
   894 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   895 :Debug_LineEnter 
   896 :StoreRegToPtr64 [E],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   897 :Debug_LineEnter 
   898 :Load64 [D],[F],8
   899 :Debug_FuncEnd 
   900 :Ret 
---ULang:StringSpan_t<uft16>:Size^ULang:StringSpan_t<uft16>&imut: 
   901 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   902 :Read64 [D],[A],8
   903 :StoreRegToReg64 [A],[F]

   //Line:13
   //Source Line:[Cant Find Line]

   904 :Debug_LineEnter 
   905 :Debug_FuncEnd 
   906 :Ret 
---ULang:StringSpan_t<uft16>:Data^ULang:StringSpan_t<uft16>&imut: 
   907 :Debug_FuncStart 
   908 :StoreFromPtrToReg64 [D],[A]
   909 :StoreRegToReg64 [A],[F]

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   910 :Debug_LineEnter 
   911 :Debug_FuncEnd 
   912 :Ret 
---ULang:String_t<uft32>:(&_new&)^ULang:String_t<uft32>&: 
   913 :Debug_FuncStart 
   914 :Debug_FuncEnd 
   915 :Ret 
---ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&: 

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   916 :Debug_FuncStart 
   917 :Store64 [A], 0
   921 :Debug_LineEnter 
   922 :StoreRegToPtr64 [A],[D]
   923 :Debug_LineEnter 
   924 :Load64 [D],[A],8
   925 :Debug_FuncEnd 
   926 :Ret 
---ULang:StringSpan_t<uft32>:(&_new&)^ULang:StringSpan_t<uft32>&,[&],uptr: 
   927 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   928 :Debug_LineEnter 
   929 :StoreRegToPtr64 [E],[D]

   //Line:9
   //Source Line:[Cant Find Line]

   930 :Debug_LineEnter 
   931 :Load64 [D],[F],8
   932 :Debug_FuncEnd 
   933 :Ret 
---ULang:StringSpan_t<uft32>:Size^ULang:StringSpan_t<uft32>&imut: 
   934 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   935 :Read64 [D],[A],8
   936 :StoreRegToReg64 [A],[F]

   //Line:13
   //Source Line:[Cant Find Line]

   937 :Debug_LineEnter 
   938 :Debug_FuncEnd 
   939 :Ret 
---ULang:StringSpan_t<uft32>:Data^ULang:StringSpan_t<uft32>&imut: 
   940 :Debug_FuncStart 
   941 :StoreFromPtrToReg64 [D],[A]
   942 :StoreRegToReg64 [A],[F]

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   943 :Debug_LineEnter 
   944 :Debug_FuncEnd 
   945 :Ret 
---ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&: 
   946 :Debug_FuncStart 
   947 :Storef32 [A], 0.000000

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   949 :Debug_LineEnter 
   950 :StoreRegToPtr32 [A],[D]
   951 :Debug_LineEnter 
   952 :Load32 [D],[A],4
   953 :Debug_FuncEnd 
   954 :Ret 
---ULang:Vec2f_t<float32>:(&_new&)^ULang:Vec2f_t<float32>&,f32,f32: 
   955 :Debug_FuncStart 
   956 :Debug_LineEnter 
   957 :StoreRegToPtr32 [E],[D]
   958 :Debug_LineEnter 
   959 :Load32 [D],[F],4
   960 :Debug_FuncEnd 

   //File:/Types/String.uc
   //Line:8
   //Source Line:[Cant Find Line]

   961 :Ret 
---ULang:Vec2f_t<float32>:(&equal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut: 
   962 :Store64 [A], 1
   966 :IncrementStackPointer [A]
   967 :Debug_FuncStart 

   //File:/Types/String.uc
   //Line:12
   //Source Line:[Cant Find Line]

   968 :StoreFromPtrToReg32 [D],[A]
   969 :Debug_LineEnter 

   //Line:13
   //Source Line:[Cant Find Line]

   970 :StoreFromPtrToReg32 [E],[B]
   971 :equalto8 [B],[A],[F]
   972 :Read32 [E],[C],4
   973 :Read32 [D],[A],4
   974 :StoreRegOnStackSub8 [F],1
   975 :equalto8 [A],[C],[F]

   //File:/Types/String.uc
   //Line:15
   //Source Line:[Cant Find Line]

   976 :GetFromStackSub8 [A],1
   977 :Ins 38:5,0,5
   978 :Store64 [A], 1

   //File:/Types/String.uc
   //Line:16
   //Source Line:[Cant Find Line]

   982 :DecrementStackPointer [A]
   983 :Debug_FuncEnd 
   984 :Ret 
---ULang:Vec2f_t<float32>:(&notequal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut: 
   985 :Store64 [A], 24

   //File:/Types/Vec2.uc
   //Line:7
   //Source Line:[Cant Find Line]

   989 :IncrementStackPointer [A]
   990 :Debug_FuncStart 

   //Line:8
   //Source Line:[Cant Find Line]

   991 :StoreFromPtrToReg64 [D],[A]
   992 :StoreRegOnStackSub64 [A],24
   993 :StoreFromPtrToReg64 [D],[B]
   994 :StoreRegOnStackSub64 [B],16
   995 :StoreFromPtrToReg64 [D],[C]

   //File:/Types/Vec2.uc
   //Line:11
   //Source Line:[Cant Find Line]

   996 :StoreRegOnStackSub64 [C],8
   997 :GetFromStackSub64 [F],8

   //Line:12
   //Source Line:[Cant Find Line]

   998 :StoreRegOnStackSub64 [F],16
   999 :Debug_LineEnter 

   //StackFrameSize:1

   1000 :GetFromStackSub64 [A],24
   1001 :StoreRegOnStackSub64 [A],24
   1002 :GetFromStackSub64 [A],24

   //File:/Types/Vec2.uc
   //Line:15
   //Source Line:[Cant Find Line]

   1003 :Push64 [A]
   1004 :StoreRegToReg64 [E],[D]
   1005 :Call {ULang:Vec2f_t<float32>:(&equal&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut}
   1009 :LogicalNot8 [F],[A]
   1010 :StoreRegToReg8 [A],[F]
   1011 :Store64 [B], 24
   1015 :DecrementStackPointer [B]
   1016 :Debug_FuncEnd 
   1017 :Ret 
---ULang:Vec2f_t<float32>:(&Compound_Plus&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut: 
   1018 :Debug_FuncStart 
   1019 :Debug_LineEnter 
   1020 :StoreFromPtrToReg32 [D],[A]

   //StackFrameSize:24

   1021 :StoreFromPtrToReg32 [E],[B]
   1022 :Addf32 [A],[B],[F]
   1023 :StoreRegToPtr32 [F],[D]

   //File:/Types/Vec2.uc
   //Line:18
   //Source Line:[Cant Find Line]

   1024 :Debug_LineEnter 
   1025 :Read32 [D],[C],4
   1026 :Read32 [E],[A],4
   1027 :Addf32 [C],[A],[F]
   1028 :Load32 [D],[F],4
   1029 :Debug_FuncEnd 
   1030 :Ret 
---ULang:Vec2f_t<float32>:(&Compound_Sub&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut: 
   1031 :Debug_FuncStart 
   1032 :Debug_LineEnter 
   1033 :StoreFromPtrToReg32 [D],[A]
   1034 :StoreFromPtrToReg32 [E],[B]
   1035 :Subf32 [A],[B],[F]
   1036 :StoreRegToPtr32 [F],[D]
   1037 :Debug_LineEnter 
   1038 :Read32 [D],[C],4
   1039 :Read32 [E],[A],4
   1040 :Subf32 [C],[A],[F]
   1041 :Load32 [D],[F],4
   1042 :Debug_FuncEnd 
   1043 :Ret 
---ULang:Vec2f_t<float32>:(&Plus&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut: 
   1044 :Store64 [A], 32
   1048 :IncrementStackPointer [A]
   1049 :Debug_FuncStart 
   1050 :Debug_LineEnter 
   1051 :StoreFromPtrToReg64 [D],[A]
   1052 :StoreRegOnStackSub64 [A],32
   1053 :StoreFromPtrToReg64 [D],[B]
   1054 :StoreRegOnStackSub64 [B],24

   //File:/Types/Vec2.uc
   //Line:21
   //Source Line:[Cant Find Line]

   1055 :StoreFromPtrToReg64 [D],[C]
   1056 :StoreRegOnStackSub64 [C],16
   1057 :GetFromStackSub64 [F],16
   1058 :StoreRegOnStackSub64 [F],24
   1059 :GetFromStackSub64 [A],32

   //Line:22
   //Source Line:[Cant Find Line]

   1060 :StoreRegOnStackSub64 [A],8
   1061 :GetPointerOfStackSub [A],8
   1062 :Debug_LineEnter 
   1063 :StoreRegToReg64 [A],[D]
   1064 :Call {ULang:Vec2f_t<float32>:(&Compound_Plus&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut}

   //File:/Types/Vec2.uc
   //Line:25
   //Source Line:[Cant Find Line]

   1068 :GetFromStackSub64 [A],8
   1069 :StoreRegOnStackSub64 [A],8
   1070 :GetFromStackSub64 [F],8
   1071 :Debug_LineEnter 
   1072 :Store64 [B], 32
   1076 :DecrementStackPointer [B]
   1077 :Debug_FuncEnd 

   //StackFrameSize:32

   1078 :Ret 
---ULang:Vec2f_t<float32>:(&Minus&)^ULang:Vec2f_t<float32>&imut,ULang:Vec2f_t<float32>&imut: 
   1079 :Store64 [A], 32
   1083 :IncrementStackPointer [A]
   1084 :Debug_FuncStart 
   1085 :Debug_LineEnter 
   1086 :StoreFromPtrToReg64 [D],[A]
   1087 :StoreRegOnStackSub64 [A],32
   1088 :StoreFromPtrToReg64 [D],[B]
   1089 :StoreRegOnStackSub64 [B],24
   1090 :StoreFromPtrToReg64 [D],[C]
   1091 :StoreRegOnStackSub64 [C],16
   1092 :GetFromStackSub64 [F],16

   //Line:30
   //Source Line:[Cant Find Line]

   1093 :StoreRegOnStackSub64 [F],24
   1094 :GetFromStackSub64 [A],32
   1095 :StoreRegOnStackSub64 [A],8
   1096 :GetPointerOfStackSub [A],8
   1097 :Debug_LineEnter 
   1098 :StoreRegToReg64 [A],[D]
   1099 :Call {ULang:Vec2f_t<float32>:(&Compound_Sub&)^ULang:Vec2f_t<float32>&,ULang:Vec2f_t<float32>&imut}
   1103 :GetFromStackSub64 [A],8
   1104 :StoreRegOnStackSub64 [A],8
   1105 :GetFromStackSub64 [F],8
   1106 :Debug_LineEnter 
   1107 :Store64 [B], 32

   //StackFrameSize:32

   1111 :DecrementStackPointer [B]
   1112 :Debug_FuncEnd 
   1113 :Ret 
---ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&: 

   //File:/Types/Vec2.uc
   //Line:34
   //Source Line:[Cant Find Line]

   1114 :Debug_FuncStart 
   1115 :Store32 [A], 0
   1117 :Debug_LineEnter 
   1118 :StoreRegToPtr32 [A],[D]
   1119 :Debug_LineEnter 
   1120 :Load32 [D],[A],4
   1121 :Debug_FuncEnd 
   1122 :Ret 
---ULang:Vec2f_t<sint32>:(&_new&)^ULang:Vec2f_t<sint32>&,i32,i32: 
   1123 :Debug_FuncStart 
   1124 :Debug_LineEnter 
   1125 :StoreRegToPtr32 [E],[D]

   //Line:35
   //Source Line:[Cant Find Line]

   1126 :Debug_LineEnter 
   1127 :Load32 [D],[F],4
   1128 :Debug_FuncEnd 
   1129 :Ret 
---ULang:Vec2f_t<sint32>:(&equal&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut: 
   1130 :Store64 [A], 1

   //Line:36
   //Source Line:[Cant Find Line]

   1134 :IncrementStackPointer [A]
   1135 :Debug_FuncStart 
   1136 :StoreFromPtrToReg32 [D],[A]
   1137 :Debug_LineEnter 
   1138 :StoreFromPtrToReg32 [E],[B]
   1139 :equalto8 [B],[A],[F]
   1140 :Read32 [E],[C],4
   1141 :Read32 [D],[A],4
   1142 :StoreRegOnStackSub8 [F],1
   1143 :equalto8 [A],[C],[F]
   1144 :GetFromStackSub8 [A],1
   1145 :Ins 38:5,0,5
   1146 :Store64 [A], 1
   1150 :DecrementStackPointer [A]

   //Line:8
   //Source Line:[Cant Find Line]

   1151 :Debug_FuncEnd 
   1152 :Ret 
---ULang:Vec2f_t<sint32>:(&notequal&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut: 
   1153 :Store64 [A], 24
   1157 :IncrementStackPointer [A]

   //Line:12
   //Source Line:[Cant Find Line]

   1158 :Debug_FuncStart 
   1159 :StoreFromPtrToReg64 [D],[A]

   //StackFrameSize:1

   1160 :StoreRegOnStackSub64 [A],24
   1161 :StoreFromPtrToReg64 [D],[B]
   1162 :StoreRegOnStackSub64 [B],16

   //File:/Types/Vec2.uc
   //Line:15
   //Source Line:[Cant Find Line]

   1163 :StoreFromPtrToReg64 [D],[C]
   1164 :StoreRegOnStackSub64 [C],8
   1165 :GetFromStackSub64 [F],8
   1166 :StoreRegOnStackSub64 [F],16
   1167 :Debug_LineEnter 
   1168 :GetFromStackSub64 [A],24
   1169 :StoreRegOnStackSub64 [A],24
   1170 :GetFromStackSub64 [A],24
   1171 :Push64 [A]
   1172 :StoreRegToReg64 [E],[D]
   1173 :Call {ULang:Vec2f_t<sint32>:(&equal&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut}
   1177 :LogicalNot8 [F],[A]
   1178 :StoreRegToReg8 [A],[F]
   1179 :Store64 [B], 24
   1183 :DecrementStackPointer [B]

   //File:/Types/Vec2.uc
   //Line:18
   //Source Line:[Cant Find Line]

   1184 :Debug_FuncEnd 
   1185 :Ret 
---ULang:Vec2f_t<sint32>:(&Compound_Plus&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut: 
   1186 :Debug_FuncStart 
   1187 :Debug_LineEnter 
   1188 :StoreFromPtrToReg32 [D],[A]
   1189 :StoreFromPtrToReg32 [E],[B]
   1190 :Add32 [A],[B],[F]
   1191 :StoreRegToPtr32 [F],[D]
   1192 :Debug_LineEnter 
   1193 :Read32 [D],[C],4
   1194 :Read32 [E],[A],4
   1195 :Add32 [C],[A],[F]
   1196 :Load32 [D],[F],4
   1197 :Debug_FuncEnd 
   1198 :Ret 
---ULang:Vec2f_t<sint32>:(&Compound_Sub&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut: 
   1199 :Debug_FuncStart 
   1200 :Debug_LineEnter 
   1201 :StoreFromPtrToReg32 [D],[A]
   1202 :StoreFromPtrToReg32 [E],[B]
   1203 :Sub32 [A],[B],[F]
   1204 :StoreRegToPtr32 [F],[D]
   1205 :Debug_LineEnter 
   1206 :Read32 [D],[C],4
   1207 :Read32 [E],[A],4
   1208 :Sub32 [C],[A],[F]
   1209 :Load32 [D],[F],4
   1210 :Debug_FuncEnd 
   1211 :Ret 
---ULang:Vec2f_t<sint32>:(&Plus&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut: 
   1212 :Store64 [A], 32
   1216 :IncrementStackPointer [A]
   1217 :Debug_FuncStart 
   1218 :Debug_LineEnter 
   1219 :StoreFromPtrToReg64 [D],[A]

   //Line:22
   //Source Line:[Cant Find Line]

   1220 :StoreRegOnStackSub64 [A],32
   1221 :StoreFromPtrToReg64 [D],[B]
   1222 :StoreRegOnStackSub64 [B],24
   1223 :StoreFromPtrToReg64 [D],[C]
   1224 :StoreRegOnStackSub64 [C],16
   1225 :GetFromStackSub64 [F],16
   1226 :StoreRegOnStackSub64 [F],24
   1227 :GetFromStackSub64 [A],32

   //File:/Types/Vec2.uc
   //Line:25
   //Source Line:[Cant Find Line]

   1228 :StoreRegOnStackSub64 [A],8
   1229 :GetPointerOfStackSub [A],8
   1230 :Debug_LineEnter 
   1231 :StoreRegToReg64 [A],[D]
   1232 :Call {ULang:Vec2f_t<sint32>:(&Compound_Plus&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut}
   1236 :GetFromStackSub64 [A],8
   1237 :StoreRegOnStackSub64 [A],8

   //StackFrameSize:32

   1238 :GetFromStackSub64 [F],8
   1239 :Debug_LineEnter 
   1240 :Store64 [B], 32
   1244 :DecrementStackPointer [B]
   1245 :Debug_FuncEnd 
   1246 :Ret 
---ULang:Vec2f_t<sint32>:(&Minus&)^ULang:Vec2f_t<sint32>&imut,ULang:Vec2f_t<sint32>&imut: 
   1247 :Store64 [A], 32
   1251 :IncrementStackPointer [A]
   1252 :Debug_FuncStart 

   //Line:30
   //Source Line:[Cant Find Line]

   1253 :Debug_LineEnter 
   1254 :StoreFromPtrToReg64 [D],[A]
   1255 :StoreRegOnStackSub64 [A],32
   1256 :StoreFromPtrToReg64 [D],[B]
   1257 :StoreRegOnStackSub64 [B],24
   1258 :StoreFromPtrToReg64 [D],[C]
   1259 :StoreRegOnStackSub64 [C],16
   1260 :GetFromStackSub64 [F],16

   //Line:31
   //Source Line:[Cant Find Line]

   1261 :StoreRegOnStackSub64 [F],24
   1262 :GetFromStackSub64 [A],32
   1263 :StoreRegOnStackSub64 [A],8
   1264 :GetPointerOfStackSub [A],8
   1265 :Debug_LineEnter 
   1266 :StoreRegToReg64 [A],[D]
   1267 :Call {ULang:Vec2f_t<sint32>:(&Compound_Sub&)^ULang:Vec2f_t<sint32>&,ULang:Vec2f_t<sint32>&imut}

   //StackFrameSize:32

   1271 :GetFromStackSub64 [A],8
   1272 :StoreRegOnStackSub64 [A],8
   1273 :GetFromStackSub64 [F],8

   //File:/Types/Vec2.uc
   //Line:34
   //Source Line:[Cant Find Line]

   1274 :Debug_LineEnter 
   1275 :Store64 [B], 32
   1279 :DecrementStackPointer [B]
   1280 :Debug_FuncEnd 
   1281 :Ret 
---ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&: 
   1282 :Debug_FuncStart 
   1283 :Storef32 [A], 0.000000
   1285 :Debug_LineEnter 

   //Line:35
   //Source Line:[Cant Find Line]

   1286 :StoreRegToPtr32 [A],[D]
   1287 :Debug_LineEnter 
   1288 :Load32 [D],[A],4
   1289 :Debug_LineEnter 
   1290 :Load32 [D],[A],8
   1291 :Debug_FuncEnd 
   1292 :Ret 
---ULang:Vec3f_t<float32>:(&_new&)^ULang:Vec3f_t<float32>&,f32,f32,f32: 
   1293 :Debug_FuncStart 

   //Line:36
   //Source Line:[Cant Find Line]

   1294 :Debug_LineEnter 
   1295 :StoreRegToPtr32 [E],[D]
   1296 :Debug_LineEnter 
   1297 :Load32 [D],[F],4
   1298 :GetFromStackSub32 [A],12
   1299 :StoreRegOnStackSub32 [A],12
   1300 :Debug_LineEnter 
   1301 :GetFromStackSub32 [B],12
   1302 :Load32 [D],[B],8
   1303 :Debug_FuncEnd 
   1304 :Ret 
---ULang:Vec3f_t<float32>:(&equal&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&: 
   1305 :Store64 [A], 2

   //File:/Types/Vec3.uc
   //Line:8
   //Source Line:[Cant Find Line]

   1309 :IncrementStackPointer [A]
   1310 :Debug_FuncStart 

   //Line:9
   //Source Line:[Cant Find Line]

   1311 :StoreFromPtrToReg32 [D],[A]
   1312 :Debug_LineEnter 

   //Line:10
   //Source Line:[Cant Find Line]

   1313 :StoreFromPtrToReg32 [E],[B]
   1314 :equalto8 [B],[A],[F]
   1315 :Read32 [D],[C],4
   1316 :Read32 [E],[A],4
   1317 :StoreRegOnStackSub8 [F],2

   //File:/Types/Vec3.uc
   //Line:13
   //Source Line:[Cant Find Line]

   1318 :equalto8 [A],[C],[F]
   1319 :Read32 [E],[A],8

   //Line:14
   //Source Line:[Cant Find Line]

   1320 :Read32 [D],[B],8
   1321 :StoreRegOnStackSub8 [F],1
   1322 :equalto8 [B],[A],[F]
   1323 :GetFromStackSub8 [A],1

   //Line:15
   //Source Line:[Cant Find Line]

   1324 :Ins 38:5,0,5
   1325 :GetFromStackSub8 [A],2
   1326 :Ins 38:5,0,5

   //StackFrameSize:2

   1327 :Store64 [A], 2
   1331 :DecrementStackPointer [A]
   1332 :Debug_FuncEnd 
   1333 :Ret 
---ULang:Vec3f_t<float32>:(&notequal&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&: 
   1334 :Store64 [A], 24
   1338 :IncrementStackPointer [A]
   1339 :Debug_FuncStart 
   1340 :StoreFromPtrToReg64 [D],[A]
   1341 :StoreRegOnStackSub64 [A],24
   1342 :StoreFromPtrToReg64 [D],[B]
   1343 :StoreRegOnStackSub64 [B],16
   1344 :StoreFromPtrToReg64 [D],[C]
   1345 :StoreRegOnStackSub64 [C],8
   1346 :GetFromStackSub64 [F],8
   1347 :StoreRegOnStackSub64 [F],16
   1348 :Debug_LineEnter 
   1349 :GetFromStackSub64 [A],24
   1350 :StoreRegOnStackSub64 [A],24
   1351 :GetFromStackSub32 [A],16
   1352 :StoreRegOnStackSub32 [A],16
   1353 :GetFromStackSub64 [A],24

   //StackFrameSize:24

   1354 :Push64 [A]
   1355 :GetFromStackSub32 [A],24
   1356 :Push32 [A]

   //File:/Types/Vec3.uc
   //Line:21
   //Source Line:[Cant Find Line]

   1357 :StoreRegToReg64 [E],[D]
   1358 :Call {ULang:Vec3f_t<float32>:(&equal&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&}
   1362 :LogicalNot8 [F],[A]
   1363 :StoreRegToReg8 [A],[F]
   1364 :Store64 [B], 24
   1368 :DecrementStackPointer [B]
   1369 :Debug_FuncEnd 
   1370 :Ret 
---ULang:Vec3f_t<float32>:(&Compound_Plus&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut: 
   1371 :Debug_FuncStart 
   1372 :Debug_LineEnter 
   1373 :StoreFromPtrToReg32 [D],[A]
   1374 :StoreFromPtrToReg32 [E],[B]
   1375 :Addf32 [A],[B],[F]
   1376 :StoreRegToPtr32 [F],[D]
   1377 :Debug_LineEnter 
   1378 :Read32 [D],[C],4
   1379 :Read32 [E],[A],4
   1380 :Addf32 [C],[A],[F]
   1381 :Load32 [D],[F],4
   1382 :Debug_LineEnter 
   1383 :Read32 [D],[A],8
   1384 :Read32 [E],[B],8
   1385 :Addf32 [A],[B],[F]
   1386 :Load32 [D],[F],8
   1387 :Debug_FuncEnd 
   1388 :Ret 
---ULang:Vec3f_t<float32>:(&Compound_Sub&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut: 
   1389 :Debug_FuncStart 
   1390 :Debug_LineEnter 
   1391 :StoreFromPtrToReg32 [D],[A]

   //File:/Types/Vec3.uc
   //Line:24
   //Source Line:[Cant Find Line]

   1392 :StoreFromPtrToReg32 [E],[B]
   1393 :Subf32 [A],[B],[F]
   1394 :StoreRegToPtr32 [F],[D]
   1395 :Debug_LineEnter 
   1396 :Read32 [D],[C],4

   //Line:25
   //Source Line:[Cant Find Line]

   1397 :Read32 [E],[A],4
   1398 :Subf32 [C],[A],[F]
   1399 :Load32 [D],[F],4
   1400 :Debug_LineEnter 
   1401 :Read32 [D],[A],8

   //Line:26
   //Source Line:[Cant Find Line]

   1402 :Read32 [E],[B],8
   1403 :Subf32 [A],[B],[F]
   1404 :Load32 [D],[F],8
   1405 :Debug_FuncEnd 
   1406 :Ret 
---ULang:Vec3f_t<float32>:(&Plus&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&imut: 
   1407 :Store64 [A], 36
   1411 :IncrementStackPointer [A]
   1412 :Debug_FuncStart 
   1413 :Debug_LineEnter 
   1414 :StoreFromPtrToReg64 [D],[A]

   //Line:30
   //Source Line:[Cant Find Line]

   1415 :StoreRegOnStackSub64 [A],36
   1416 :StoreFromPtrToReg64 [D],[B]
   1417 :StoreRegOnStackSub64 [B],28
   1418 :StoreFromPtrToReg64 [D],[C]
   1419 :StoreRegOnStackSub64 [C],20

   //Line:31
   //Source Line:[Cant Find Line]

   1420 :GetFromStackSub64 [F],20
   1421 :StoreRegOnStackSub64 [F],28
   1422 :GetFromStackSub64 [A],36
   1423 :StoreRegOnStackSub64 [A],12
   1424 :GetFromStackSub32 [A],28

   //StackFrameSize:36

   1425 :StoreRegOnStackSub32 [A],4
   1426 :GetPointerOfStackSub [A],12
   1427 :Debug_LineEnter 

   //File:/Types/Vec3.uc
   //Line:34
   //Source Line:[Cant Find Line]

   1428 :StoreRegToReg64 [A],[D]
   1429 :Call {ULang:Vec3f_t<float32>:(&Compound_Plus&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut}
   1433 :GetFromStackSub64 [A],12
   1434 :StoreRegOnStackSub64 [A],12
   1435 :GetFromStackSub32 [A],4
   1436 :StoreRegOnStackSub32 [A],4
   1437 :GetPointerOfStackSub [B],12
   1438 :StoreRegToReg64 [B],[F]
   1439 :Debug_LineEnter 
   1440 :Store64 [B], 36
   1444 :DecrementStackPointer [B]
   1445 :Debug_FuncEnd 
   1446 :Ret 
---ULang:Vec3f_t<float32>:(&Minus&)^ULang:Vec3f_t<float32>&imut,ULang:Vec3f_t<float32>&imut: 
   1447 :Store64 [A], 36
   1451 :IncrementStackPointer [A]

   //Line:36
   //Source Line:[Cant Find Line]

   1452 :Debug_FuncStart 
   1453 :Debug_LineEnter 
   1454 :StoreFromPtrToReg64 [D],[A]
   1455 :StoreRegOnStackSub64 [A],36
   1456 :StoreFromPtrToReg64 [D],[B]
   1457 :StoreRegOnStackSub64 [B],28
   1458 :StoreFromPtrToReg64 [D],[C]
   1459 :StoreRegOnStackSub64 [C],20
   1460 :GetFromStackSub64 [F],20
   1461 :StoreRegOnStackSub64 [F],28
   1462 :GetFromStackSub64 [A],36

   //StackFrameSize:36

   1463 :StoreRegOnStackSub64 [A],12
   1464 :GetFromStackSub32 [A],28
   1465 :StoreRegOnStackSub32 [A],4

   //File:/Types/Vec3.uc
   //Line:39
   //Source Line:[Cant Find Line]

   1466 :GetPointerOfStackSub [A],12
   1467 :Debug_LineEnter 
   1468 :StoreRegToReg64 [A],[D]
   1469 :Call {ULang:Vec3f_t<float32>:(&Compound_Sub&)^ULang:Vec3f_t<float32>&,ULang:Vec3f_t<float32>&imut}
   1473 :GetFromStackSub64 [A],12
   1474 :StoreRegOnStackSub64 [A],12
   1475 :GetFromStackSub32 [A],4
   1476 :StoreRegOnStackSub32 [A],4
   1477 :GetPointerOfStackSub [B],12
   1478 :StoreRegToReg64 [B],[F]
   1479 :Debug_LineEnter 

   //Line:40
   //Source Line:[Cant Find Line]

   1480 :Store64 [B], 36
   1484 :DecrementStackPointer [B]
   1485 :Debug_FuncEnd 
   1486 :Ret 
---ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&: 
   1487 :Debug_FuncStart 
   1488 :Store32 [A], 0

   //Line:41
   //Source Line:[Cant Find Line]

   1490 :Debug_LineEnter 
   1491 :StoreRegToPtr32 [A],[D]
   1492 :Debug_LineEnter 
   1493 :Load32 [D],[A],4
   1494 :Debug_LineEnter 
   1495 :Load32 [D],[A],8
   1496 :Debug_FuncEnd 
   1497 :Ret 
---ULang:Vec3f_t<sint32>:(&_new&)^ULang:Vec3f_t<sint32>&,i32,i32,i32: 
   1498 :Debug_FuncStart 
   1499 :Debug_LineEnter 
   1500 :StoreRegToPtr32 [E],[D]
   1501 :Debug_LineEnter 
   1502 :Load32 [D],[F],4
   1503 :GetFromStackSub32 [A],12
   1504 :StoreRegOnStackSub32 [A],12
   1505 :Debug_LineEnter 

   //File:/Types/Vec3.uc
   //Line:8
   //Source Line:[Cant Find Line]

   1506 :GetFromStackSub32 [B],12
   1507 :Load32 [D],[B],8

   //Line:9
   //Source Line:[Cant Find Line]

   1508 :Debug_FuncEnd 
   1509 :Ret 
---ULang:Vec3f_t<sint32>:(&equal&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&: 

   //Line:10
   //Source Line:[Cant Find Line]

   1510 :Store64 [A], 2
   1514 :IncrementStackPointer [A]

   //File:/Types/Vec3.uc
   //Line:13
   //Source Line:[Cant Find Line]

   1515 :Debug_FuncStart 
   1516 :StoreFromPtrToReg32 [D],[A]

   //Line:14
   //Source Line:[Cant Find Line]

   1517 :Debug_LineEnter 
   1518 :StoreFromPtrToReg32 [E],[B]
   1519 :equalto8 [B],[A],[F]
   1520 :Read32 [D],[C],4

   //Line:15
   //Source Line:[Cant Find Line]

   1521 :Read32 [E],[A],4
   1522 :StoreRegOnStackSub8 [F],2
   1523 :equalto8 [A],[C],[F]

   //StackFrameSize:2

   1524 :Read32 [E],[A],8
   1525 :Read32 [D],[B],8
   1526 :StoreRegOnStackSub8 [F],1

   //File:/Types/Vec3.uc
   //Line:18
   //Source Line:[Cant Find Line]

   1527 :equalto8 [B],[A],[F]
   1528 :GetFromStackSub8 [A],1
   1529 :Ins 38:5,0,5
   1530 :GetFromStackSub8 [A],2
   1531 :Ins 38:5,0,5
   1532 :Store64 [A], 2
   1536 :DecrementStackPointer [A]
   1537 :Debug_FuncEnd 
   1538 :Ret 
---ULang:Vec3f_t<sint32>:(&notequal&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&: 
   1539 :Store64 [A], 24
   1543 :IncrementStackPointer [A]
   1544 :Debug_FuncStart 
   1545 :StoreFromPtrToReg64 [D],[A]
   1546 :StoreRegOnStackSub64 [A],24
   1547 :StoreFromPtrToReg64 [D],[B]
   1548 :StoreRegOnStackSub64 [B],16
   1549 :StoreFromPtrToReg64 [D],[C]
   1550 :StoreRegOnStackSub64 [C],8

   //StackFrameSize:24

   1551 :GetFromStackSub64 [F],8
   1552 :StoreRegOnStackSub64 [F],16
   1553 :Debug_LineEnter 

   //File:/Types/Vec3.uc
   //Line:21
   //Source Line:[Cant Find Line]

   1554 :GetFromStackSub64 [A],24
   1555 :StoreRegOnStackSub64 [A],24
   1556 :GetFromStackSub32 [A],16
   1557 :StoreRegOnStackSub32 [A],16
   1558 :GetFromStackSub64 [A],24
   1559 :Push64 [A]
   1560 :GetFromStackSub32 [A],24
   1561 :Push32 [A]
   1562 :StoreRegToReg64 [E],[D]
   1563 :Call {ULang:Vec3f_t<sint32>:(&equal&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&}
   1567 :LogicalNot8 [F],[A]
   1568 :StoreRegToReg8 [A],[F]
   1569 :Store64 [B], 24
   1573 :DecrementStackPointer [B]
   1574 :Debug_FuncEnd 
   1575 :Ret 
---ULang:Vec3f_t<sint32>:(&Compound_Plus&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut: 
   1576 :Debug_FuncStart 
   1577 :Debug_LineEnter 
   1578 :StoreFromPtrToReg32 [D],[A]
   1579 :StoreFromPtrToReg32 [E],[B]
   1580 :Add32 [A],[B],[F]
   1581 :StoreRegToPtr32 [F],[D]
   1582 :Debug_LineEnter 
   1583 :Read32 [D],[C],4
   1584 :Read32 [E],[A],4
   1585 :Add32 [C],[A],[F]
   1586 :Load32 [D],[F],4
   1587 :Debug_LineEnter 
   1588 :Read32 [D],[A],8

   //File:/Types/Vec3.uc
   //Line:24
   //Source Line:[Cant Find Line]

   1589 :Read32 [E],[B],8
   1590 :Add32 [A],[B],[F]
   1591 :Load32 [D],[F],8
   1592 :Debug_FuncEnd 
   1593 :Ret 
---ULang:Vec3f_t<sint32>:(&Compound_Sub&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut: 

   //Line:25
   //Source Line:[Cant Find Line]

   1594 :Debug_FuncStart 
   1595 :Debug_LineEnter 
   1596 :StoreFromPtrToReg32 [D],[A]
   1597 :StoreFromPtrToReg32 [E],[B]
   1598 :Sub32 [A],[B],[F]

   //Line:26
   //Source Line:[Cant Find Line]

   1599 :StoreRegToPtr32 [F],[D]
   1600 :Debug_LineEnter 
   1601 :Read32 [D],[C],4
   1602 :Read32 [E],[A],4
   1603 :Sub32 [C],[A],[F]
   1604 :Load32 [D],[F],4
   1605 :Debug_LineEnter 
   1606 :Read32 [D],[A],8

   //File:/Types/Vec3.uc
   //Line:29
   //Source Line:[Cant Find Line]

   1607 :Read32 [E],[B],8
   1608 :Sub32 [A],[B],[F]
   1609 :Load32 [D],[F],8
   1610 :Debug_FuncEnd 
   1611 :Ret 
---ULang:Vec3f_t<sint32>:(&Plus&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&imut: 

   //Line:30
   //Source Line:[Cant Find Line]

   1612 :Store64 [A], 36
   1616 :IncrementStackPointer [A]

   //Line:31
   //Source Line:[Cant Find Line]

   1617 :Debug_FuncStart 
   1618 :Debug_LineEnter 
   1619 :StoreFromPtrToReg64 [D],[A]
   1620 :StoreRegOnStackSub64 [A],36
   1621 :StoreFromPtrToReg64 [D],[B]

   //StackFrameSize:36

   1622 :StoreRegOnStackSub64 [B],28
   1623 :StoreFromPtrToReg64 [D],[C]
   1624 :StoreRegOnStackSub64 [C],20

   //File:/Types/Vec3.uc
   //Line:34
   //Source Line:[Cant Find Line]

   1625 :GetFromStackSub64 [F],20
   1626 :StoreRegOnStackSub64 [F],28
   1627 :GetFromStackSub64 [A],36
   1628 :StoreRegOnStackSub64 [A],12
   1629 :GetFromStackSub32 [A],28
   1630 :StoreRegOnStackSub32 [A],4
   1631 :GetPointerOfStackSub [A],12
   1632 :Debug_LineEnter 
   1633 :StoreRegToReg64 [A],[D]
   1634 :Call {ULang:Vec3f_t<sint32>:(&Compound_Plus&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut}
   1638 :GetFromStackSub64 [A],12

   //Line:35
   //Source Line:[Cant Find Line]

   1639 :StoreRegOnStackSub64 [A],12
   1640 :GetFromStackSub32 [A],4
   1641 :StoreRegOnStackSub32 [A],4
   1642 :GetPointerOfStackSub [B],12
   1643 :StoreRegToReg64 [B],[F]
   1644 :Debug_LineEnter 
   1645 :Store64 [B], 36

   //Line:36
   //Source Line:[Cant Find Line]

   1649 :DecrementStackPointer [B]
   1650 :Debug_FuncEnd 
   1651 :Ret 
---ULang:Vec3f_t<sint32>:(&Minus&)^ULang:Vec3f_t<sint32>&imut,ULang:Vec3f_t<sint32>&imut: 
   1652 :Store64 [A], 36
   1656 :IncrementStackPointer [A]
   1657 :Debug_FuncStart 
   1658 :Debug_LineEnter 
   1659 :StoreFromPtrToReg64 [D],[A]

   //StackFrameSize:36

   1660 :StoreRegOnStackSub64 [A],36
   1661 :StoreFromPtrToReg64 [D],[B]
   1662 :StoreRegOnStackSub64 [B],28

   //File:/Types/Vec3.uc
   //Line:39
   //Source Line:[Cant Find Line]

   1663 :StoreFromPtrToReg64 [D],[C]
   1664 :StoreRegOnStackSub64 [C],20
   1665 :GetFromStackSub64 [F],20
   1666 :StoreRegOnStackSub64 [F],28
   1667 :GetFromStackSub64 [A],36
   1668 :StoreRegOnStackSub64 [A],12
   1669 :GetFromStackSub32 [A],28
   1670 :StoreRegOnStackSub32 [A],4
   1671 :GetPointerOfStackSub [A],12
   1672 :Debug_LineEnter 
   1673 :StoreRegToReg64 [A],[D]
   1674 :Call {ULang:Vec3f_t<sint32>:(&Compound_Sub&)^ULang:Vec3f_t<sint32>&,ULang:Vec3f_t<sint32>&imut}
   1678 :GetFromStackSub64 [A],12
   1679 :StoreRegOnStackSub64 [A],12
   1680 :GetFromStackSub32 [A],4
   1681 :StoreRegOnStackSub32 [A],4
   1682 :GetPointerOfStackSub [B],12
   1683 :StoreRegToReg64 [B],[F]
   1684 :Debug_LineEnter 
   1685 :Store64 [B], 36
   1689 :DecrementStackPointer [B]
   1690 :Debug_FuncEnd 
   1691 :Ret 
---ULang:Ptr<sint32>:(&_new&)^ULang:Ptr<sint32>&,i32&: 
   1692 :Debug_FuncStart 
   1693 :Debug_LineEnter 
   1694 :StoreRegToPtr64 [E],[D]
   1695 :Debug_FuncEnd 
   1696 :Ret 
---ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&: 
   1697 :Debug_FuncStart 
   1698 :StoreFromPtrToReg64 [D],[A]
   1699 :StoreRegToReg64 [A],[F]
   1700 :Debug_LineEnter 

   //File:/Types/Ptr.uc
   //Line:12
   //Source Line:[Cant Find Line]

   1701 :Debug_FuncEnd 
   1702 :Ret 
---ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&imut: 
   1703 :Debug_FuncStart 
   1704 :StoreFromPtrToReg64 [D],[A]
   1705 :StoreRegToReg64 [A],[F]
   1706 :Debug_LineEnter 

   //File:/Types/Ptr.uc
   //Line:14
   //Source Line:[Cant Find Line]

   1707 :Debug_FuncEnd 
   1708 :Ret 
---ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32&imut: 
   1709 :Debug_FuncStart 
   1710 :Debug_FuncEnd 
   1711 :Ret 
---ULang:Ptr<sint32>:Write^ULang:Ptr<sint32>&,i32mov: 
   1712 :Debug_FuncStart 

   //File:/Types/Ptr.uc
   //Line:15
   //Source Line:[Cant Find Line]

   1713 :Debug_FuncEnd 
   1714 :Ret 
---ULang:Ptr<sint32>:ReadPtr^ULang:Ptr<sint32>&imut: 
   1715 :Debug_FuncStart 
   1716 :Debug_LineEnter 
   1717 :Call {ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&imut}
   1721 :Debug_FuncEnd 
   1722 :Ret 
---ULang:Ptr<sint32>:(&IndirectMember&)^ULang:Ptr<sint32>&: 
   1723 :Debug_FuncStart 

   //File:/Types/Ptr.uc
   //Line:20
   //Source Line:[Cant Find Line]

   1724 :Debug_LineEnter 
   1725 :Call {ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&}
   1729 :Debug_FuncEnd 
   1730 :Ret 
---ULang:Ptr<sint32>:(&IndirectMember&)^ULang:Ptr<sint32>&imut: 
   1731 :Debug_FuncStart 

   //File:/Types/Ptr.uc
   //Line:22
   //Source Line:[Cant Find Line]

   1732 :Debug_LineEnter 
   1733 :Call {ULang:Ptr<sint32>:Get^ULang:Ptr<sint32>&imut}
   1737 :Debug_FuncEnd 
   1738 :Ret 
---ULang:Ptr<sint32>:ReassignTo^ULang:Ptr<sint32>&,i32&: 
   1739 :Debug_FuncStart 

   //File:/Types/Ptr.uc
   //Line:24
   //Source Line:[Cant Find Line]

   1740 :Debug_LineEnter 
   1741 :StoreRegToPtr64 [E],[D]
   1742 :Debug_FuncEnd 
   1743 :Ret 
---ULang:Ptr<sint32>:Make^i32&imut: 
   1744 :Store64 [A], 8

   //File:/Types/Ptr.uc
   //Line:26
   //Source Line:[Cant Find Line]

   1748 :IncrementStackPointer [A]
   1749 :Debug_FuncStart 

   //StackFrameSize:8

   1750 :GetPointerOfStackSub [A],8
   1751 :Debug_LineEnter 
   1752 :StoreFromPtrToReg64 [D],[B]

   //File:/Types/Ptr.uc
   //Line:0
   //Source Line:[Cant Find Line]

   1753 :StoreRegToReg64 [A],[D]
   1754 :Debug_LineEnter 
   1755 :StoreRegToReg32 [B],[E]

   //Line:30
   //Source Line:[Cant Find Line]

   1756 :Call {ULang:Ptr<sint32>:(&_new&)^ULang:Ptr<sint32>&,i32&}
   1760 :GetFromStackSub64 [F],8
   1761 :Store64 [A], 8
   1765 :DecrementStackPointer [A]
   1766 :Debug_FuncEnd 
   1767 :Ret 
---ULang:Ptr<sint32>:Make^i32&: 
   1768 :Store64 [A], 8

   //StackFrameSize:8

   1772 :IncrementStackPointer [A]
   1773 :Debug_FuncStart 
   1774 :GetPointerOfStackSub [A],8

   //File:/Types/Ptr.uc
   //Line:0
   //Source Line:[Cant Find Line]

   1775 :Debug_LineEnter 
   1776 :StoreRegToReg64 [A],[D]

   //Line:31
   //Source Line:[Cant Find Line]

   1777 :Debug_LineEnter 
   1778 :StoreRegToReg64 [D],[E]
   1779 :Call {ULang:Ptr<sint32>:(&_new&)^ULang:Ptr<sint32>&,i32&}
   1783 :GetFromStackSub64 [F],8
   1784 :Store64 [A], 8
   1788 :DecrementStackPointer [A]
   1789 :Debug_FuncEnd 
   1790 :Ret 
---ULang:Ptr<sint32>:(&_new&)^ULang:Ptr<sint32>&: 
   1791 :Debug_FuncStart 
   1792 :Debug_FuncEnd 
   1793 :Ret 
---ULang:ptr<sint32>^i32&: 
   1794 :Debug_FuncStart 
   1795 :Debug_LineEnter 
   1796 :Call {ULang:Ptr<sint32>:Make^i32&}
   1800 :Debug_FuncEnd 
   1801 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:(&_new&)^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&: 
   1802 :Debug_FuncStart 
   1803 :Debug_LineEnter 
   1804 :StoreRegToPtr64 [E],[D]
   1805 :Debug_FuncEnd 
   1806 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&: 

   //File:/Types/Ptr.uc
   //Line:12
   //Source Line:[Cant Find Line]

   1807 :Debug_FuncStart 
   1808 :StoreFromPtrToReg64 [D],[A]
   1809 :StoreRegToReg64 [A],[F]
   1810 :Debug_LineEnter 
   1811 :Debug_FuncEnd 
   1812 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&imut: 

   //File:/Types/Ptr.uc
   //Line:14
   //Source Line:[Cant Find Line]

   1813 :Debug_FuncStart 
   1814 :StoreFromPtrToReg64 [D],[A]
   1815 :StoreRegToReg64 [A],[F]
   1816 :Debug_LineEnter 
   1817 :Debug_FuncEnd 
   1818 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Write^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&imut: 

   //File:/Types/Ptr.uc
   //Line:15
   //Source Line:[Cant Find Line]

   1819 :Debug_FuncStart 
   1820 :Debug_FuncEnd 
   1821 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Write^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>mov: 
   1822 :Debug_FuncStart 
   1823 :Debug_FuncEnd 
   1824 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:ReadPtr^ULang:Ptr<ULang:Vec2f_t<float32>>&imut: 
   1825 :Debug_FuncStart 
   1826 :Debug_LineEnter 
   1827 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&imut}
   1831 :Debug_FuncEnd 
   1832 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&: 
   1833 :Debug_FuncStart 
   1834 :Debug_LineEnter 
   1835 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&}
   1839 :Debug_FuncEnd 
   1840 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:(&IndirectMember&)^ULang:Ptr<ULang:Vec2f_t<float32>>&imut: 
   1841 :Debug_FuncStart 
   1842 :Debug_LineEnter 
   1843 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:Get^ULang:Ptr<ULang:Vec2f_t<float32>>&imut}
   1847 :Debug_FuncEnd 
   1848 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:ReassignTo^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&: 
   1849 :Debug_FuncStart 
   1850 :Debug_LineEnter 
   1851 :StoreRegToPtr64 [E],[D]
   1852 :Debug_FuncEnd 
   1853 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Make^ULang:Vec2f_t<float32>&imut: 

   //File:/Types/Ptr.uc
   //Line:26
   //Source Line:[Cant Find Line]

   1854 :Store64 [A], 32
   1858 :IncrementStackPointer [A]

   //File:/Types/Ptr.uc
   //Line:0
   //Source Line:[Cant Find Line]

   1859 :Debug_FuncStart 
   1860 :GetPointerOfStackSub [A],32
   1861 :Debug_LineEnter 
   1862 :StoreFromPtrToReg64 [D],[B]
   1863 :StoreRegOnStackSub64 [B],24
   1864 :StoreFromPtrToReg64 [D],[C]
   1865 :StoreRegOnStackSub64 [C],16
   1866 :StoreFromPtrToReg64 [D],[E]
   1867 :StoreRegOnStackSub64 [E],8
   1868 :GetFromStackSub64 [F],8

   //Line:30
   //Source Line:[Cant Find Line]

   1869 :StoreRegOnStackSub64 [F],16
   1870 :StoreRegToReg64 [A],[D]
   1871 :Debug_LineEnter 
   1872 :GetFromStackSub64 [A],24
   1873 :Push64 [A]
   1874 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:(&_new&)^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&}
   1878 :GetFromStackSub64 [F],40
   1879 :Store64 [A], 32
   1883 :DecrementStackPointer [A]
   1884 :Debug_FuncEnd 
   1885 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:Make^ULang:Vec2f_t<float32>&: 

   //StackFrameSize:8

   1886 :Store64 [A], 8
   1890 :IncrementStackPointer [A]

   //Line:31
   //Source Line:[Cant Find Line]

   1891 :Debug_FuncStart 
   1892 :GetPointerOfStackSub [A],8
   1893 :Debug_LineEnter 
   1894 :StoreRegToReg64 [A],[D]
   1895 :Debug_LineEnter 
   1896 :StoreRegToReg64 [D],[E]
   1897 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:(&_new&)^ULang:Ptr<ULang:Vec2f_t<float32>>&,ULang:Vec2f_t<float32>&}
   1901 :GetFromStackSub64 [F],8
   1902 :Store64 [A], 8
   1906 :DecrementStackPointer [A]
   1907 :Debug_FuncEnd 
   1908 :Ret 
---ULang:Ptr<ULang:Vec2f_t<float32>>:(&_new&)^ULang:Ptr<ULang:Vec2f_t<float32>>&: 
   1909 :Debug_FuncStart 
   1910 :Debug_FuncEnd 
   1911 :Ret 
---ULang:ptr<ULang:Vec2f_t<float32>>^ULang:Vec2f_t<float32>&: 
   1912 :Debug_FuncStart 

   //File:/Types/Ptr.uc
   //Line:5
   //Source Line:[Cant Find Line]

   1913 :Debug_LineEnter 
   1914 :Call {ULang:Ptr<ULang:Vec2f_t<float32>>:Make^ULang:Vec2f_t<float32>&}
   1918 :Debug_FuncEnd 
   1919 :Ret 
---ULang:Native:OS_PrintChar^c: 
   1920 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:9
   //Source Line:[Cant Find Line]

   1921 :Debug_LineEnter 
   1922 :Call {__LogChar^c}
   1926 :Debug_FuncEnd 
   1927 :Ret 
---ULang:Native:OS_PrintSpan^c[&]imut,uptr: 
   1928 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:20
   //Source Line:[Cant Find Line]

   1929 :Debug_LineEnter 
   1930 :Call {__LogBuffer^c[&]imut,uptr}
   1934 :Debug_FuncEnd 
   1935 :Ret 
---ULang:Native:OS_PrintClear: 
   1936 :Debug_FuncStart 
   1937 :Debug_FuncEnd 
   1938 :Ret 
---ULang:Native:OS_Malloc^uptr: 
   1939 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:29
   //Source Line:[Cant Find Line]

   1940 :Debug_LineEnter 
   1941 :Call {__Malloc^uptr}
   1945 :Debug_FuncEnd 
   1946 :Ret 
---ULang:Native:OS_Free^u8[&]: 
   1947 :Debug_FuncStart 

   //File:/OS_Calls.uc
   //Line:36
   //Source Line:[Cant Find Line]

   1948 :Debug_LineEnter 
   1949 :Call {__Free^u8[&]}
   1953 :Debug_FuncEnd 
   1954 :Ret 
---__Log^c[&]imut: 
   1955 :Debug_FuncStart 
   1956 :CppCallNamed "__Log"
   1957 :Debug_FuncEnd 
   1958 :Ret 
---__LogChar^c: 
   1959 :Debug_FuncStart 
   1960 :Ins 209:3,204,204
   1961 :Debug_FuncEnd 
   1962 :Ret 
---__LogBuffer^c[&]imut,uptr: 
   1963 :Debug_FuncStart 
   1964 :Ins 210:3,4,204
   1965 :Debug_FuncEnd 
   1966 :Ret 
---__ReadChar: 
   1967 :Debug_FuncStart 
   1968 :Ins 211:5,204,204
   1969 :Debug_FuncEnd 
   1970 :Ret 
---__Malloc^uptr: 
   1971 :Debug_FuncStart 
   1972 :Malloc [D],[F]
   1973 :Debug_FuncEnd 
   1974 :Ret 
---__Free^u8[&]: 
   1975 :Debug_FuncStart 
   1976 :Free [D]
   1977 :Debug_FuncEnd 
   1978 :Ret 
---__OpenFile^c[&],uptr: 
   1979 :Debug_FuncStart 
   1980 :CppCallNamed "__OpenFile"
   1981 :Debug_FuncEnd 
   1982 :Ret 
---__CloseFile^uptr: 
   1983 :Debug_FuncStart 
   1984 :CppCallNamed "__CloseFile"
   1985 :Debug_FuncEnd 
   1986 :Ret 
---__IsFileOpen^uptr: 
   1987 :Debug_FuncStart 
   1988 :CppCallNamed "__IsFileOpen"
   1989 :Debug_FuncEnd 
   1990 :Ret 
