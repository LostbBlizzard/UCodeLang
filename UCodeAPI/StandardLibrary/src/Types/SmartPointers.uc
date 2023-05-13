%ULang:
  //has built in alias in compiler.Ex: int^ and unq int();
  $Unique_ptr<T>:
   private:
    T& _Data;
   public:

    |new[this&]:
     _Data &= bitcast<T[&]>(0);

    |new[this&, moved this& Item];

    

  //has built in alias in compiler.Ex: int[\]^ and unq int[\](); 
  $Unique_Array<T>;

  //has built in alias in compiler.Ex: shr int();
  $Shared_ptr<T>;


  $Shared_Array<T>;