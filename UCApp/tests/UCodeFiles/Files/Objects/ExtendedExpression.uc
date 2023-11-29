

$Vec2:
 int X = 0;
 int Y = 0;

$Player:
 Vec2 _Pos;

 |Pos[this&] -> Vec2&:
  ret _Pos;

 |This[this&] -> this&:ret this;

|Hello[] -> Player&:
 static Player P = [];
 ret P;

|main[]:
 Hello().Pos().X = 1;
 
 Hello().This()._Pos.X++;
 
 ret Hello().This()._Pos.X;
