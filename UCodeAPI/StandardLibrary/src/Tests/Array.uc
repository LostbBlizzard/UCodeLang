Tests:
 
 [Test]
 |Array_1[]:
  int[3] vals = [];

  vals[0] = 5;
  vals[1] = 10;
  vals[2] = 15;

  int r = 0;

  for [i,item : vals]:r += item + (i -> int);

  ret r == 33; 
 
 [Test]
 |Array_2[]:
  int[3] vals = [5,10,15];

  int r = 0;

  for [i,item : vals]:r += item + (i -> int);

  ret r == 33; 
 [Test]
 |Array_3[]:
  int[3] vals = [5,10,15];

  vals.Reverse();

  ret vals[0] == 15 && vals[1] == 10 && vals[2] == 5; 

 [Test]
 |Array_4[]:
  int[4] vals = [5,10,15,20];

  var A = vals.Find([x] => x == 5);

  if Opt(A,out item):
   ret item == 5;

  ret false;

 [Test]
 |Array_5[]:
  int[4] vals = [5,10,15,20];

  var A = vals.FindIndex([x] => x == 20);

  if Opt(A,out item):
   ret item == 3;

  ret false;
