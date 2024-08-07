
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
  
 [Test]
 |Vector_7[]:
  int r = 0;
  int[] vals = [];
  vals.Push(5);
  vals.Push(10);
  vals.Push(15);
  for [item : vals]:r += item;

  ret r == 30;

 [Test]
 |Vector_8[]:
  int[] vals = [];
  vals.Push(5);
  vals.Push(10);
  vals.Push(15);

  int r = 0;

  for [i,item : vals]:r += item + (i -> int);

  ret r == 33;

 [Test]
 |Vector_9[]:
  int[] vals = [5,10,15]; 

  int r = 0;

  for [i,item : vals]:r += item + (i -> int);

  ret r == 33;

 [Test]
 |Vector_10[]:
  int[] vals = [5,10,15]; 
  int[] copy = vals;

  int r = 0;

  for [i,item : copy]:r += item + (i -> int);

  ret r == 33;

 [Test]
 |Vector_11[]:
  int[] vals = [5,10,15]; 
  int[] copy = move vals;

  int r = 0;

  for [i,item : copy]:r += item + (i -> int);

  ret r == 33 && vals.Size() == 0;



