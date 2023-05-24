%ULang:
  
  //has built in alias in compiler.Ex: int^ and unq int();

  $Unique_ptr<T>:
   private:
    T& _Data = LowLevel::NullPtr<T&>();
   public:
    |new[this&] -> void;

    |new[this&,moved this& Other] -> void;
     _Data &= Other._Data;

     Other._Data &= LowLevel::NullPtr<T&>();

    //|new[this&,umut this&] -> invaid;


    |drop[this&] -> void:
     drop(_Data);
     _Data &= LowLevel::NullPtr<T&>();
     
    |Value[this&] -> T&:
     
     $if Compiler::IsDebugBuild:
       if LowLevel::IsNullPtr(_Data):Debug::NullPtrAccess();
     
     ret _Data;

    |Value[umut this&] -> umut T&:
     
     $if Compiler::IsDebugBuild:
       if LowLevel::IsNullPtr(_Data):Debug::NullPtrAccess();
     
     ret _Data;

    |~>[umut this&] -> umut T&:ret Value();
    |~>[this&] -> umut T&:ret Value();


    |Make<T...>[T Pars] -> this:
     var R = this();
     R._Data &= new T(Pars);

     ret R;
