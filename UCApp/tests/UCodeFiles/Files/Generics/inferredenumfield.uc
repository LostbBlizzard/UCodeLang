

$Optional<T> enum:
 Option[T Value],
 Null,

|main[] -> int:
 int? Val = Option(5);

 match Val:
  Option(out Valu):ret Valu;
  Null:ret 0;