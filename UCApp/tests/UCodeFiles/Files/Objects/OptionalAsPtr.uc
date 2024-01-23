
$Optional<T> enum:
 Opt[T Val],
 NoOpt,

|NullOp[]:
 int&? V = NoOpt;

 ret sizeof(int&?) == sizeof(uintptr);


|NullOp2[] -> int:
 int p = 10;
 int&? V = Opt(p);


 
 match V:
  Opt(out s):

    p += 5;
    ret s + 5;
  
  NoOpt:
    ret 0;
 