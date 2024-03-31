
$Buffer_t<T> trait export:
 |[][this&,uintptr Index] -> T&;
 |[][imut this&,uintptr Index] -> T&;
 |Size[imut this&] -> uintptr;


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


