
$Vec2f_t<T> export:
 T X;
 T Y;
 
 export |new[this&]:
  X = 0;
  Y = 0;

 export |new[this&,T x,T y]:
  X = x;
  Y = y;

 export |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 export |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 export |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 export |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 export |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 export |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec2i_t<T> export:
 T X;
 T Y;
 export |new[this&]:
  X = 0;
  Y = 0;
 export |new[this&,T x,T y]:
  X = x;
  Y = y;

 export |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 export |!=[imut this&,imut this& Other]:
  ret !(this == Other);
 
 export |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 export |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 export |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 export |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec2 export = Vec2f_t<float>;
$Vec2i export = Vec2i_t<int>;
