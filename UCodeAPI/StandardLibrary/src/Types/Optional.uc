%ULang:
 
 trait EmptyVariant:
   
   |Opt_IsEmpty[] -> bool;

   |Opt_SetAsEmpty[] -> void;

   |Opt_SetValue[umut T& Value] -> void;

   |Opt_SetValue[moved T Value] -> void;


 //has built in alias in compiler.Ex: int?
 $Optional<T>:
  private:
   T _Value;

   enum OptType:
    TwoMembers,
    NullPtr,
    EmptyVariant,

   $if type(T == T&):
    eval OptValue = OptType::NullPtr;
   $else if type(T ~= EmptyVariant):
    eval OptValue = OptType::EmptyVariant;
   $else:
    eval OptValue = OptType::TwoMembers;

   $if OptValue == OptType::TwoMembers:
    bool _HasValue;
    
    |SetValue[this&,umut T& Value]:
     _HasValue = true;
     _Value = Value;

    |SetValue[this&,moved T Value]:
     _HasValue = true;
     _Value = Value;

    |SetEmpty[this&]:
     _HasValue = false;

    |HasValue[umut this&] => _HasValue;
   
   $else if OptValue == OptType::NullPtr:
    
    eval uintptr Nullptr = 0;

    |SetValue[this&,umut T& Value]:
     _Value &= Value;

    |SetValue[this&,moved T Value]:
     _Value &= Value;

    |SetEmpty[this&]:
     bitcast<uintptr&>(_Value) &= bitcast<uintptr&>(Nullptr);

    |HasValue[umut this&] => bitcast<uintptr>(_Value) == bitcast<uintptr>(Nullptr);

   $else:
    |Invaild[]:invaild("OptType was not added");

  public:
   |new[this&] => SetEmpty();
   |new[this&,umut T& Value] => SetValue(Value);
   |new[this&,moved T Value] => SetValue(Value);

   |IsEmpty[] => HasValue();

   |Value[umut this&] => _Value;
   |Value[this&] => _Value;