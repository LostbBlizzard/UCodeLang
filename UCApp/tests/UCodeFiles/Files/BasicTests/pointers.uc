
|main[] -> int:

 int A = 0;

 int& V = A;
 V = 1;

 ret V;

|main2[] -> int:

 int A = 1;

 int& V = A;
 int H = V;
 ret H;