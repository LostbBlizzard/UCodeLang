
// 
// Packaged FilePath:Math/Math.uc
// 

%ULang:
 $Math:
  |Sqrt<T>[T Value] -> T;

// 
// Packaged FilePath:Types/Array.uc
// 

%ULang:
 $Array<T,X>;// X is an uintptr

// 
// Packaged FilePath:Types/Optional.uc
// 

%ULang:
 $Nothing;
 
 $Optional<T>:
  bool _HasValue;
  T _Value;
  
  |new[this&,Nothing Value]:SetValue(Value);
  |new[this&]:SetValue(Nothing());
  |new[this&,umut T& Value]:SetValue(Value);


  |SetValue[umut this&,umut T& Value]:
   _HasValue = true;
   _Value =Value;

  |SetValue[umut this&,Nothing Value]:
   _HasValue = false;
  
  |HasValue[umut this&] => _HasValue;
  |Value[umut this&] => _Value;

// 
// Packaged FilePath:Types/Primitives.uc
// 

%ULang:
  $Size_t = uintptr;

  |ToString<T>[];//for ints 
  |ToString<T>[];//for floats 

// 
// Packaged FilePath:Types/Span.uc
// 

%ULang:
 $Span<T>;
 $Span<T,X>;//compile time vir

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
 $Vector<T>;

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
