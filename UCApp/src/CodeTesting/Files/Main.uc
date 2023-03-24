



$Loopable:
 $LoopObject:
  |new[this&] -> void;
  
  |->[this&] -> int;
  |?[this&] -> bool;

 |for[this&] => LoopObject();

|main[] -> var:
 
 Loopable List;
 for [var& Item : List];

 ret 0;