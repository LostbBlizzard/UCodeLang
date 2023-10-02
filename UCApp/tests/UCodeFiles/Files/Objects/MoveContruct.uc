

int Value1 = 0;
int Value2 = 0;

$MoveType:
 |new[this&]:
  Value1++;
 |new[this&,moved this& other]:
  Value2++;

|main[]:
 MoveType CVal = [];//Contruct
 MoveType CVal2 = move CVal;//Move

 ret Value1 == Value2;