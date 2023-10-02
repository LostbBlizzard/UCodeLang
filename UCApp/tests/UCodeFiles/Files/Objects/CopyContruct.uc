
int Value1 = 0;
int Value2 = 0;

$CopyType:
 |new[this&]:
  Value1++;
 |new[this&,imut this& other]:
  Value2++;

|main[]:
 CopyType CVal = [];//Contruct
 CopyType CVal2 = CVal;//copy

 ret Value1 == Value2;