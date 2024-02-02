
$Result<T,E> enum:
 Value[T val],
 Error[E err],

$ErrorV;
|main[] -> int:
 int!ErrorV v = Value(0);
 match v:
  Value(out V):ret 1;
  Error(out V):ret 0;
