$Vec2:
 float X = 0;
 float Y = 0;
 |new[this&] -> void:
  ret;
 |new[this&,float x,float y]:
  X = x;
  Y = y;

 |+[this&,this& Other] -> this:
  ret [X + Other.X,Y + Other.Y];


|test[] -> Vec2:
 Vec2 A = [1.0,1.0];
 Vec2 B = [-1.0,-1.0];
 ret B + A;

  