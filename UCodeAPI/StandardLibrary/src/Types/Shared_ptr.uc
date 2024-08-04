$Shared_ptr<T> export:
 private:
  $Wapper:
    T base;
    uintptr refcount = 0;
    |new<P...>[this&,P pars]:
     base = [pars];

  Wapper& _ptr;

  |AddOneRef[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
     _ptr.refcount++;

  |RemoveOneRef[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
    _ptr.refcount--;
    if _ptr.refcount == 0:
      unsafe drop(_ptr);
      unsafe _ptr =: unsafe bitcast<Wapper&>(0);

  |NullCheck[this&]:
   uintptr ptr = unsafe bitcast<uintptr>(_ptr);
   if ptr == uintptr(0):
    //panic("Unique Pointer Is Null");
 
 public:
  export |new[this&] -> void:
   unsafe _ptr =: unsafe bitcast<Wapper&>(0);
  
  export |new[this&,imut this& Other]:
   uintptr ptr = unsafe bitcast<uintptr>(Other._ptr);
   if ptr != 0:
    unsafe _ptr =: Other._ptr;
    AddOneRef();
   else:
    unsafe _ptr =: unsafe bitcast<Wapper&>(0);

  export |new[this&,moved this& Value] -> void:
   unsafe _ptr =: Value._ptr;
   unsafe Value._ptr =: unsafe bitcast<Wapper&>(0);
  
  export |drop[this&]:
   RemoveOneRef();

  export |Make<Arg...>[Arg pars] -> this:
   this r = [];
   unsafe r._ptr =: unsafe new Wapper(pars);
   r.AddOneRef();
   ret r;

  export |Get[this&] -> T&:
    NullCheck();
    ret _ptr.base;

  export |Get[imut this&] -> imut T&:
    NullCheck();
    ret _ptr.base;
   
  export |~>[this&] -> T&:ret Get();
  export |~>[imut this&] -> imut T&:ret Get();
  
  export |RefCount[this&] -> uintptr:
    NullCheck();
    ret _ptr.refcount;
