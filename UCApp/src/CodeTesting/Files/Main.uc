

$FuncPtr2 = !|[] -> int;
$FuncPtr = |[] -> int;


|TestFunc[]:
 ret 1;

|main[] -> var:
 
 FuncPtr a = TestFunc;

 //ret a();
