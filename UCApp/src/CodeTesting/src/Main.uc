
$Cool<T>;
$Other<T>;


|Func[int V] -> void;
|Func<T>[Cool<T> V] -> int;
|Func<T>[Other<T> V] -> bool;




|main[]:
 Cool<int> V = [];
 Other<char> V1 = [];
 int V2 = [];
 
 Func(V);
 Func(V1);
 Func(V2);
