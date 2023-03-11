


$Player:
 int Hp;

 |new[this&] -> void:
  Hello(5);

 |drop[this&] -> void;
 
 |Hello[this&,int a] -> void;
 



|main[]:
 Player[&] Player1 = new Player[5];
 
 drop(Player1);
 