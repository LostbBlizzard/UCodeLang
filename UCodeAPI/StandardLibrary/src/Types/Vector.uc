
$Vector<T>[Buffer_t<T>] export:
 private: 
  T[&] _data;
  uintptr _size;
  uintptr _capacity;
 public:
  export |new<[T]Arg...>[this&,Arg args]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
   _capacity = 0;

   Reserve(args.Count());
   $for [item : args]:
     Push(item);

  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
   _capacity = 0;

  export |new[this&,imut this& tocopyfrom]:
   _data = unsafe new T[tocopyfrom._size];
   _size = tocopyfrom._size;
   _capacity = tocopyfrom._size;
   for [uintptr i = 0;i < tocopyfrom._size;i++]:
       unsafe _data[i] = tocopyfrom._data[i];
  
  export |new[this&,moved this tocopyfrom]:
   _data = tocopyfrom._data;
   _size = tocopyfrom._size;
   _capacity = tocopyfrom._capacity;
   
   tocopyfrom._data = unsafe bitcast<T[&]>(0);
   tocopyfrom._size = 0;
   tocopyfrom._capacity = 0;

  export |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_data);
   if ptr != uintptr(0):
   unsafe drop(_data);

  export unsafe |Data[imut this&] -> imut T[&]:ret _data;
  export unsafe |Data[this&] -> T[&]:ret _data;

  export |Size[imut this&] => _size;
  export |Capacity[imut this&] => _capacity;

  export |Resize[this&,uintptr Size] -> void:
    Reserve(Size);
    _size = Size;

  export |Reserve[this&,uintptr Size] -> void:
    if Size > _capacity:
      var oldsize = _size;
      var old = _data;


      _capacity = Size;
      _data = unsafe new T[Size];
      for [uintptr i = 0;i < oldsize;i++]:
       unsafe _data[i] = move old[i];

      uintptr ptr =unsafe bitcast<uintptr>(old);
      if ptr != uintptr(0):
       unsafe drop(old);
  
  export |Clear[this&] -> void:_size = 0;

  export |Pop[this&] -> T:
   var r = move this[_size - 1];
   _size--;
   ret r;

  export |Remove[this&,uintptr Index] -> T:
   $if compiler::IsDebug():
     if Index >= _size:panic("Index is out of bounds");

   var r = move this[Index];
   for [uintptr i = Index + 1;i < _size;i++]:
    this[i-1] = move this[i];
  
   _size--;

   ret r;

  export |Push[this&,imut T& Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;


  export |Push[this&,moved T Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;

  export |Insert[this&,uintptr Index,imut T& Item] -> void:
   $if compiler::IsDebug():
     if Index > _size:panic("Index is out of bounds");

   Resize(_size + 1);

   //shift all the elements
   for [uintptr i = _size - 1;i >= Index;i--]:
    unsafe _data[i] =move _data[i - 1];    

   unsafe _data[Index] = Item;

  export |Insert[this&,uintptr Index,moved T Item] -> void:
   $if compiler::IsDebug():
     if Index > _size:panic("Index is out of bounds");

   Resize(_size + 1);

   //shift all the elements
   for [uintptr i = _size - 1;i >= Index;i--]:
    unsafe _data[i] =move _data[i - 1];    

   unsafe _data[Index] = Item;

  //Not required Functions 
  export |Append[this&,imut T[:] Val] -> void:
   var oldsize = _size;
   Resize(_size + Val.Size());

   for [uintptr i = 0;i < Val.Size();i++]:
       unsafe _data[oldsize+i] = Val[i];

  export |Append[this&,moved Span<T> Val] -> void:
   var oldsize = _size;
   Resize(_size + Val.Size());

   for [uintptr i = 0;i < Val.Size();i++]:
       unsafe _data[oldsize+i] = move Val[i];

  export |Insert[this&,uintptr Index,imut T[:] Val] -> void;
  
  export |Insert[this&,uintptr Index,moved Span<T> Val] -> void;

  
  export |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];
  export |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");
    
    ret unsafe _data[Index];
 