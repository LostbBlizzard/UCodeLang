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
 
 $Vec3i_t<T>:
  T X;
  T Y;
  T Z;
  |new[this&]:
   X = 0;
   Y = 0;
   Z = 0;
  |new[this&,T x,T y]:
   X = x;
   Y = y;
   Z = y;


 //Typedefs
 $Vec2i = Vec2i_t<int>;
 $Vec2 = Vec2f_t<float>;

 $Vec3i = Vec3i_t<int>;
 $Vec3 = Vec3f_t<float>;