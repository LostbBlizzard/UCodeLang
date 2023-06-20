



$Vec2:
 int x;
 int y;
 int z;
 
|main[Vec2& V]:
 V.x = 0;
 V.y = 0;
 V.z = 0;
 ret 0;

|main2[Vec2& V,Vec2& Other]:
 V.x = Other.x;
 V.y = Other.y;
 V.z = Other.z;
 ret 0;


/*
static Vec2 V3 = [];
Vec2 V2 = [];
|main2[]:
 V2.X = 0;
 V3.X = 0;

*/