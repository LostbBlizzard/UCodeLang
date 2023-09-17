$Optional<T> enum:
 Value[T val],
 None,


|func[] -> int?:
 int? v = Value(0);
 ret ?v + 5;
 

|main[] -> int!Error:
 match func():
  Value(out V):ret V;
  None:ret 0;