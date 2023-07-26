/*

|main[] -> int:

 int A = 0;

 int& V = A;
 V = 1;

 ret V;

|main2[] -> int:

 int A = 1;

 int& V = A;
 int H = A;
 ret H;



|main3[int A] -> int&:
 int& R = A;
 //A = 0;
 A = 15;
 ret R;
 
*/


|main[]:

  int A = 0 + 7;
  A+=5;
  ret A;



/*
|main2[]:
  int a = 4;
  int b = 8;
  int c = 12;
  ret a + b + c;
*/