ULang::Native:
 $PathChar = FilePathChar;

 |OS_PrintChar[char Value] -> void: 

  $if compiler::IsBackendC11():
   putchar(Value);
  $else:
   putchar(Value);


 |OS_PrintSpan[imut char[&] Buffer,uintptr Size] -> void:

  $if compiler::IsBackendC11():
   
   for [uintptr i = 0;i < Size;i++]:
     OS_PrintChar(Buffer[i]);

  $else:
   putspan(Buffer,Size);


 |OS_PrintClear[] -> void;
 |OS_Malloc[uintptr Size]: 

  $if compiler::IsBackendC11():
   ret malloc(Size);
  $else:
   ret __Malloc(Size);

 |OS_Free[byte[&] Block]:
   
   $if compiler::IsBackendC11():
    free(Block);
   $else:
    __Free(Block);

