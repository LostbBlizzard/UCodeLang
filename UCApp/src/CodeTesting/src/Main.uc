

|Func[int a] => a;

$Object;

$Vec3:
 int x = 1;
 int y = 2;
 int z = 3;

$Vec2:
 int x = 1;
 int y = 2;
 //byte w = 4;
static Vec2 StaticVec = [];
thread Vec2 ThreadVec = [];

extern dynamic |Test[int a] -> Vec3;

//|main2[] => Test(0);

|main2[Vec3& Other] -> Vec3:
 ret Other;


//|Other[] => 1;
|main[] => 8;
 


//|main3[] -> void;


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