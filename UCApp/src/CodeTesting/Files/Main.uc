

$Vec2:
 
 int x;
 int y;

 |new[this&]:
  x = 0;
  y = 0;
 |new[this&,int X,int Y]:
  x = X;
  y = Y;
  DoSomeThing();

 |DoSomeThing[]:
  x = 0;


|main[] -> int:
 
 Vec2 X = [];
 Vec2 Y = [1,1];

 Y.DoSomeThing();

 ret 1;