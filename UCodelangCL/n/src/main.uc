//import UCodeLang::{Span,StringSpan,Console};

int temv = 7; 
$VoidType = byte&;

|main[]:
 printtspan("Hello World");

$Test tag;

$Include tag;

[Test]
|func[] => false;

[Test]
|func2[] => true;

|printf1[char V]:
 char& ptr = V;
 putchar(unsafe bitcast<int&>(ptr));

|printtspan[StringSpan V] -> void;
 

[Include("stdio.h")]
extern "c" |printf[char& v] -> int;

[Include("stdio.h")]
extern "c" |putchar[int v] -> int;

[Include("stdio.h")]
extern "c" |free[VoidType ptr] -> void;

[Include("stdio.h")]
extern "c" |malloc[uintptr size] -> VoidType;

$StringSpan_t<T>:
 T[&] data;
 uintptr size;

 unsafe |new[this&,umut T[&] data,uintptr size]:
  this.data = data;
  this.size = size;
  

$StringSpan =StringSpan_t<char>;
$StringSpan8 =StringSpan_t<utf8>;
$StringSpan16 =StringSpan_t<utf16>;
$StringSpan32 =StringSpan_t<utf32>;

$type1 =Vec<int>;
$type2  =Vec<char>;

$Vec<T>:
 uintptr a = 0;
 uintptr b = 0;
 uintptr c = 0;