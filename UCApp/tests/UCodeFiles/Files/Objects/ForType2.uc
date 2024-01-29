
$Optional<T> enum:
 Value[T val],
 None,

$for<T> Optional<T>:
  |HasValue[this&] -> bool:
   ret true;

|main[]:
 int? v = None;

 ret v.HasValue();