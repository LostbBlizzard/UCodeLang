|main[]:

 bool V = true;
 int r =  match V:
  true => 0;
  //Error no false block
 ;
 ret r;