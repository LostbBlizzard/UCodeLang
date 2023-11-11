
$Cool<T>;
$Other<T>;


|Func[int V] => 1;
|Func<T>[Cool<T> V] => 3; 
|Func<T>[Other<T> V] => 2;



|main[]:
 Cool<int> V = [];
 Other<char> V1 = [];
 int V2 = [];
 
 var r = Func(V);
 var r1 = Func(V1);
 var r2 = Func(V2);
 ret r == 3 && r1 == 2 && r2 == 1; 