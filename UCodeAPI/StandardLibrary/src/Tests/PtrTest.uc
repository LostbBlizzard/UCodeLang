
Tests:
 
 [Test]
 |Ptr_1[]:
  int r = 5;
  Ptr<int> Vptr = ptr(r);

  Vptr.Get() = 10;


  ret r == 10;
 
 [Test]
 |Ptr_2[]:
  int r = 5;
  Ptr<int> Vptr = ptr(r);

  Vptr.Write(10);


  ret r == 10;


 [Test]
 |Ptr_3[]:
  int r = 5;
  int b = 8;
  Ptr<int> Vptr = ptr(r);

  Vptr.ReassignTo(b);

  Vptr.Write(7);

  ret b == 7;

 [Test]
 |Ptr_4[]:
  int r = 5;
  Ptr<int> Vptr = ptr(r);

  r += 5;

  ret Vptr.Read() == 10;


 [Test]
 |Ptr_5[]:

  Vec2 r = [8,6];
  var Vptr = ptr(r);

  Vptr~>X = 5;
  Vptr~>Y = 8;


  ret r == Vec2(5,8);
