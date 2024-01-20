
$StringSpan_t<T>:
 T[&] data;
 uintptr size;

 unsafe |new[this&,imut T[&] data,uintptr size]:
  this.data = data;
  this.size = size;
  

$StringSpan =StringSpan_t<char>;
$String8Span =StringSpan_t<utf8>;
$String16Span =StringSpan_t<utf16>;
$String32Span =StringSpan_t<utf32>;


|main[]:
 var Str = "Hello"; //StringSpan
 imut StringSpan Str_other = "Hello"; //StringSpan
 imut String8Span  Str8 = "Hello"; //StringSpan8
 imut String16Span  Str16 = "Hello"; //StringSpan16
 imut String32Span  Str32 = "Hello"; //StringSpan32
 ret Str.size + Str_other.size + Str8.size + Str16.size + Str32.size;