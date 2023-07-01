$Item enum:
 Red,
 Green,
 Blue,


|main[]:

 var V = Item::Red;
 int r =  match V:
  Item::Red => 0;
  Item::Green => 1;
  //Error no Blue block
 ;
 ret r;