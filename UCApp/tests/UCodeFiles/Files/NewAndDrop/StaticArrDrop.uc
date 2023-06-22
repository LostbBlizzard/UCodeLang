
int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:
 Drop[/5] = [[],[],[],[],[]];
 

 ret R == 5;