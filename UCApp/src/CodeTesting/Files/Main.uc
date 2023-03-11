


$Vec2i:
 int x;
 int y;

 |Other[this&]:
  ret 1;

$Player:
 Vec2i Pos;
 int Hp;

 |new[this&] -> void:
  Hello(5);

 |drop[this&] -> void;
 
 |Hello[this&,int a] -> void;
 



|main[]:
 Player Player1 = [];
 
 Player1.Hello(7);
 