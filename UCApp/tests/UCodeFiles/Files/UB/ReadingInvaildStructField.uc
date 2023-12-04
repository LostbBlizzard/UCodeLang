

$Vec3:
 int X;
 int Y;
 int Z;
 |new[this&] -> void;


|main[]:
 Vec3 v = [];

 ret v.X * 5;//reading uninitialized field X on struct Vec3.