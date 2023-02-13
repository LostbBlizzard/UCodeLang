
|main_if[] -> int:
 if 1 == 1:
  ret 10;
 else:
  ret 0;

|main_while[] -> int:
 
 int A = 0;
 while A < 10:
  A++;

 ret A;


|main_do[] -> int:
 
 int A = 0;

 do:
  A++;
 while A < 10;

 ret A;