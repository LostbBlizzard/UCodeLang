

$Dropable:
 |new[this&] -> void;
 |drop[this&] -> void;

$Func_t =|[] -> int;

|func[] -> int;

|main[]:

 Dropable Items = [];
 int z = 10;
 
 var Tep = |[int x,int y] => x * y;

 ret Tep(5,5);