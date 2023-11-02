
use API;
use ECS;

$Player[Component]:
 int num = 0;
 |Start[this&]:
  entity().name() = "Hello World";
  entity().position2d() = [2,5];
 
 |Update[this&] -> void:
  num++;
  


|main[] => 0;

