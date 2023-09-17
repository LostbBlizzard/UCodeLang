
$MoveOnly<T>:
 T Base;
 |new[this&]:
  Base = [];
 |new[this&,T base]:
  Base =base;

 |new[this&,imut this& other] = invalid;

|main[]:
 MoveOnly<int> Val = [];
 //MoveOnly<int> Val2 = move Val;//ok
 MoveOnly<int> Val3 = Val;//bad


            
