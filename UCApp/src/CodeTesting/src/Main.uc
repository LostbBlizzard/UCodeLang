

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
 int X = 0;
 int Y = 0;

static Vec2 OnS = [];

|main[]:
 
 OnS.X++;
 OnS.Y++;

 ret OnS;