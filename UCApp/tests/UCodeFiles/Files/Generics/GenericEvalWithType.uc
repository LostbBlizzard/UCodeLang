
$Array<T,[uintptr](Count)>:
 T[/Count] base;
 |Size[this&] => Count;
 |new<T...>[this&,T pars] -> void;

|main[]:
 int[3] g = [1,2,3];// 3 would be an int and not uintptr if not the type hint in Array
 ret g.Size();
