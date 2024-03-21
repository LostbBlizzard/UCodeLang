
$Array<T,[uintptr](Count)>:
 T[/Count] base;

  
 |new<[T]P...>[this&,P pars] -> void; 

 |other<[T]P...>[this&,P pars] => pars.Count();
 |Size[this&] => Count;
  

|main[]:
 int16[3] g = [1,2,3];//number node is an int32 by default with out type inference

 ret g.Size() == g.other(1,2,3);



