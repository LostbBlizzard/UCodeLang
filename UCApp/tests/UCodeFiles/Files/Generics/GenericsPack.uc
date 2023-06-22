
|CoolFunc<T...>[T ParList] -> int:
 int R = 0;
 $for [var& Item : ParList]:
  var Hello = Item;
  R++;

 ret R;


|main[] => CoolFunc(0,true,'H');
