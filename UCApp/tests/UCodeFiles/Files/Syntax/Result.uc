
$Result<T,E> enum:
 Value[T val],
 Error[E err],

$Error;
|main[] -> int:
 int!Error v = Value(0);
 match v:
  Value(out V):ret 1;
  Error(out V):ret 0;
