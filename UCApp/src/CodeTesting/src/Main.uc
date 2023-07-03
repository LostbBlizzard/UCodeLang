

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

eval |Func[int a] => a;

|main[]:
 ret Func(10);


|OutFunc[int V,out int Item] -> bool:
 Item = 10;
 ret true;