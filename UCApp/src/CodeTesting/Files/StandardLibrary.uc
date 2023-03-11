
// 
// Packaged FilePath:Math/Math.uc
// 

%ULang:
 $Math:
  |Sqrt<T>[T Value] -> T;

// 
// Packaged FilePath:Types/alias.uc
// 

%ULang:
 $size_t = uintptr;

// 
// Packaged FilePath:Types/Array.uc
// 

%ULang:
 $Array<T,X>;// X is an uintptr

// 
// Packaged FilePath:Types/Optional.uc
// 

%ULang:
 
 
 $Optional<T>:
  bool _HasValue;
  T _Value;
  

  |new[this&] => SetValue();
  
  |new[this&,umut T& Value] => SetValue(Value);


  |SetValue[umut this&,umut T& Value]:
   _HasValue = true;
   _Value =Value;

  |SetValue[umut this&,null_t Value]:
   _HasValue = false;
  
  |HasValue[umut this&] => _HasValue;

  |Value[umut this&] => _Value;

  |Value[this&] => _Value;

// 
// Packaged FilePath:Types/Primitives.uc
// 

%ULang:
  $Size_t = uintptr;

  |ToString<T>[];//for ints 
  |ToString<T>[];//for floats 

// 
// Packaged FilePath:Types/String.uc
// 

%ULang:
 
 $String;



 $StringView = Span<char>;

// 
// Packaged FilePath:Types/Vector.uc
// 

%ULang:
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

// 
// Packaged FilePath:Types/Vec_Types.uc
// 

%ULang:
 $Vec2i_t<T>:
  T X;
  T Y;
  |new[this&]:
   X = 0;
   Y = 0;
  |new[this&,T x,T y]:
   X = x;
   Y = y;

 $Vec2f_t<T>:
  T X;
  T Y;
  |new[this&]:
   X = 0;
   Y = 0;
  |new[this&,T x,T y]:
   X = x;
   Y = y;




 //Typedefs
 $Vec2i = Vec2i_t<int>;
 //$Vec2 = Vec2f_t<float>;

// 
// Packaged FilePath:Types/View.uc
// 

%ULang:
 $View<T>:
  T[&] _Data;
  size_t _Size;

  //new
  |new[this&]:
   _Size = 0;

  |new[T[&] Data,size_t Size]:
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
