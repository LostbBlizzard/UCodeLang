


/*

$Player[Component]:

 |Start[this&]:
  entity().name() = "Hello World";
  entity().position() = [2,5];
 
 |Update[this&] -> void;
  
*/

|main[]:
 var H = unsafe new int[1];


 H[0] = 1;
 int Tep = H[0];
 
 unsafe drop(H);
 
 ret Tep;