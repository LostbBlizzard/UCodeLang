$FuncPtr = |[] -> int;
$Struct:
 FuncPtr func;

 |new[this&,FuncPtr ptr]:
  func = ptr;
 |()[this&] -> int:
   ret func();


|func1[] -> int: ret 10;
|func2[] -> int: ret 20;

|main[]:
  Struct val1 = [func1];
  Struct val2 = [func2];

  ret (val1() + val2()) == 30; 


