
$Droping:
 int D = []; 
 |drop[this&] -> void;

$Test:
 Droping X = [];
 int Y = [];
 |+[this&,this& Other] -> this;

var __B = Test() + Test();
int V = 5 * 7;

|Add[Test& V] -> void;

|main[]:
  Add(Test());
