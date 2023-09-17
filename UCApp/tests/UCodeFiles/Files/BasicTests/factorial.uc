

|main[] => factorial(10);


|factorial[int n] -> int: 
  if n == 0:
    ret 1;
  else:
    ret n * factorial(n-1);
