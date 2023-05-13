%ULang:

 //has built in alias in compiler.Ex: int[10]
 $Array<T,(Size)>[]:
  private:
   T[\Size] Data;

  public:
  
  |[][this&,size_t Index] => Data[Index]; 
  |Size[this&] => Size;
