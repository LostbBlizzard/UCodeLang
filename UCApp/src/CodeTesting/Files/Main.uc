

$Vec2:
 
 int x;
 int y;

 |new[this&]:
  x = 0;
  y = 0;
 |new[this&,int X,int Y]:
  x = X;
  y = Y;

 |Tep[this&,this& Other] -> Vec2:
  ret Vec2(x + Other.x,y + Other.y);


|G[Vec2 V] -> void;

|main[] -> int:
 
 Vec2 X = [];
 Vec2 Y = [1,1];

 G(Vec2());

 ret 1;