%ULang:

 //has built in alias in compiler.Ex: int[]
 $Vector<T>:
  T[&] _Data;
  size_t _Size;
  size_t _Capacity;

  //new
  |new[this&]:
   _Size = 0;
   _Capacity = 0;

  |new[View<T> Data] -> void;

  //umut
  |Size[umut this&] => _Size;
  |Data[umut this&] => _Data;
  |Capacity[umut this&] => _Capacity;

  |SubView[umut this&,size_t Offset] => SubView(Offset,_Size);
  |SubView[umut this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
  |View[umut this&] => SubView(0);

  //mut
  |Data[this&] => _Data;
  
  |SubView[this&,size_t Offset] => SubView(Offset,_Size);
  |SubView[this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
  |View[this&] => SubView(0);

  |Add[this&,umut T& Item] -> void;
  |RemoveIndex[size_t Index] -> void;
  
  |Resize[this&,size_t Offset] -> void;
  |Clear[this&] -> void;