ULang::Native:
 $PathChar = char;

 |OS_PrintChar[char Value] -> void: __LogChar(Value);
 |OS_PrintSpan[imut char[&] Buffer,uintptr Size] -> void:__LogBuffer(Buffer,Size);
 |OS_PrintClear[] -> void;
 |OS_Malloc[uintptr Size] => __Malloc(Size);
 |OS_Free[byte[&] Block] => __Free(Block);

