
int someval = 0;

|func1[]: 
 someval++;
 ret true;

|func2[]: 
 someval++;
 ret true;

|main[] -> int:

 if func1() && func2():
  int unused = 0;

 ret someval;
