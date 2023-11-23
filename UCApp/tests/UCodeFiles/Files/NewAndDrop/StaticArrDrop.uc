
int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:

 block:
  Drop[/5] A = [[],[],[],[],[]];
 

 ret R == 5;