
$List:
 int[/1] data;
 |[][this&,uintptr index] -> int&: ret unsafe data[0];

|main[]:
 
 List val = [];
 var& ptr = val[0];
 
 ptr = 5;

 ret val[0];
