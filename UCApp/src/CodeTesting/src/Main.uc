

$Unique_ptr<T>:
 T& Ptr;

 |new[this&] -> void;

 |Make[] -> this:
  var P =new T();
  this R = [];
  R.Ptr =: P;

  ret R;


|main[] -> int:
 var Object = unq int(); 
 ret 0;