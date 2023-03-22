

$Vec2i:
 int X;
 int Y;
 |new[this&] -> void;
 |new[this&,int x,int y]:
  X = x;
  Y = y;

 |+[this&,this& Other] -> this:
  ret [X + Other.X,Y + Other.Y];
 
 |[][this&,int Index] -> int:
  ret Index;
 
 |()[this&,int Par1,int Par2]:
  ret Par1 + Par2;

|main[] -> var:
 Vec2i A = [1,1];
 Vec2i B = [2,2];

 ret B + A;