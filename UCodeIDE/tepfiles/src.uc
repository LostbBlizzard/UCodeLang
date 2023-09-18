
$MoveOnly<T>:
 T Base;
 |new[T base]:
  Base =base;

 |new[imut T& base] = invalid;

|main[]:
 MoveOnly<int> Val = [];
 MoveOnly<int> Val2 = move Val;//ok
 MoveOnly<int> Val3 = Val;//bad


            
