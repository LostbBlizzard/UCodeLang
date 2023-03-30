
$Vec2:
 int x;
 moved int y;
 |new[this&] -> void;
 |DoSomething[this&,moved this& Test] -> void;


|main[]:
 Vec2 v = [];
 Vec2 v2 = [];
 v.DoSomething(v2);