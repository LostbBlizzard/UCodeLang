
$Trait trait:
 |Func[this&] -> int;

$Type1[Trait]:
 |Func[this&] -> int: ret 5;

$Type2[Trait]:
 |Func[this&] -> int: ret 3;

|Func<[Trait]T>[imut T& val]:
 ret val.Func();

$WithOutTrait;
|main[]:
 WithOutTrait val1 = [];

 ret Func(val1) == 5;
