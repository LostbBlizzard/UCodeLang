

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

$Vec2:
 int X;
 int Y;

static Vec2 OnS = [];

|main[]:
 
 OnS.X = 12;
 OnS.Y = 13;

 ret OnS;