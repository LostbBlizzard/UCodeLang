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
