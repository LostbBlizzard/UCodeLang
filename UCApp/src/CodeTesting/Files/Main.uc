


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
 |drop[this&] -> void;

|main[]:

 Player& Tep = new Player();
 Tep.Ptr.Other();

 var H = Tep.Pos;
 
 drop(Tep);

 Player Player1 = [];
 var H = Player1.Ptr.y;
 
 //Player1.Hello();
 //Player::Hello(Player1);