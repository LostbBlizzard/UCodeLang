uintptr valuesize = 0;

$Test:
  |new<X>[this&,moved Test2<X> Value] -> void:
   valuesize = sizeof(X);

$Test2<T>;

|main[]:
 Test b = Test2<int>();
 ret valuesize == sizeof(int);
