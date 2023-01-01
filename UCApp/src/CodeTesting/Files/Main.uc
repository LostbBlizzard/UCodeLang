use Player::hi;

%Tep:
 $Vec2:
  int a;

tag LoadDLL:
 int a;

enum Color:
 Red,Green,Blue

$Vec2:
 int a;
 int b;
$Player:
 int8 a;
 int16 b;
 int32 c;
 int64 d;

 uint8 e;
 uint16 f;
 uint32 g;
 uint64 h;

 sint8 i;
 sint16 g;
 sint32 k;
 sint64 l;
 bool G;
 char Hello;
 Color color;

 |this[]:ret;
 |copy[]:ret; 
 |move[]:ret;
 |swap[]:ret;
 |drop[]:ret;
