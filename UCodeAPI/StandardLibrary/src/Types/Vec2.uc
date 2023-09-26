
$Vec2f_t<T>:
 T X;
 T Y;
 
 |new[this&]:
  X = 0;
  Y = 0;

 |new[this&,T x,T y]:
  X = x;
  Y = y;

$Vec2i_t<T>:
 T X;
 T Y;
 |new[this&]:
  X = 0;
  Y = 0;
 |new[this&,T x,T y]:
  X = x;
  Y = y;

$Vec2 = Vec2f_t<float>;
$Vec2i = Vec2f_t<int>;
