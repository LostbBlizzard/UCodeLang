


$StringSpan_t<T>:
 T[&] data;
 uintptr size;

 unsafe |Make[umut T[&] data,uintptr size] -> umut this:
  var V = this();
  V.data = data;
  V.size = size;
  ret V;

$StringSpan =StringSpan_t<char>;
$StringSpan8 =StringSpan_t<uft8>;
$StringSpan16 =StringSpan_t<uft16>;
$StringSpan32 =StringSpan_t<uft32>;


|main[]:
 var Str = "Hello"; //StringSpan
 StringSpan Str_other = "Hello"; //StringSpan
 StringSpan8  Str8 = "Hello"; //StringSpan8
 StringSpan16  Str16 = "Hello"; //StringSpan16
 StringSpan32  Str32 = "Hello"; //StringSpan32
 ret Str.size + Str_other.size + Str8.size + Str16.size + Str32.size;