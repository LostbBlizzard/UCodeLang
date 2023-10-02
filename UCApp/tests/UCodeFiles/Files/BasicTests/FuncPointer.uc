$FuncPointer = |[int a,int b] -> int;

|Add[int a,int b] => a + b;
|Mult[int a,int b] => a * b;

|main[]:
 FuncPointer V = Add;
 int a = 99;
 a = V(5,5);

 V = Mult;

 a = V(a,0);

 ret a;


