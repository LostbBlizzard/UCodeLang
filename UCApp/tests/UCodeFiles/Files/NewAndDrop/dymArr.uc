int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:
 Drop[&] Item =unsafe new Drop[5];
 unsafe drop(Item);

 ret R == 5;