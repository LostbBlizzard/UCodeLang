

|Func[int a] => a;

$Vec2:
 byte x = 1;
 byte y = 2;
 byte z = 3;
 byte w = 4;
static Vec2 StaticVec = [];
thread Vec2 ThreadVec = [];

extern dynamic |Test[int a] -> Vec2;

/*
|main[Vec2& Other] -> Vec2:
 ret Other;

|main2[] => Test(0);



|main3[] -> void;
*/

/*
 ret V.x;
*/

/*
static Vec2 V3 = [];
Vec2 V2 = [];
|main2[]:
 V2.X = 0;
 V3.X = 0;

*/