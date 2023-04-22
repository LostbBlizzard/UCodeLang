

$Buffer:
 int A=0;
 int B=0;
 int C=0;

//int B = 10;

$Player:
 int X = 0;
 int Y = 0;
 int Z = 0;
 |Update[this&,int a]:
  Z += a;



|Main2[]:
 //Buffer B = [];
 //ret main(B,10,2);

|main[Buffer buffer,int D,int E] -> var:
 ret D / E;



  
