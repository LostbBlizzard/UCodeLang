|main[]:
 var Func = |[int a,int b] => a + b;

 ret Func(-5,6);

|main2[]:
 int Z = 10;
 var Func = |[int a,int b] => a + b * 10;
 ret Func(-5,6) / 10;