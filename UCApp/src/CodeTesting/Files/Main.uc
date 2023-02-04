
$Vec2:
 int X;
 int Y;
 |new[this&]:
  X = 0;
  Y = 0;

$Player:
 Vec2 Pos;
 int Hp;
 |new[this&]:
  Pos.X = 0;
  Pos.Y = 5;
  Hp = 0;
  

 |drop[this&];
 |DoSome[this&,int a];
 
|main[bool V]: 

 Vec2 Value = [];
 //Vec2& V2 = Value;

 //Vec2::DoSome(Value,5);
 //Value.DoSome(5);