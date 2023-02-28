

$FuncPtr2 = !|[] -> int;
$FuncPtr = |[] -> int;


|TestFunc[Item V]:
 V =Item::Green;
 ret V;

|main[] -> var:
 
 //FuncPtr a = TestFunc;
 TestFunc(Item::Red);

 //ret a();
