
$Optional<T> enum:
 Value[T],
 None,


|func[] -> int?:
 int? v = Value(0);
 ret Value(?v + 5);
 

|main[] -> int:
 match func():
  Value(out V):ret V;
  None:ret 0;