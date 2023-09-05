$Vec2:
 float X;
 float Y;

 |new[this&,float x,float y]:
  X = x;
  Y = y;

|main[] -> float:
 Vec2 V = [0.0,0.0];
 ret V.X;

