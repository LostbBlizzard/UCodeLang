


$Vec2i:
 int x;
 int y;
 |Other[this&]:
  ret 1;

$Player:
 |new[this&] -> void;
 Vec2i Pos;
 int Hp;

 Vec2i& Ptr;

 |Hello[this&]:
  ret 0;

|main[]:

 Player& Tep = new Player();
 Tep.Ptr.Other();

 Player Player1 = [];
 var H = Player1.Ptr.y;
 
 //Player1.Hello();
 //Player::Hello(Player1);