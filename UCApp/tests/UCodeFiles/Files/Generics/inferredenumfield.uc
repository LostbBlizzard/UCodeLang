

$Optional<T> enum:
 Option[T Value],
 Null,

|main[] -> int:
 int? Val = Option(5);

 match Val:
  Option(out Value):ret Value;
  Null:ret 0;