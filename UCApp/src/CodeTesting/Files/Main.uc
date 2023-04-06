


|main[]:

 var Hello = |[x,y] => x * y;
 var Hello2 = |[x,y]:
  var Tep = x * y;
 ;
                       
 var Hello3 = |[x,y]() => x * y;
 var Hello4 = |[int x,int y]():
  var Tep = x * y;
 ;

 var Hello5 = |[int x,int y]();;
 
 var Hello6 = [x,y] => x * y;
 var Hello7 = [x,y,z]:
  var Tep = x * y * z;
 ;

 

 ret 0;