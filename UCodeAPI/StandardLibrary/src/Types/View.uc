%ULang:

 //has built in alias in compiler.Ex: int[:]
 $Span<T>:
  private:
   T[&] _Data;
   size_t _Size;


  public:
   |new[this&]:
    _Size = 0;
    _Data &= LowLevel::NullPtr<T[&]>();

   |new[this&,T[&] Data,size_t Size]:
    _Data = Data;
    _Size = Size;

   |Size[umut this&] => _Size;
   |Data[umut this&] => _Data;

   |AsSpan[umut this&,size_t Offset] => AsSpan(Offset,_Size);
   |AsSpan[umut this&,size_t Offset,size_t Size] => this(_Data[Offset],Size);
   |AsSpan[umut this&] => AsSpan(0);

   |Data[this&] => _Data;
 
   |AsSpan[this&,size_t Offset] => AsSpan(Offset,_Size);
   |AsSpan[this&,size_t Offset,size_t Size] => this(_Data[Offset],Size);
   |AsSpan[this&] => AsSpan(0);

   |AsReverseSpan[this] -> RSpan<T>:ret [_Data, _Size];

   |[][this&,size_t Index] -> T&:
    
    $if Compiler::IsDebugBuild: 
       if Index >= _Size:
        Debug::OutOfBounds(Index,_Size);

    ret _Data[Index];

 $RSpan<T>:
  private:
   T[&] _Data;
   size_t _Size;
  public:
  |new[this&]:
    _Size = 0;
    _Data &= LowLevel::NullPtr<T[&]>();

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

  $SpanType = T[:];

  |AsSpan[umut this&] -> umut ViewType;
  |AsSpan[this&] -> ViewType;
  
  |AsSpan[umut this&,size_t Offset,size_t Size] => AsSpan().AsSpan(Offset,Size);
  |AsSpan[this&,size_t Offset,size_t Size] => AsSpan().AsSpan(Offset,Size);


  |AsSpan[this&,size_t Offset] => AsSpan().AsSpan(Offset);
  |AsSpan[umut this&,size_t Offset] => AsSpan().AsSpan(Offset);

  
  
  |[][umut this&,size_t Index] => AsSpan()[Index];
  |[][this&,size_t Index] => AsSpan()[Index];

  |Size[umut this&] => AsSpan().Size();