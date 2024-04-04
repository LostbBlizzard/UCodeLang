
$Buffer_t<T> trait export:
 |[][this&,uintptr Index] -> T&;
 |[][imut this&,uintptr Index] -> T&;
 |Size[imut this&] -> uintptr;
 unsafe |Data[imut this&] -> imut T[&];
 unsafe |Data[this&] -> T[&];

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


  export |[][this&,Range_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  export |[][imut this&,Range_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];

  export |AsSpan[this&] -> T[:]:ret unsafe [Data(),Size()];
  export |AsSpan[imut this&] -> imut T[:]:ret unsafe [Data(),Size()];

