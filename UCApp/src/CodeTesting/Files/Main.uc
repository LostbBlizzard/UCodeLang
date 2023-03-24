



$Loopable:
 $IntV:
   |new[this&] -> void;
   |drop[this&] -> void;
 $LoopObject:
  |new[this&] -> void;
  
  |->[this&] -> IntV;
  |?[this&] -> bool;
  |drop[this&] -> void;

 |for[this&] => LoopObject();
 |new[this&] -> void;
 |drop[this&] -> void;

|main[] -> var:
 
 Loopable List = [];
 for [var Item : List]:
  ret Item;

 ret 0;