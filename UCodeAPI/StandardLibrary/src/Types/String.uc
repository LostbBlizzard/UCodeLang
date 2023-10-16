

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

$String_t<T>:
 //$StringSpan = StringSpan_t<T>;

$String = String_t<char>;
$StringSpan = StringSpan_t<char>;

$String8 = String_t<utf8>;
$StringSpan8 = StringSpan_t<utf8>;

$String16 = String_t<utf16>;
$StringSpan16 = StringSpan_t<utf16>;

$String32 = String_t<utf32>;
$StringSpan32 = StringSpan_t<utf32>;
