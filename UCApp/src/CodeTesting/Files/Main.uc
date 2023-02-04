use ULang;


$Player:
 int2 Pos;
 int Hp;
 |new[this&]:
  Pos.X = 0;
  Pos.Y = 5;
  Hp = 0;
  

 |drop[this&];
 |DoSome[this&,int a];
 
|main[bool V];