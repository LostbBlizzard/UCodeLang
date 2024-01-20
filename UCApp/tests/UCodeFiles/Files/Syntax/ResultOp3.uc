$Result<T,E> enum:
 Error[E],
 Value[T],

$ErrorV;

|func[] -> int!ErrorV:
 int!ErrorV v = Value(0);
 ret Value(?v + 5);
 

|main[] -> int:
 match func():
  Value(out V):ret V;
  Error(out V):ret 0;