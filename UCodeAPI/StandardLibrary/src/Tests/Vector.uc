
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

  int[:] span = List[uintptr(1)..uintptr(2)];

  ret span.Size() == 1 && span[uintptr(0)] == 15;

 [Test]
 |Vector_3[]:
  int[] List = [];
  List.Push(5);
  List.Push(15);

  List.Insert(1,20);

  ret List[0] == 5 && List[1] == 20 && List[2] == 15;

 [Test]
 |Vector_4[]:
  int[] List = [];
  List.Push(5);
  List.Push(15);

  var a = List.Remove(0);

  ret List[0] == 15 && List.Size() == 1 && a == 5;

 [Test]
 |Vector_5[]:
  int[] List = [];
  List.Push(5);
  List.Push(15);

  var a = List.Pop();

  ret a == 15 && List.Size() == 1;

 [Test]
 |Vector_6[]:
  int[] List = [];
  List.Push(5);

  var a = List[0];

  List.Insert(0,10);

  var b = List[0];

  ret a == 5 && b == 10; 
