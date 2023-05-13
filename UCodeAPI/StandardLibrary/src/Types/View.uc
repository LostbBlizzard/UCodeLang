%ULang:

 //has built in alias in compiler.Ex: int[:]
 $View<T>:
  private:
   T[&] _Data;
   size_t _Size;


  public:
   |new[this&]:
    _Size = 0;
    _Data &= bitcast<T[&]>(0);

   |new[this&,T[&] Data,size_t Size]:
    _Data = Data;
    _Size = Size;

   |Size[umut this&] => _Size;
   |Data[umut this&] => _Data;

   |SubView[umut this&,size_t Offset] => SubView(Offset,_Size);
   |SubView[umut this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
   |View[umut this&] => SubView(0);

   |Data[this&] => _Data;
 
   |SubView[this&,size_t Offset] => SubView(Offset,_Size);
   |SubView[this&,size_t Offset,size_t Size] => View<T>(_Data[Offset],Size);
   |AsView[this&] => SubView(0);

   |AsReverseView[this] -> RView<T>:ret [_Data, _Size];

 $RView<T>:
  |new[this&]:
    _Size = 0;
    _Data &= bitcast<T[&]>(0);

  |new[this&,T[&] Data,size_t Size]:
    _Data = Data;
    _Size = Size;

 trait Buffer<T>:

  /*
  $if type(T == char)
   $ViewType = StringView;
  $else
   $ViewType = T[:];
  */

  $ViewType = T[:];

  |AsView[umut this&] -> umut ViewType;
  |AsView[this&] -> ViewType;
  
  |SubView[umut this&,size_t Offset,size_t Size] => AsView().SubView(Offset,Size);
  |SubView[this&,size_t Offset,size_t Size] => AsView().SubView(Offset,Size);


  |SubView[this&,size_t Offset] => AsView().SubView(Offset);
  |SubView[umut this&,size_t Offset] => AsView().SubView(Offset);

  
  
  |[][umut this&,size_t Index] => AsView()[Index];
  |[][this&,size_t Index] => AsView()[Index];


  |Size[umut this&] => AsView().Size();