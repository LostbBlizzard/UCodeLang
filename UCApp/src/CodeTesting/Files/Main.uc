
$Vec2:
 uintptr X;
 uintptr Y;
 |new[this&];
 |drop[this&];
 |DoSome[this&,int a];
 
|main[bool V]: 

 Vec2 Value = [];
 Vec2::DoSome(Value,5);
 
 Value.DoSome(5);