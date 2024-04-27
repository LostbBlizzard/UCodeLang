
$Vec2i export:
 int X;
 int Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |==[imut this&,imut this& Other] -> bool:
  ret this.X == Other.X && this.Y == Other.Y;

 |new[this&]:
  X = 0;
  Y = 0;
 |new[this&,int x,int y]:
  X = x;
  Y = y;

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

|main[]:
 Vec2i v = [2,3] + [4,2];
 ret v == [6,5];

|main2[]:
 Vec2i v = [2,3];
 v += [4,3] + [1,2];
 ret v == [7,8];
