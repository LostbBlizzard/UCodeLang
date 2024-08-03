
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

  export |Make<Arg...>[Arg pars] -> this:
   this r = [];
   unsafe r._ptr =: unsafe new T(pars);
   ret r;

  |NullCheck[this&]:
   uintptr ptr = unsafe bitcast<uintptr>(_ptr);
   if ptr == uintptr(0):
    panic("Unique Pointer Is Null");
  
  export |Get[this&] -> T&: 
   NullCheck();
   ret _ptr;
  export |Get[imut this&] -> imut T&:
   NullCheck();
   ret _ptr;
   
  export |~>[this&] -> T&:ret Get();
  export |~>[imut this&] -> imut T&:ret Get();
