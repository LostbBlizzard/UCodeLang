
|Func<[functor [int a,int b] -> int]FuncType>[FuncType val,int a,int b]:
 //ret val(a,b);

|OtherFunc2[int a,int b,int c] -> int:
 ret a * b * c;

|main[]:
 ret Func(OtherFunc2,10,5);
