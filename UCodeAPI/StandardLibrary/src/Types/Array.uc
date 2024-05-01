

$Array<T,[uintptr](ArraySize)>[Buffer_t<T>] export:
 private:
  T[/ArraySize] _data;
 public:
  export |new[this&] -> void;

  export |new<[T]Agrs...>[this&,Agrs pars]: 
   $if pars.Count() != ArraySize:
    invalid("Args Count is not the same as ArraySize");

   $for [i,par : pars]:
    unsafe _data[i] = par;

  export unsafe |Data[imut this&] -> imut T[&]:ret unsafe _data[0];
  export unsafe |Data[this&] -> T[&]:ret unsafe _data[0];

  export |Size[imut this&] => ArraySize;

  export |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= ArraySize:panic("Index is out of bounds");

    ret unsafe _data[Index];
  export |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= ArraySize:panic("Index is out of bounds");

    ret unsafe _data[Index];