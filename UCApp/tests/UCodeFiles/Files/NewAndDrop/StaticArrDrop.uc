
int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:

 block:
  Drop[/5] A = [[],[],[],[],[]];//will a hard-coded offsets drop the elements.
 

 ret R == 5;