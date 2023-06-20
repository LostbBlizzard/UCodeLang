

|Func[int a] => a;

$Vec2:
 int x;
 int y;
 //int z;

extern dynamic |Test[int a] -> Vec2;


|main[Vec2& Other] -> Vec2:
 ret Other;

 
/*
|main[]:
 var V = Test(0);
 ret V.x;
*/

/*
static Vec2 V3 = [];
Vec2 V2 = [];
|main2[]:
 V2.X = 0;
 V3.X = 0;

*/