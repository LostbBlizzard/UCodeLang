%ULang:
  
  //has built in alias in compiler.Ex: int[^] and unq int[?]; 
  
  $Unique_Array<T>[]:   
   private:
    T[&] _Data = LowLevel::NullPtr<T[&]>();
    
    $if Compiler::IsDebugBuild: 
     size_t _DebugSize = 0;

   public:
    |new[this&] -> void;

    |new[this&,moved this& Other] -> void;
     _Data &= Other._Data;
    
     $if Compiler::IsDebugBuild: 
      _DebugSize = Other._DebugSize;

     Other._Data &= bitcast<T[&]>(0);
     $if Compiler::IsDebugBuild: 
      Other._DebugSize = 0;
    
    
    |new[this&,umut this&] -> invaid;


    |drop[this&] -> void:
     drop(_Data);
     
     $if Compiler::IsDebugBuild: 
      _Data &= bitcast<T[&]>(0);
      _DebugSize = 0;



    |[][this&,Size_t Index] -> T&:
     $if Compiler::IsDebugBuild: 
       if Index >= _DebugSize:
        Debug::OutOfBounds(Index,_DebugSize);
     
     ret _Data;


    |Make<T...>[size_t Size] -> this:
     var R = this();
     R._Data &= new T[Par];
     
     $if Compiler::IsDebugBuild:
      R._DebugSize = Size;
     
     ret R;