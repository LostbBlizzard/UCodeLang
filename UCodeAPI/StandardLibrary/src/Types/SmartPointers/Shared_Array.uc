%ULang:
  //has built in alias in compiler.Ex: int[$] and shr int[?];
  $Shared_Array<T>:
   private:
    $Data_t:
     T[&] _Data = LowLevel::NullPtr<T[&]>(); 
     size_t RefCount = 0;
     $if Compiler::IsDebugBuild: 
      size_t _DebugSize = 0;

     |new[this&,size_t Size]:
      _Data &= new T[Size];
      _DebugSize = Size;

     |drop[]:
      drop(_Data);
    
    |AddRef[]:
     if !LowLevel::IsNullPtr(_Data):
      _Data.RefCount++;
   
    |RemoveRef[]:
     if !LowLevel::IsNullPtr(_Data):
      _DataPtr.RefCount--;
      if _DataPtr.RefCount == 0:
       drop(_Data);
       _Data &= LowLevel::NullPtr<Data&>();
   
   Data_t& _DataPtr=  LowLevel::NullPtr<Data_t&>();
   public:
    |new[this&] -> void;

    |new[this&,umut this& Other]:
     _DataPtr &= Other._DataPtr;
     AddRef(); 

    |drop[this&]:
     RemoveRef();

    |[][this&,Size_t Index] -> T&:

     if LowLevel::IsNullPtr(_Data):Debug::NullPtrAccess();

     $if Compiler::IsDebugBuild: 
       if Index >= _DataPtr._DebugSize:
        Debug::OutOfBounds(Index,_DataPtr._DebugSize);
     
     ret _Data;


    |Make<T...>[size_t Size] -> this:
     var R = this();
     R._DataPtr &= new Data_t(Size);
     ret R;