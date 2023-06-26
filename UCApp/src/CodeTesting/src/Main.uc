
/*
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

*/
//static Vec2 StaticVec = [];
//thread Vec2 ThreadVec = [];

/*
|main2[Vec3& Other] -> Vec3:
 ret Other;
*/
extern dynamic |Test[int a,int b] -> int;

/*
|Other[] => 1;
|main[]:
 int a = 0;
 int b = 3;
 a = 5;
 b = 5;
 Test(a,b);

 */
 

|InLineMe[int V] => V + 9;

|HH[int A]:
 A = 1;
 ret InLineMe(A);

|Hm[]:
 HH(5);

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