

Tests:

 [Test]
 |Unique_ptr_1[]:
  int^ v = unq int();
  ret v.Get() == 0;

 //[Test]
 |Unique_ptr_2[]:
  //int^ v = unq int(5);
  //ret v.Get() == 5;

 //[Test]
 |Unique_ptr_3[]:
  //int r = 5;
  //int^ Vptr = unq int(r);
  
  //Vptr.Write(10);

  //ret Vptr.ReadPtr() == 10;

 //[Test]
 |Unique_ptr_4[]:
  //var Vptr = unq Vec2(8,6);

  //Vptr~>X = 5;
  //Vptr~>Y = 8;


  //ret r == Vec2(5,8);

 //[Test]
 |Unique_ptr_5[]:
  //var Vptr = unq Vec2(8,6);
  //var Bptr = unq Vec2(6,8);
  
  //swap(Vptr,Bptr);

  //ret Vptr~>X == Bptr~>Y && Vptr~>Y == Bptr~>X && Bptr~>X == 6;