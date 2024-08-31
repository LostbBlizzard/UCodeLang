

Tests:

 [Test]
 |Unique_ptr_1[]:
  int^ v = unq int();
  ret v.Get() == 0;

 [Test]
 |Unique_ptr_2[]:
  int^ v = unq int(5);
  ret v.Get() == 5;

 [Test]
 |Unique_ptr_3[]:
  int r = 5;
  int^ Vptr = unq int(r);
  
  Vptr.Get() = 10;

  ret Vptr.Get() == 10;

 [Test]
 |Unique_ptr_4[]:
  var Vptr = unq Vec2(8.0,6.0);

  Vptr~>X = 5;
  Vptr~>Y = 8;


  ret Vptr.Get() == Vec2(5.0,8.0);

 [Test]
 |Unique_ptr_5[]:
  var Vptr = unq Vec2(8.0,6.0);
  var Bptr = unq Vec2(6.0,8.0);
  
  Swap(Vptr,Bptr);

  ret Vptr~>X == Bptr~>Y && Vptr~>Y == Bptr~>X && Bptr~>X == 8;
 
 [Test]
 |Unique_ptr_6[]:
  $Item trait export:
   dynamic |SomeFunc[this&] -> int;
  
  $SomeType[Item] export: 
    int somevalue = 0;
    |new[this&,int value]:
     somevalue = value;
     
    |SomeFunc[this&] -> int:ret somevalue;

  dynamic<Item>^ ptr = unq SomeType(10);

  
  //ret ptr~>SomeFunc() == 10;
