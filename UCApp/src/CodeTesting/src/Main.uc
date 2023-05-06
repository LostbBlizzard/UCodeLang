


$ItenBuffer:
 int A;
 int B;
 int C;
 int D;
 int E;

$Vec2:
 int X;
 int Y;

|main[]:
 var Ptr = bitcast<int&>(0);

 Ptr &= new int();
 
 ret Ptr;

