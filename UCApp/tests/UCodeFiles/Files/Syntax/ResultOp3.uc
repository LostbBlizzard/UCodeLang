$Result<T,E> enum:
 Error[E],
 Value[T],

$Error;

|func[] -> int!Error:
 int!Error v = Value(0);
 ret ?v + 5;
 

|main[] -> int!Error:
 match func():
  Value(out V):ret V;
  Error(out V):ret 0;