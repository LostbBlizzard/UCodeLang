$Array<T,[uintptr](Count)>:
 T[/Count] base;

  
 |new<T...>[this&] -> void;//parameter is not pack
 |Size[this&] => Count;
  

|main[]:
 int[3] g = [1,2,3];//should not compile becuase pack is not used as parameter

 ret g.Size();



