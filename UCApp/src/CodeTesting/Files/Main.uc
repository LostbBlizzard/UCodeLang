

$FuncPtr2 = !|[] -> int;
$FuncPtr = |[] -> int;

|DropData[int& V]:
 drop(V);

|main[int& V] -> var:
 DropData(new int());
