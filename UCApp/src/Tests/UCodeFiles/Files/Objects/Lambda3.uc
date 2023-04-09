$Object:
 int a = 10;
 |Call2[this&] => a;
 |Call[this&]:
  
  var Func1 = |[]:
   var Func2 = |[] => (a + this.a) - Call2();
   ret Func2();
  ;

  ret Func1() - 9;
 |Call3[this&]:
  
  var Func1 = |[]:
   var Func2 = |[] => (Call2() + this.Call2()) - Call2(this);
   ret Func2();
  ;

  ret Func1() - 9;

|main[]:
 Object Item = [];
 ret Item.Call();

|main2[]:
 Object Item = [];
 ret Item.Call();