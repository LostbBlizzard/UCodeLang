

/*

$Unique_ptr<T>:
 T& Ptr;

 |new[this&] -> void:
  Ptr =: bitcast<T&>(0);
 
 |~>[this&] -> T&:ret Ptr;
 |AsPtr[this&] -> T&:ret Ptr;

 |drop[this&]:
  uintptr PtrAsInt = bitcast<uintptr>(Ptr);
  if PtrAsInt != uintptr(0):
   drop(Ptr);
   Ptr =: bitcast<T&>(0);

 |Make[] -> this:
  this R = [];
  R.Ptr =: new T();

  ret R;


|main[] -> int:
 var Object = unq int(); 
 ret Object.AsPtr();

*/

|Func1[] ->int;
|Func2[] ->int;
|Func3[] ->int;


|ifcode[]:
 //int Value = 0;
 if true: Func1();
 else if false: Func2();
 else: Func3();

$Item enum:
 Red,
 Green,
 Blue,

|main[]:

 bool V = Item::Red == Item::Green;
 
 /*
 match Item::Red:
  Item::Red: Func1();
  Item::Green: Func2();
  Item::Blue: Func3();
 ;
 */