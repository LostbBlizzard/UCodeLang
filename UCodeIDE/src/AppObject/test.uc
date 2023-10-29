

/*
|main[] => factorial(10);


|factorial[int n] -> int: 
  if n == 0:
    ret 1;
  else:
    ret factorial(n-1) * n;
*/

/*

thread int V1 = 0;
thread int V2 = 0;
thread int V3 = 0;

thread int V4 = 0;
thread int V5 = 0;
thread int V6 = 0;



|coolp[int a,int b,int c]:
 
 int m1 = a;
 int m2 = b;
 int m3 = c;

 a++;
 b++;
 c++;

 V1 = m1;
 V2 = m2;
 V3 = m3;

 
 V4 = a;
 V5 = b;
 V6 = c;

 ret a;

*/

$Optional<T> enum:
 Opt[T Val],
 NoOpt,


|main[] => match main2():
 Opt(out val) => val;
 NoOpt => 0;
;

|main2[] -> int?:
 int? V = Opt(5);

 int Val = ?V;

 int X = Val + 2;

 ret Opt(X);
 

|NullOp[]:
 int&? V = NoOpt;