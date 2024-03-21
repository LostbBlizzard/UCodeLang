
$Array<T,[uintptr](Count)>:
 T[/Count] base;

  
 |new<[T]P...>[this&,P pars] -> void;
 |Size[this&] => Count;
  

|main[]:
 int[3] g = [1,'2',3];//type_pack wants all T in this case sint32

 ret g.Size();