$Optional<T> enum:
 Value[T val],
 None,

$Array<T,[uintptr](Count)>:
 T[/Count] base;

 |new<[T]P...>[this&,P pars] -> void:

  $for [i,item : pars]:
   unsafe base[i] = item; 

 $LoopObject:
   $MyType = Array<T,(Count)>;
   uintptr index = 0;
   MyType& _This;

   |new[this&,MyType& object]:
    unsafe _This =: object;

   |Next[this&] -> T&?:
    T&? r = None;

    if index < Count:
     r = Value(unsafe _This.base[index]);

    index++;    
    ret r;


 |for[this&] => LoopObject(this);


|main[]:
 int r = 0;
 int[3] v = [1,2,3];

 for [item : v]:
  r += item;

 ret r;


