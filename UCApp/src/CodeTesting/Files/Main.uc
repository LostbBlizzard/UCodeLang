
$Vec2:
 int x = 0;
 int y = 0;
 |new[this&] -> void;
 |DoSomething[this&,moved this& Test] -> void;


|main[]:
 Vec2 v = [];
 Vec2 v2 = [];
 v.DoSomething(move v2);