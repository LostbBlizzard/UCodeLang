
$Unique_ptr<T> export:
 private:
  T& _ptr;
 public:
  export |new[this&] -> void:
   unsafe _ptr =: unsafe bitcast<T&>(0);
  
  export |new[this&,imut this& Other] = invalid;

  export |new[this&,moved this& Value] -> void:
   unsafe _ptr =: Value._ptr;
   unsafe Value._ptr =: unsafe bitcast<T&>(0);
  
  export |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
    unsafe drop(_ptr);

  export |Make[] -> this:
   this r = [];
   unsafe r._ptr =: unsafe new T();
   ret r;


  export |Get[this&] -> T&:ret _ptr;
  export |Get[imut this&] -> imut T&:ret _ptr;
  
  export |Write[this&,imut T& newvalue] -> void:Get() = newvalue;
  export |Write[this&,moved T newvalue] -> void:Get() = newvalue;

  export |ReadPtr[imut this&] -> imut T&:ret Get();

  export |~>[this&] -> T&:ret Get();

  export |~>[imut this&] -> imut T&:ret Get();
