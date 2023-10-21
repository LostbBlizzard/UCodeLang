
$Vector<T>:
 private: 
  T[&] _data;
  uintptr _size;
  uintptr _capacity;
 //$Span = Span<T>;
 public:
  |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
   _capacity = 0;

  unsafe |Data[imut this&] -> imut T&:ret _data;
  unsafe |Data[this&] -> T&:ret _data;

  |Size[imut this&] => _size;
  |Capacity[imut this&] => _capacity;

  |Resize[this&,uintptr Size] -> void;
  |Reserve[this&,uintptr Size] -> void;
  |Clear[this&] -> void;

  |Pop[this&] -> T;
  |Remove[this&,uintptr Index] -> T;

  |Push[this&,imut T& Val] -> void;
  //|Push[this&,moved T Val] -> void;

  |Insert[this&,uintptr Index,imut T& Item] -> void;
  //|Insert[this&,uintptr Index,moved T Item] -> void;

  //Not required Funcions 
  |Append[this&,imut T[:] Val] -> void;
  //|Append[this&,moved T[:] Val] -> void;

  
  |[][this&,uintptr Index] -> T&:ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _data[Index];
