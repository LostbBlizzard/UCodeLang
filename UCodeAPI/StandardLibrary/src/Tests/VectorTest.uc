
Tests:
 //[Test]
 |Vector_1[]:
  int[] List = [];
  List.Push(5);
  ret List.Size() == 1 && List[0 -> uintptr] == 5;

 //[Test]
 |Vector_2[]:
  int[] List = [];
  List.Push(5);
  List.Push(15);

  int[:] span = List[1..2];

  ret span.Size() == 1 && span[0 -> uintptr] == 15;
