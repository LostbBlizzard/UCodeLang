
$Vector<T>:
 private: 
  T[&] _data;
  uintptr _size;
  uintptr _capacity;
 public:
  |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
   _capacity = 0;
  |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_data);
   if ptr != uintptr(0):
   unsafe drop(_data);

  unsafe |iData[imut this&] -> imut T[&]:ret _data;
  unsafe |Data[this&] -> T[&]:ret _data;

  |Size[imut this&] => _size;
  |Capacity[imut this&] => _capacity;

  |Resize[this&,uintptr Size] -> void:
    Reserve(Size);
    _size = Size;

  |Reserve[this&,uintptr Size] -> void:
    if Size > _capacity:
      var oldsize = _size;
      var old = _data;


      _capacity = Size;
      _data = unsafe new T[Size];
      for [uintptr i = 0;i < oldsize;i++]:
       _data[i] = old[i];

      uintptr ptr =unsafe bitcast<uintptr>(old);
      if ptr == uintptr(0):
       unsafe drop(old);
  
  |Clear[this&] -> void:_size = 0;

  |Pop[this&] -> T;
  |Remove[this&,uintptr Index] -> T;

  |Push[this&,imut T& Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;


  |Push[this&,moved T Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;

  |Insert[this&,uintptr Index,imut T& Item] -> void;
  |Insert[this&,uintptr Index,moved T Item] -> void;

  //Not required Functions 
  |Append[this&,imut T[:] Val] -> void;
  |Append[this&,moved T[:] Val] -> void;

  
  |[][this&,uintptr Index] -> T&:ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _data[Index];


  //Span

  |[][this&,Range_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  |[][imut this&,Range_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];

  |AsSpan[this&] -> T[:]:ret unsafe [_data,_size];
  |iAsSpan[imut this&] -> imut T[:]:ret unsafe [_data,_size];