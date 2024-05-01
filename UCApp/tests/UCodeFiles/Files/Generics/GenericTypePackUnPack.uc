
|other[int a,int b] => a + b;
|other[int a,int b,int c] => a + b + c;

|func<T...>[T pars] => other(pars);

|main[] => func(5,10);
|main2[] => func(5,10,15);