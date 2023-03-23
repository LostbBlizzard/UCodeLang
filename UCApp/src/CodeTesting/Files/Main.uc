



$Loopable:
 $LoopObject:
  |new[this&] -> void;
  |t[this&] -> bool;

 |for[this&] => LoopObject();

|main[] -> var:
 
 Loopable List;
 for [var& Item : List];

 ret 0;