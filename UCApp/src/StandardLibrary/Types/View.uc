%ULang:
 $View<T>:
  T[&] _Data;
  size_t _Size;

  //new
  |new[this&]:
   _Size = 0;

  |new[this&,T[&] Data,size_t Size]:
   _Data = Data;
   _Size = Size;

  //umut
  |Size[umut this&] => _Size;
  |Data[umut this&] => _Data;

  |SubView[umut this&,size_t Offset] => SubView(Offset,_Size);
  |SubView[umut this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
  |View[umut this&] => SubView(0);

  //mut
  |Data[this&] => _Data;
 
  |SubView[this&,size_t Offset] => SubView(Offset,_Size);
  |SubView[this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
  |View[this&] => SubView(0);

 //$Span<T,X>;//compile time version