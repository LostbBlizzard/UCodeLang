
Tests:

 [Test]
 |Vector_1[]:
  int[] List = [];
  List.Push(5);
  ret List.Size() == 1 && List[uintptr(0)] == 5;

 [Test]
 |Vector_2[]:
  int[] List = [];
  List.Push(5);
  List.Push(15);

  int[:] span = List[uintptr(1)..2];

  ret span.Size() == 1 && span[uintptr(0)] == 15;
