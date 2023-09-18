
$StringSpan_t<T>:
 T[&] data;
 uintptr size;

 unsafe |new[this&,imut T[&] data,uintptr size]:
  //this.data = data;
  //this.size = size;
  

$StringSpan =StringSpan_t<char>;
$StringSpan8 =StringSpan_t<utf8>;
$StringSpan16 =StringSpan_t<utf16>;
$StringSpan32 =StringSpan_t<utf32>;


|main[]:
 var Str = "Hello"; //StringSpan
 imut StringSpan Str_other = "Hello"; //StringSpan
 imut StringSpan8  Str8 = "Hello"; //StringSpan8
 imut StringSpan16  Str16 = "Hello"; //StringSpan16
 imut StringSpan32  Str32 = "Hello"; //StringSpan32
 ret Str.size + Str_other.size + Str8.size + Str16.size + Str32.size;