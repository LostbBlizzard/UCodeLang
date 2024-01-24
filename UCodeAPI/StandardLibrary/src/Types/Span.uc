$Span<T>:
 private:
  T[&] _data;
  uintptr _size;
 public:
  |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;

  unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;
  |Size[imut this&] => _size;
  unsafe |Data[this&] => _data;
  unsafe |iData[imut this&] => _data;
  

  |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");
    
    ret unsafe _data[Index];

  |[][this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];
  |[][imut this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];