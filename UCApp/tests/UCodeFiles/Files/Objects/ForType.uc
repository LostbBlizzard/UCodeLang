
$SomeType;


$for SomeType:
 |func[this&,int num] => num + 6;
 |staticfunc[int num] => num + 8;


|main[]:
 SomeType v = [];

 ret v.func(5) == 11  && SomeType::staticfunc(4) == 12;