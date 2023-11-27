int R = 0;

$Drop:
 |drop[this&]:
  R++;

|main[]:

 block:
  Drop[/10] A = [[],[],[],[],[],[],[],[],[],[]];//will use a loop the drop the elements.
 

 ret R == 10;