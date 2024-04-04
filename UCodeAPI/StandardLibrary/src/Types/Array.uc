

$Array<T,[uintptr](ArraySize)>[Buffer_t<T>] export:
 private:
  T[/ArraySize] _data;
 public:
  |new[this&] -> void;

  |new<[T]Agrs...>[this&,Agrs pars]: 
   $if pars.Count() != ArraySize:
    invalid("Args Count is not the same as ArraySize");

   $for [i,par : pars]:
    unsafe _data[i] = par;

  unsafe |Data[imut this&] -> imut T[&]:ret unsafe _data[0];
  unsafe |Data[this&] -> T[&]:ret unsafe _data[0];

  |Size[imut this&] => ArraySize;

  |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= ArraySize:panic("Index is out of bounds");

    ret unsafe _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= ArraySize:panic("Index is out of bounds");

    ret unsafe _data[Index];