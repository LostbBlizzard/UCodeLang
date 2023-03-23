



$Loopable:
 $LoopObject:
  |new[this&] -> void;
  |t[this&] -> bool;

 |For[this&] => LoopObject();

|main[] -> var:
 
 for [int a = 0; 10 > a; a++];

 //for [var& Item : List];

 ret 0;