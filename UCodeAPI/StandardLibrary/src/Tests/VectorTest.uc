
Tests:
 [Test]
 |Vector_1[]:
  int[] List;

  int item = 5;
  List.Push(item);

  ret item.Size() == 1 && item[0] == 5;
