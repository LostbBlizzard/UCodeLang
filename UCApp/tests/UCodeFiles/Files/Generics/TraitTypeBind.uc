$Trait trait:
 |Func[this&] -> int;

$Type1[Trait]:
 |Func[this&] -> int: ret 5;

$Type2[Trait]:
 |Func[this&] -> int: ret 3;

|Func<[Trait]T>[imut T& val]:
 ret val.Func();

|main[]:
 Type1 val1 = [];
 Type2 val2 = [];

 ret Func(val1) == 5 && Func(val2) == 3;
