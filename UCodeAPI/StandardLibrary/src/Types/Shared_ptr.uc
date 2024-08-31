$Shared_Basic<T> export:
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
    panic("Unique Pointer Is Null");
 
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



//This could be better optimized with away to get destructer and vtable without storeing it in the Wapper struct.
//I need to add new compiler::funcion for it.
$Shared_Trait<T> export:
 private:
  $DesturcterPtr = |[uintptr object] -> void;
  $BaseAs:
   uintptr pointer;
   uintptr vtable;
  $Wapper:
    uintptr refcount = 0;
    uintptr vtable = 0;
    DesturcterPtr destructor;
    byte padding = 0;

    |GetObject[this&] -> uintptr:
     uintptr ptr =unsafe bitcast<uintptr>(padding);
     ret ptr;

  $Wapper2<X>:
    uintptr refcount = 0;
    uintptr vtable = 0;
    DesturcterPtr destructor;
    X padding;
    |new[this&,moved X value]:
     padding = value;


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
      var p = _ptr.destructor;
      p(_ptr.GetObject());

      unsafe drop(_ptr);
      unsafe _ptr =: unsafe bitcast<Wapper&>(0);

  |NullCheck[this&]:
   uintptr ptr = unsafe bitcast<uintptr>(_ptr);
   if ptr == uintptr(0):
    panic("Unique Pointer Is Null");
 
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
  
  export |new<X>[this&,moved Unique_Basic<X> Value] -> void:
     var& ptr = unsafe Value.Release();
     var& newptr = unsafe new Wapper2<X>(move ptr);

     var& releaseasptr = ptr;
     uintptr releaseasint = unsafe bitcast<uintptr>(releaseasptr); 
     BaseAs& asbase = unsafe bitcast<BaseAs&>(releaseasint);

     newptr.destructor = |[uintptr object]:
         X& asptr = unsafe bitcast<X&>(object); 

         //TODO remove memory allocation.
         //this may cause problems if type 'X' cant fit on the stack.
         X& copy = unsafe new X();

         unsafe:
            byte[&] asptrbit = bitcast<byte[&]>(bitcast<uintptr>(asptr));
            byte[&] ascopybit = bitcast<byte[&]>(bitcast<uintptr>(copy));
            LowLevel::Memmove(ascopybit,asptrbit,sizeof(X));

         unsafe drop(copy);
     ;
     newptr.vtable = asbase.vtable;
     unsafe drop(ptr);

     uintptr newptrasint = unsafe bitcast<uintptr>(newptr); 
     Wapper& aswapper = unsafe bitcast<Wapper&>(newptrasint);

     unsafe _ptr =: aswapper;
     AddOneRef();
  
  export |drop[this&]:
   RemoveOneRef();

  //export |Make<Arg...>[Arg pars] -> this:
  // this r = [];
  // unsafe r._ptr =: unsafe new Wapper(pars);
  // r.AddOneRef();
  // ret r;

  |GetDyn[this&] -> T:
   NullCheck();
   BaseAs val = [];
   val.pointer = _ptr.GetObject(); 
   val.vtable = _ptr.vtable;

   BaseAs& asptr = val;
   uintptr asint = unsafe bitcast<uintptr>(asptr);
   ret unsafe bitcast<T&>(asint);

  export |Get[this&] -> T: ret GetDyn();
  export |Get[imut this&] -> imut T: ret GetDyn();
   
  export |~>[this&] -> T:ret Get();
  export |~>[imut this&] -> imut T:ret Get();
  
  export |RefCount[this&] -> uintptr:
    NullCheck();
    ret _ptr.refcount;


$Shared_ptr<T> = match type(T).IsDynmaicTrait():
  true => type(Shared_Trait<T>);
  false => type(Shared_Basic<T>);
;
