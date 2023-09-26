
$Math:
 eval |PI<T>[] => 3.14;
 
 |Max<T>[T A,T B]:
  if A > B:ret A;
  ret B;

 |Min<T>[T A,T B]:
  if A < B:ret A;
  ret B;
 
 |Pow<T>[T val,T power] => val;//TODO