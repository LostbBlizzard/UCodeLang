


$FuncPtr = |[] -> int;

|Func[] -> int:
 ret 1;
|Func2[] -> int:
 ret 5;

|main[FuncPtr N] -> var: 
 ret Func();
