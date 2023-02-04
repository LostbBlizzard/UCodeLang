%ULang:
 $Vec2i_t<T>:
  T X;
  T Y;
  |new[this&]:
   X = 0;
   Y = 0;

 $Vec2f_t<T>:
  T X;
  T Y;
  |new[this&]:
   X = 0;
   Y = 0;




 //Typedefs
 $int2 = Vec2i_t<int>;
 $Uintptr2 = Vec2i_t<uintptr>;