
$Vec2:
 int X = 0;
 int Y = 0;

$Entity:
 Vec2 _pos;
 |Pos[this&,Vec2 val] -> void:_pos = val;
 |Pos[this&] -> Vec2:ret _pos;

Entity entity = [];

|func[] -> Entity&: ret entity;

|main[]:
 var pos = func().Pos();

 pos.X += 1;
 pos.Y += 1;

 func().Pos(pos);

 var pos2 = func().Pos();
 ret pos2.X + pos2.Y;
