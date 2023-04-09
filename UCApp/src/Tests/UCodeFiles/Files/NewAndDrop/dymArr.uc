int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:
 Drop[&] Item = new Drop[5];
 drop(Item);

 ret R == 5;