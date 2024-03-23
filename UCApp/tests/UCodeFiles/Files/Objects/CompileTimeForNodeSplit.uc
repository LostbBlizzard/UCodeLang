$Array<T,[uintptr](Count)>:
 T[/Count] base;
 |new[this&] ->void;
 
 |new<[T]P...>[this&,P pars] -> void:

  $for [i,item : pars]:
    unsafe base[i] = item;
 


|main[]:
 int[3] r = [1,2,3];

 ret r;
