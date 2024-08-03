|IsFuncionPointer<T>[] -> bool:
  ret false;
|main[]:
 $FuncOther = |[] -> int;
 ret IsFuncionPointer<int>() == false && IsFuncionPointer<FuncOther>() == true;
