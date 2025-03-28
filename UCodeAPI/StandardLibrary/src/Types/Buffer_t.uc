
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

  export |[][this&,RangeFull_t Range] -> T[:]:ret AsSpan(); 
  export |[][imut this&,RangeFull_t Range] -> imut T[:]:ret AsSpan();
  
  export |[][this&,RangeTo_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  export |[][imut this&,RangeTo_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];
  
  export |[][this&,RangeFrom_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  export |[][imut this&,RangeFrom_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];
 
  export |AsSpan[this&] -> T[:]:ret unsafe [Data(),Size()];
  export |AsSpan[imut this&] -> imut T[:]:ret unsafe [Data(),Size()];
  
  export |Reverse[this&]:
   for [uintptr i = 0;i < (Size() / 2);i++]:
    Swap(this[i],this[Size() - 1 - i]);

  export |Find<[functor [imut T&] -> bool]Func>[this&,Func func] -> T&?: 
   for [uintptr i = 0;i < Size();i++]:
    if func(this[i]):
     ret Opt(this[i]);
   ret None;

  export |Find<[functor [imut T&] -> bool]Func>[imut this&,Func func] ->  imut T&?: 
   for [uintptr i = 0;i < Size();i++]:
    if func(this[i]):
     ret Opt(this[i]);
   ret None;

  export |FindIndex<[functor [imut T&] -> bool]Func>[imut this&,Func func] -> uintptr?:
   for [uintptr i = 0;i < Size();i++]:
    if func(this[i]):
     ret Opt(i);
   ret None;

  //Based On https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
  export |Shuffle<[Random_t]Rand>[this&,Rand& Rng] -> void:
   var span = AsSpan();

   for [uintptr i = 0;i < span.Size();i++]:
     var newspan = span[0..span.Size() - i];

     uintptr indextomove = Rng.NextInt<uintptr>(0,newspan.Size());

     T tep = move newspan[indextomove];

     //Shift all newspan item left
     for [uintptr x = indextomove + 1; x < newspan.Size(); x++]:
       newspan[x - 1] = move newspan[x];

     //Shift all donespan items left
     for [uintptr x = 0; x < i; x++]:
        span[newspan.Size() + x - 1] = move span[newspan.Size() + x];

     span[span.Size() - 1] = move tep;
  
