


|main[]:
 int a = 10;
 match a:
  1:a = 10;
  2:a = 20;
  3:a = 30;
  invalid:a = 0;
   
 
 int V = match a:
  50 => 500;
  70 => 700;
  invalid => 100;
 ;