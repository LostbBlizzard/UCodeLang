%ULang:
 

 //has built in alias in compiler.Ex: int?
 $Optional<T>:
  
  enum OptType:
    TwoMembers,
    NullPtr,
    EmptyVariant,

  private:
   T _Value;
   bool _HasValue;

   |SetEmpty[this&]:_HasValue =false;

   |SetValue[this&,umut T& Value]:_Value = Value;
   
   |SetValue[this&,moved T& Value]:_Value = Value;

   |GetOptType => OptType::TwoMembers;
  public:
   |new[this&] => SetEmpty();
   |new[this&,umut T& Value] => SetValue(Value);
   |new[this&,moved T Value] => SetValue(Value);

   |IsEmpty[] => HasValue();

   |Value[umut this&] => _Value;
   |Value[this&] => _Value;