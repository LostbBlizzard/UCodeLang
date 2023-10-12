/*
$FuncPointer = |[Vec2&] -> float;
$FuncPointer2 = |[Vec3&] -> float;

$Vec3:
 float X =0;
 float Y =0;
 float Z =1;
 |new[this&] -> void;

 |Update[this&] => Z;

$Vec2:
 float X;
 float Y = 3;

 |Update[this&] => Y;


|main[]:
 Vec3 A = [];
 Vec2 B = [];

 FuncPointer Func = Vec2::Update;
 float a = 2;

 a -= Func(B);

 FuncPointer2 Func2 = Vec3::Update;

 a += Func2(A);

 ret a;

*/

$FuncPointer = |[int a,int b] -> int;

|Add[int a,int b] => a + b;
|Mult[int a,int b] => a * b;

|main[]:
 FuncPointer V = Add;
 int a = 99;
 a = V(5,5);

 V = Mult;

 a = V(a,0);

 ret a;
