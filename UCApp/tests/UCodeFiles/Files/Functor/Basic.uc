|Func<[functor [int a,int b] -> int]FuncType>[FuncType val,int a,int b]:
 ret val(a,b);

|main[]:
 ret Func([a,b] => a * b,10,5);
