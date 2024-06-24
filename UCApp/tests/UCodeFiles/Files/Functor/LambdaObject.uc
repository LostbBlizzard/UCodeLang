
|Func<[functor [int a,int b] -> int]FuncType>[FuncType val,int a,int b]:
 ret val(a,b);

|main[]:
 int c = 10;
 ret Func([a,b] => a * b * c,10,5);
