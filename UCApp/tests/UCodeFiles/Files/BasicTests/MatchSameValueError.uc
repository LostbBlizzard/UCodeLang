$Item enum:
 Red,
 Green,
 Blue,


|main[]:

 bool V = true;
 int r =  match V:
  true => 0;
  true => 1;//the value 'true' is matched more then once
  false => 0;
 ;
 ret r;