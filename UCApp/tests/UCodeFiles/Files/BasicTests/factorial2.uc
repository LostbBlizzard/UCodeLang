
|main[] => factorial(10);


|factorial[int n] -> int: 
  if n == 0:
    ret 1;
  else:
    ret factorial(n-1) * n;