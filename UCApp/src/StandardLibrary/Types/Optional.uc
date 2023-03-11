%ULang:
 
 
 $Optional<T>:
  bool _HasValue;
  T _Value;
  

  |new[this&] => SetValue();
  
  |new[this&,umut T& Value] => SetValue(Value);


  |SetValue[umut this&,umut T& Value]:
   _HasValue = true;
   _Value =Value;

  |SetValue[umut this&,null_t Value]:
   _HasValue = false;
  
  |HasValue[umut this&] => _HasValue;

  |Value[umut this&] => _Value;

  |Value[this&] => _Value;