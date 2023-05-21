
/*

|main[]:
 int a = 10;
 match a:
  1:a = 10;
  2:a = 20;
  3:a = 30;
  invalid:a = 0;
   
 
 int V = match a:
  50 => 500;
  70 => 700;
  invalid => 100;
 ;

*/

eval char[/1] VarableName = "Y";

$Cool:
 int X;
 int Y;

|main[]:
 
 Cool V = [];
 //eval typeinfo V_type = type(Cool);
 eval typeinfo V_type2 = type(int);

 bind(V_type2) ItemB = 50;
 V.$VarableName = 5;
