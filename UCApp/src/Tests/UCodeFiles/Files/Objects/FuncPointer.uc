$FuncPointer = [var&] -> int;

$Vec3:
 float X =0;
 float Y =0;
 float Z =1;
 |new[this&];

 |Update[] => Z:

$Vec2:
 float X;
 float Y = 3;

 |Update[] => Y:

|main[]:
 Vec3 A = [];
 Vec2 B = [];

 FuncPointer Func = Vec2::Update;
 int a = 2;

 a -= Func(B);

 Func = Vec3::Update;

 a += Func(A);

 ret a;