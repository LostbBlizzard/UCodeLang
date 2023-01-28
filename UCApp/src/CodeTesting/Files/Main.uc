//|V[] -> [int X,int Y];

%UCode:
 
 $Vec2:
  int X;
  int Y;

  |new[this&]:
   X = 0;
   Y = 0;
   
   
|main[]:
 use UCode; 

 Vec2 pos = [];
 Vec2 pos2 = [];

 pos.X = 5;
 pos.Y = 8;
 
 pos2.X =pos.X;
 pos2.Y =pos.Y;

