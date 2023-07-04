

|main[]:
 
 if OutFunc(5,out NewValue):
  ret NewValue;

 ret 0;


|OutFunc[int V,out int Item] -> bool:
 Item = 10;
 ret true;