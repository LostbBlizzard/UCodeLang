
$Vec2:
 uintptr X;
 uintptr Y;
 |new[this&];
 |drop[this&];
 |DoSome[this&,int a];

|Main2[Vec2 Object] -> Vec2;
 

|main[bool V]: 

 Vec2 Value = [];
 Vec2::DoSome(Value,5);
 
 Value.DoSome(5);
