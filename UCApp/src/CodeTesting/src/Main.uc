

$Vec2:
 int X;
 int Y;

|CoolFunc<T...>[int B,T ParList] -> void:
 $for [var& Item : ParList]:
  var Hello = Item;


|main[]:
 CoolFunc(0,true,'H');

