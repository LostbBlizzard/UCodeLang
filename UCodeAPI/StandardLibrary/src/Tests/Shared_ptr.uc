
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
 
