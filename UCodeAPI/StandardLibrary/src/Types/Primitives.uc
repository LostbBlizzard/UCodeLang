%ULang:
  $size_t = uintptr;

  $ascllchar = char;
  $char8 = char;
  $char16 = uint16;
  $char32 = uint32;

  $void_ptr = !uintptr;

  |ToString<T>[] -> String;//for ints 
  |ToStringf<T>[] -> String;//for floats 