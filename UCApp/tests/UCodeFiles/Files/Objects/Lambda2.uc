|main[]:
 int z = 2;
 var Func = |[int a,int b]:
   var FuncTwo = |[int x] => a * z; 
   ret FuncTwo(a + b) / 2;
 ;
 ret Func(-5,6);


$Object:
 int a = 10;
 |Call2[this&] => a;
 |Call[this&]:
  
  var Func1 = |[] => a * Call2();
  var Func2 = |[] => 5 * this.Call2();
  var Func3 = |[] => 5 * Call2(this);
  var Func4 = |[] => ((a - this.a * 2) / 2); 
  ret (Func1() / 100) + (Func2() - Func3()) + Func4();

|main2[]:
 Object Item = [];
 ret Item.Call();