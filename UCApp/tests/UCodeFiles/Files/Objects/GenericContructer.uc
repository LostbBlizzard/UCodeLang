bool wascalled = false;

$Test:
  |new<X>[this&,moved X Value] -> void:
   wascalled = true;

$Test2;

|main[]:
 Test b = Test2();
 ret wascalled;
