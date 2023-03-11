


$Vec2i:
 int x;
 int y;

 |Other[this&]:
  ret 1;

$Player:
 Vec2i Pos;
 int Hp;

 |new[this&] -> void:
  Hp++;

 |drop[this&] -> void;
 
 |Hello[this&] -> void;
 



|main[]:
 Player Player1 = [];
 
 Player1.Hello();
 