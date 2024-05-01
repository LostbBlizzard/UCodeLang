
$Optional<T> enum export:
 Opt[T Value],
 None,

$for<T> Optional<T> export:
 export |IsOpt[this&] -> bool:ret Opt(this,out tep);
 
 export |OptOr[this&,IPar<T> Else] -> T:
  if Opt(this,out r):
   ret r;

  ret Else;

 export |Unwrap[this&] -> T:
  if Opt(this,out r):
   ret r;

  panic("Bad Unwrap");
