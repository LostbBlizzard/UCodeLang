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
 