

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



$Item enum:
 Red[int V],
 Green,
 Blue,

|main[]:

 var V = Item::Red(10);

 match V:
  Item::Red(out Num):int a =Num;
  Item::Green: int a =2;
  Item::Blue: int a =3;
 
 