|IsFuncionPointer<T>[] -> bool:
  ret type(T).GetFuncionPointerInfo(out typeinfo FuncTypeValue);

|main[]:
 $FuncOther = |[] -> int;
 ret IsFuncionPointer<int>() == false && IsFuncionPointer<FuncOther>() == true;
