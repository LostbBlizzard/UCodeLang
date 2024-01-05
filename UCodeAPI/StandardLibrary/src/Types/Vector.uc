
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
       _data[i] = move old[i];

      uintptr ptr =unsafe bitcast<uintptr>(old);
      if ptr != uintptr(0):
       unsafe drop(old);
  
  |Clear[this&] -> void:_size = 0;

  |Pop[this&] -> T:
   var r = move this[_size - 1];
   _size--;
   ret r;

  |Remove[this&,uintptr Index] -> T:
   $if compiler::IsDebug():
     if Index >= _size:panic("Index is out of bounds");

   var r = move this[_size - 1];
   

  |Push[this&,imut T& Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;


  |Push[this&,moved T Val] -> void:
   Resize(_size + 1);
   this[_size - 1] = Val;

  |Insert[this&,uintptr Index,imut T& Item] -> void:
   Resize(_size + 1);

   //shift all the elements
   uintptr i = _size - 2;
   while true:
    _data[i+1] =move _data[i];
    
    if i == Index: break;

    i--;

   _data[Index] = Item;

  |Insert[this&,uintptr Index,moved T Item] -> void:
   Resize(_size + 1);

   //shift all the elements
   uintptr i = _size - 2;
   while true:
    _data[i+1] =move _data[i];
    
    if i == Index: break;

    i--;

   _data[Index] = Item;

  //Not required Functions 
  |Append[this&,imut T[:] Val] -> void:
   var oldsize = _size;
   Resize(_size + Val.Size());

   for [uintptr i = 0;i < Val.Size();i++]:
       _data[oldsize+i] = Val[i];

  |Append[this&,moved Span<T> Val] -> void:
   var oldsize = _size;
   Resize(_size + Val.Size());

   for [uintptr i = 0;i < Val.Size();i++]:
       _data[oldsize+i] = move Val[i];

  |Insert[this&,uintptr Index,imut T[:] Val] -> void;
  
  |Insert[this&,uintptr Index,moved Span<T> Val] -> void;

  
  |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");
    
    ret _data[Index];


  //Span

  |[][this&,Range_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  |[][imut this&,Range_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];

  |AsSpan[this&] -> T[:]:ret unsafe [_data,_size];
  |iAsSpan[imut this&] -> imut T[:]:ret unsafe [_data,_size];