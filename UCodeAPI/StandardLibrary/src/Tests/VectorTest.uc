
Tests:
 [Test]
 |Vector_1[]:
  int[] List = [];
  List.Push(5);

  ret List.Size() == 1 && List[0] == 5;
