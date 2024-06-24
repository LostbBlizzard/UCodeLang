
$Optional<T> enum export:
 Opt[T Value],
 None,

$Array<T,[uintptr](ArraySize)>:
  T[/ArraySize] _data;

  |new<[T]Agrs...>[this&,Agrs pars]: 
   $if pars.Count() != ArraySize:
    invalid("Args Count is not the same as ArraySize");

   $for [i,par : pars]:
    unsafe _data[i] = par;

  |Find<[functor [int&] -> bool]X>[this&,X func] -> int&?:
   for [uintptr i = 0;i < ArraySize;i++]:
    if func(unsafe _data[i]):
     ret Opt(unsafe _data[i]);
   ret None;

|main[]:
 int[3] list = [1,2,3];

 int&? p = list.Find([x] => x == 2);

 if Opt(p,out val):
  ret val == 2;

 ret false;
