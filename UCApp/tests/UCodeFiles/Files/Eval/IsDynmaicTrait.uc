|IsDynmaicTrait<T>[] -> bool:
  ret type(T).IsDynmaicTrait();

|main[]:
 $Some trait:
  dynamic |Funcion[] -> int;

 $Test = dynamic<Some>;
 ret IsDynmaicTrait<int>() == false && IsDynmaicTrait<Test>() == true;
