
$Enable<[uintptr](StrSize)> tag:
 bool Enable;
 char[/StrSize] Error;
 |new[this&,bool v,char[/StrSize] err]:
  Enable = v;
  Error = err;


$Hello<T>:
  [Enable(type(bool == T),"Type is not Bool")]
  |Func[this&] => 1;

|main[]:
 Hello<int> val = [];
 ret val.Func() == 1;// Error Type is not Bool
