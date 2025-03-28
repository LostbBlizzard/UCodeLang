
Tests:

 [Test]
 |Shared_ptr_1[]:
  int$ p = shr int(1);
  ret p.RefCount() == 1;

 
 [Test]
 |Shared_ptr_2[]:
  int$ p = shr int(1);

  uintptr oldcount = p.RefCount();

  int$ other = p;

  ret p.RefCount() == 2 && oldcount == 1;
 
 [Test]
 |Shared_ptr_3[]:
  $Item trait export:
   dynamic |SomeFunc[this&] -> int;
  
  $SomeType[Item] export: 
    int somevalue = 0;
    |new[this&,int value]:
     somevalue = value;
     
    |SomeFunc[this&] -> int:ret somevalue;

  dynamic<Item>$ ptr = unq SomeType(10);

  
  //ret ptr~>SomeFunc() == 10;
