
enum Test:
 Value[int],
 Value2[int A],
 Value3[int],

|main[] -> int:
 Test Obj = Test::Value2(5);
 

 //var Item = Test::Value(Obj,out Value);//bad 

 if Test::Value2(Obj,out Value):
  ret Value;

 if Test::Value3(Obj,out Value):
  ret Value;

 ret [];
