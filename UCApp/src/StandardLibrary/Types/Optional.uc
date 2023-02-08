%ULang:
 $Nothing;
 
 $Optional<T>:
  bool _HasValue;
  T _Value;
  
  |new[this&,Nothing Value]:SetValue(Value);
  |new[this&]:SetValue(Nothing());
  |new[this&,umut T& Value]:SetValue(Value);


  |SetValue[umut this&,umut T& Value]:
   _HasValue = true;
   _Value =Value;

  |SetValue[umut this&,Nothing Value]:
   _HasValue = false;
  
  |HasValue[umut this&] => _HasValue;
  |GetValue[umut this&] => _Value;