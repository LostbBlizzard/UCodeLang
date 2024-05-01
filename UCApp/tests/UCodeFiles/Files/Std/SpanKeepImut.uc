$StringSpan_t<T>:
 T[&] _data;
 uintptr _size;
 
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
 
 unsafe |new[this&,T[&] Data,uintptr Size]:
  _data = Data;
  _size = Size;

$StringSpan = StringSpan_t<char>;
|main[]:
 StringSpan v = "Hello World";//cant lose imut is ref buffer types.
