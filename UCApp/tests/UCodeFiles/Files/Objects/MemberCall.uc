
$Item:
 int A = -1;
 |AddToItem(this&,int Value):
  ret A + Value;

|main[] -> int:
 Item V = Item();
 ret V.AddToItem(1);