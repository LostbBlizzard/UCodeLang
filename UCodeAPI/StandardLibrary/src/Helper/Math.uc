
$Math:
 export eval |PI<T>[] => 3.14;
 
 export |Max<T>[T A,T B]:
  if A > B:ret A;
  ret B;

 export |Min<T>[T A,T B]:
  if A < B:ret A;
  ret B;
 
 export |Pow<T>[T val,T power] => val;//TODO

 export |Lerp<T>[T start,T end,T time]:
  ret start + (end - start) * time;