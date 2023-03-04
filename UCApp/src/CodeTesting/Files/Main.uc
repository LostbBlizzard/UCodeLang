

$FuncPtr2 = !|[] -> int;
$FuncPtr = |[] -> int;


|main2[int& V] -> var&:
 V += V;
 ret 6;

|main[int& V] -> int:
 int& v = 0;
 ret v;
