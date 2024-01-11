
int v = [];

|func[int a,int b]: 
 v = v * (a + b);

|main[]:
 block:
  int g = 10;
  defer func(g,6);
 
  g = 0;
  v = 4;

 ret v;//64