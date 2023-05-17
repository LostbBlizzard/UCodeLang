

$Vec2:
 int X;
 int Y;
 |Update[this&]:
  X++;
 
|Hello[] -> Vec2&;

|main[]:
 Hello().X = 12;
 Hello().X() = 10;

