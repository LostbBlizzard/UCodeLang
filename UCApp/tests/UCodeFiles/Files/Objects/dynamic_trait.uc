$Object trait:
 |GetValue[this&] -> int;

$Object_1[Object]:
 |GetValue[this&] -> int:
  ret 1;

$Object_2[Object]:
 |GetValue[this&] -> int:
  ret 2;

|main[]:
 Object_1 Obj = [];
 dynamic<Object> DynObj =Obj;
 int A =DynObj.GetValue();

 Object_2 Obj2 = [];
 DynObj = Obj2;
 

 int B = DynObj.GetValue();
 ret (A + B) - 2;