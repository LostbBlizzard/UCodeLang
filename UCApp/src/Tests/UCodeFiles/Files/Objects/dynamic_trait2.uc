trait Object;



int a = 0;

$Player[Object]:
 |drop[this&]:
  a = 1;

|main[]:
 dynamic<Object> Hello = new Player();
 
 drop(Hello);
 ret a;