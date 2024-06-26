$Span<T> export:
 private:
  T[&] _data;
  uintptr _size;
 public:
  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;

  export unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;
  export |Size[imut this&] => _size;
  export unsafe |Data[this&] => _data;
  export unsafe |Data[imut this&] => _data;
  

  export |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];
  export |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");
    
    ret unsafe _data[Index];

  export |[][this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];
  export |[][imut this&,Range_t<uintptr> Range] -> this:ret unsafe [_data[Range.Start()],Range.End() - Range.Start()];

  $ThisType = this;
  $Iterator export:
    uintptr Index = 0;
    ThisType& _This; 
    export |new[this&,ThisType& value]:
     unsafe _This =: value;
   
    export |Next[this&] -> T&?:
     if Index < _This.Size():
      var old = Index;
      Index++;
      ret Opt(_This[old]);

     ret None;
  
    $Spit export:
     uintptr _Index;
     T& _Value;

    export |Next2[this&] -> Spit?:
     if Index < _This.Size():
      var old = Index;
      Index++;

      Spit r = [];
      r._Index = old;
      unsafe r._Value =: _This[old];
      ret Opt(r);

     ret None;
 
  $IIterator export:
    uintptr Index = 0;
    imut ThisType& _This; 
    export |new[this&,imut ThisType& value]:
     unsafe _This =: value;
   
    export |Next[this&] -> imut T&?:
     if Index < _This.Size():
      var old = Index;
      Index++;
      ret Opt(_This[old]);

     ret None;
  
    $Spit export:
     uintptr _Index;
     imut T& _Value;

    export |Next2[this&] -> Spit?:
     if Index < _This.Size():
      var old = Index;
      Index++;

      Spit r = [];
      r._Index = old;
      unsafe r._Value =: _This[old];
      ret Opt(r);

     ret None;


  export |for[this&] => Iterator(this);
  export |for[imut this&] => IIterator(this);

  export |Empty[imut this&] => Size() == 0;

  export |First[imut this&] -> imut T&?:
   if Size() == 0:
    ret None;
   ret Opt(this[0]);
  
  export |First[this&] -> T&?:
   if Size() == 0:
    ret None;
   ret Opt(this[0]);
  
  export |Last[imut this&] -> imut T&?:
   if Size() == 0:
    ret None;
   ret Opt(this[Size() - 1]);
  
  export |Last[this&] -> T&?:
   if Size() == 0:
    ret None;
   ret Opt(this[Size() - 1]);

  export |Find<[functor [imut T&] -> bool]Func>[this&,Func func] -> T&?: 
   for [uintptr i = 0;i < _size;i++]:
    if func(unsafe _data[i]):
     ret Opt(unsafe _data[i]);
   ret None;

  export |Find<[functor [imut T&] -> bool]Func>[this&,Func func] ->  imut T&?: 
   for [uintptr i = 0;i < _size;i++]:
    if func(unsafe _data[i]):
     ret Opt(unsafe _data[i]);
   ret None;

  export |FindIndex<[functor [imut T&] -> bool]Func>[imut this&,Func func] -> uintptr?:
   for [uintptr i = 0;i < _size;i++]:
    if func(unsafe _data[i]):
     ret Opt(i);
   ret None;
