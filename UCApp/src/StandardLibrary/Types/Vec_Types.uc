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
 $Vec2i = Vec2i_t<int>;
 //$Vec2 = Vec2f_t<float>;