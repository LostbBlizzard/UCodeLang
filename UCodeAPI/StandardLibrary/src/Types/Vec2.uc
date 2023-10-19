
$Vec2f_t<T>:
 T X;
 T Y;
 
 |new[this&]:
  X = 0;
  Y = 0;

 |new[this&,T x,T y]:
  X = x;
  Y = y;

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec2i_t<T>:
 T X;
 T Y;
 |new[this&]:
  X = 0;
  Y = 0;
 |new[this&,T x,T y]:
  X = x;
  Y = y;

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);
 
 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec2 = Vec2f_t<float>;
$Vec2i = Vec2f_t<int>;
