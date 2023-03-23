



$Loopable:
 $LoopObject:
  |new[this&] -> void;
  |?[this&] -> bool;

 |For[this&] => LoopObject();

|main[] -> var:
 
 for [int a = 0; a < 10; a++];

 for [var& Item : List];

 ret 0;