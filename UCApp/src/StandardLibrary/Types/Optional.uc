%ULang:
 $Nothing;
 
 $Optional<T>:
  bool _HasValue;
  T _Value;
  |new[this&,Nothing Value]:
   _HasValue = false;
  |new[this&]:
   _HasValue = false;
  

  |HasValue[this&] => _HasValue;
  |GetValue[this&] => _Value;