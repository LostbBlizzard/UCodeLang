
$Unique_ptr<T>:
 private:
  T& _ptr;
 public:
  |new[this&] -> void:
   _ptr =: unsafe bitcast<T&>(0);
  
  |new[this&,imut this& Other] = invalid;

  |new[this&,moved this& Value] -> void:
   _ptr =: Value._ptr;
   Value._ptr =: unsafe bitcast<T&>(0);
  
  |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
    unsafe drop(_ptr);

  |Make[] -> this:
   this r = [];
   r._ptr =: unsafe new T();
   ret r;


  |Get[this&] -> T&:ret _ptr;
  |Get[imut this&] -> imut T&:ret _ptr;
  
  |Write[this&,imut T& newvalue] -> void:Get() = newvalue;
  |Write[this&,moved T newvalue] -> void:Get() = newvalue;

  |ReadPtr[imut this&] -> imut T&:ret Get();

  |~>[this&] -> T&:ret Get();

  |~>[imut this&] -> imut T&:ret Get();
