
$Vec2:
 int X = 0;
 int Y = 0;

$Entity: 
 Vec2 _pos;
 |Pos[this&,Vec2 val] -> void:_pos = val;
 |Pos[this&] -> Vec2:ret _pos;

|main[]:
 Entity val = [];

 val._pos.X = 1;
 val._pos.Y = 1;

 var op = val.Pos();

 op.X += 1;
 op.Y += 1;

 val.Pos(op);
 
 var current = val.Pos();

 ret current.X == 2 && current.Y == 2;
