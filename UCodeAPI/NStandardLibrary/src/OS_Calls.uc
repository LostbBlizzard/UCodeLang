ULang::Native:
 $PathChar export= FilePathChar;

 export |OS_PrintChar[char Value] -> void: 

  $if compiler::IsBackendC11():
   putchar(Value);
  $else:
   putchar(Value);


 export |OS_PrintSpan[imut char[&] Buffer,uintptr Size] -> void:

  $if compiler::IsBackendC11():
   
   for [uintptr i = 0;i < Size;i++]:
     OS_PrintChar(unsafe Buffer[i]);

  $else:
   putspan(Buffer,Size);


 export |OS_PrintClear[] -> void;
 export |OS_Malloc[uintptr Size]: 

  $if compiler::IsBackendC11():
   ret malloc(Size);
  $else:
   ret __Malloc(Size);

 export |OS_Free[byte[&] Block]:
   
   $if compiler::IsBackendC11():
    free(Block);
   $else:
    __Free(Block);

