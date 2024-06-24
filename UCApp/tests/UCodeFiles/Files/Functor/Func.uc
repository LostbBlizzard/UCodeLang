
|Func<[functor [int a,int b] -> int]FuncType>[FuncType val,int a,int b]:
 ret val(a,b);

|OtherFunc[int a,int b] -> int:
 ret a * b;

|main[]:
 ret Func(OtherFunc,10,5);