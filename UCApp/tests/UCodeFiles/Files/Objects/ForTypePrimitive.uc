$String;

|main[]:
 int a = 10.Pow(5);

 String str = [];
 int num = int::Parse(str);

 ret a == num;

$for int:
 |Pow[this&,int ex]:
  if ex == 0:
   ret 1;

  int base = this;
  int r = base;
  for [uintptr i = 1;i < ex -> uintptr;i++]:
   r = r * base;

  ret r;
 
 |Parse[imut String& val] -> int:ret 100000;

