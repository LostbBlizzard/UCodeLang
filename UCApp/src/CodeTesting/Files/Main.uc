

$Dropable:
 |new[this&] -> void;
 |drop[this&] -> void;

$Func_t =|[] -> int;

|func[] -> int;

|main[]:

 Dropable Items = [];
 int z = 10;
 
 var Tep = |[int x,int y]:
  int z = 5;
  var Tep2 = |[int x,int y] => z;
  var TV = Tep2(0,0);
  ret x * y * z * TV;
 ;
 ret Tep(5,5);