$Unique_Basic<T> export:
 private:
  T& _ptr;
  |NullCheck[this&]:
   uintptr ptr = unsafe bitcast<uintptr>(_ptr);
   if ptr == uintptr(0):
    panic("Unique Pointer Is Null");
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

  
  export |Get[this&] -> T&: 
   NullCheck();
   ret _ptr;
  export |Get[imut this&] -> imut T&:
   NullCheck();
   ret _ptr;
   
  export |~>[this&] -> T&:ret Get();
  export |~>[imut this&] -> imut T&:ret Get();
  
  unsafe |Release[this&] -> T&:
   NullCheck();
   var& val = _ptr;

   unsafe _ptr =: unsafe bitcast<T&>(0);
   ret val;

$Unique_Trait<T> export:
 private:
  T _base;
  $BaseAs:
   uintptr pointer;
   uintptr vtable;
  |GetBaseAs[this&] -> BaseAs&:
   T& ptr = _base;
   uintptr asnum = unsafe bitcast<uintptr>(ptr);
   BaseAs& r = unsafe bitcast<BaseAs&>(asnum);
   ret r;
  |NullCheck[this&]:
   var& base = GetBaseAs();
   if base.pointer != 0:
    panic("Unique Pointer Is Null");
 public:
  export |new[this&] -> void:
   var& base = GetBaseAs();
   base.pointer = 0;
   base.vtable = 0;
  
  export |new[this&,imut this& Other] = invalid;

  export |new[this&,moved this& Value] -> void:
   var& base = GetBaseAs();
   var& otherbase = Value.GetBaseAs();
   base = otherbase;
   
   otherbase.pointer = 0;
   otherbase.vtable = 0;
  
  export |new<X>[this&,moved Unique_Basic<X> Value] -> void:
     _base = unsafe Value.Release();
  
  export |drop[this&]:
   var& base = GetBaseAs();
   if base.pointer != 0:
    unsafe drop(_base);

  //export |Make<Arg...>[Arg pars] -> this:
  // this r = [];
  // unsafe r._ptr =: unsafe new T(pars);
  // ret r;

  
  export |Get[this&] -> T: 
   NullCheck();
   ret _base;
  export |Get[imut this&] -> imut T:
   NullCheck();
   ret _base;
   
  export |~>[this&] -> T:ret Get();
  export |~>[imut this&] -> imut T:ret Get();

  unsafe |Release[this&] -> T:
   NullCheck();
   var val = _base;

   var& base = GetBaseAs();
   base.pointer = 0;
   base.vtable = 0;
   ret val;

$Unique_ptr<T> = match type(T).IsDynmaicTrait():
  true => type(Unique_Trait<T>);
  false => type(Unique_Basic<T>);
;
