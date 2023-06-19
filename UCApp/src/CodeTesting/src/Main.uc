


/*

$Vec2:
 int x = 0;
 int y = 0;

|main[]:
 Vec2 V = [];
 ret V.X;

static Vec2 V3 = [];
Vec2 V2 = [];
|main2[]:
 V2.X = 0;
 V3.X = 0;

*/

$SomeTag<T> tag;

[SomeTag<int>] 
|Hello[] -> void;

[SomeTag<int>(0)] 
|Hello2[] -> void;