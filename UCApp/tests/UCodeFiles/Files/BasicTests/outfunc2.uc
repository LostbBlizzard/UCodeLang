
$Object:
 |Value[this&,out int Num] -> bool:
  Num = 10;
  ret true;
 |None[this&,out void Num] -> bool:
  ret false;

|main[] -> int:
 
 Object Item;

 match Item:
  None():ret 0;
  Value(out Value):ret Value;
  invaid:ret 0;

 ret 0;