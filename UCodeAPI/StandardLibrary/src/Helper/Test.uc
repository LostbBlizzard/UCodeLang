%ULang:
 
 $tag Test:
  $enum Success:
   NoPanic,
   MustPanic,

  Success Value = Success::NoPanic;

  |new[this&] -> void;
  |new[this&,Success Mode]:
   Value = Mode; 