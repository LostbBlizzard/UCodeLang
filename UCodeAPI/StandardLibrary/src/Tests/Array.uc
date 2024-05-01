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

  ret true;
  //ret vals[0] == 15 && vals[1] == 10 && vals[2] == 5; 

