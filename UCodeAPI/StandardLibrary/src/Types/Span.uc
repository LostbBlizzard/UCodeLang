$Span<T> export:
 private:
  T[&] _data;
  uintptr _size;
 public:
  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;

  export unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;
  export |Size[imut this&] => _size;
  export unsafe |Data[this&] => _data;
  export unsafe |iData[imut this&] => _data;
  

  export |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];
  export |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");
    
    ret unsafe _data[Index];

  export |[][this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];
  export |[][imut this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];