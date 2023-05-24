%ULang:
 //has built in alias in compiler.Ex: int$ and shr int();
 $Shared_ptr<T>:
  private:
   $Data_t:
    T _Data;
    size_t RefCount = 0;
    |new<T...>[this&,T Pars]:
     _Data = T(Pars);
  
   Data& _DataPtr = LowLevel::NullPtr<Data&>();
   
   |AddRef[]:
    if !LowLevel::IsNullPtr(_Data):
     _Data.RefCount++;
   
   |RemoveRef[]:
    if !LowLevel::IsNullPtr(_Data):
     _DataPtr.RefCount--;
     if _DataPtr.RefCount == 0:
      drop(_Data);
      _Data &= LowLevel::NullPtr<Data&>();

  public:
   |new[this&] -> void;

   |new[this&,umut this& Other]:
    _DataPtr &= Other._DataPtr;
    AddRef();

   |drop[this&]:
    RemoveRef();
  
   |Value[this&] -> T&: 

     if LowLevel::IsNullPtr(_Data):Debug::NullPtrAccess();

     ret _DataPtr._Data;

   |Value[umut this&] -> umut T&:
     
     if LowLevel::IsNullPtr(_Data):Debug::NullPtrAccess();
     
     ret _DataPtr._Data;

   |~>[umut this&] -> umut T&:ret Value();
   |~>[this&] -> umut T&:ret Value();

   |Make<T...>[T Pars] -> this:
    var R = this();
    R._Data &= new Data_t(Pars);
    R.AddRef();
    ret R;