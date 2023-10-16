
$Vec3f_t<T>:
 T X;
 T Y;
 T Z;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;
 
 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z;

$Vec3i_t<T>:
 T X;
 T Y;
 T Z;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;
 
 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z;

$Vec3 = Vec3f_t<float>;
$Vec3i = Vec3f_t<int>;
