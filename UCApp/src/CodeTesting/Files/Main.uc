


$Player:
 int Hp;

 |new[this&] -> void:
  Hello(5);

 |drop[this&] -> void;
 
 |Hello[this&,int a] -> void;
 

|newPlayers[] -> Player[&]:
 ret new Player[5];
|droplayers[Player[&] V]:
 drop(V);

|main[]-> void;
 