
$Vec3f_t<T>:
 T X;
 T Y;
 T Z;

 $MyVec2f = Vec2f_t<T>;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;

 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z;

 |new[this&,IPar<MyVec2f> pos,T z]:
  X = pos.X;
  Y = pos.Y;
  Z = z;

 |==[imut this&,this& Other]:
  ret this.X == Other.X && this.Y == Other.Y && this.Z == Other.Z;

 |!=[imut this&,this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;
  this.Z += Other.Z;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;
  this.Z -= Other.Z;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec3i_t<T>:
 T X;
 T Y;
 T Z;
 $MyVec2i = Vec2i_t<T>;

 |new[this&]:
  X = 0;
  Y = 0;
  Z = 0;
 

 |new[this&,T x,T y,T z]:
  X = x;
  Y = y;
  Z = z; 
  
  
 |new[this&,IPar<MyVec2i> pos,T z]:
  X = pos.X;
  Y = pos.Y;
  Z = z;


 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y && this.Z == Other.Z;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;
  this.Z += Other.Z;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;
  this.Z -= Other.Z;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;

$Vec3 = Vec3f_t<float>;
$Vec3i = Vec3f_t<int>;
