

$Cool:
 int V;
 int C;

$Vec2:
 int X;
 int Y;
 Cool B;
 |Update[this&]:
  //X++;
 |Func[this&] => 10;
 |Func2[this&] -> Vec2&;

|Hello[] -> Vec2&;

|main[]:
 //Hello().Func2() = 12;
 var Item = Hello().Func2().Func2().X.B.V;

