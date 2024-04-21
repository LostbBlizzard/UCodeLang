
$Optional<T> enum export:
 Opt[T Value],
 None,

$for<T> Optional<T>:
 |IsOpt[this&] -> bool:ret Opt(this,out tep);
 
 |OptOr[this&,IPar<T> Else] -> T:
  if Opt(this,out r):
   ret r;

  ret Else;

 |Unwrap[this&] -> T:
  if Opt(this,out r):
   ret r;

  panic("Bad Unwrap");
