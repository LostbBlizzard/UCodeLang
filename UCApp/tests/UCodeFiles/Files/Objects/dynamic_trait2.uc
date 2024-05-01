$Object trait:
 dynamic |drop[this&] -> void;



int a = 0;

$Player[Object]:
 |drop[this&]:
  a = 1;

|main[]:
 dynamic<Object> Hello =unsafe new Player();
 
 unsafe drop(Hello);
 ret a;