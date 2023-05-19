


|main[]:
 int a = 10;
 match a:
  0:a = 0;
  1:a = 10;
  2:a = 20;
  invalid:a = 0;
   

 int V = match a:
  0 => 1;
  1 => 2;
  invalid => 0;
