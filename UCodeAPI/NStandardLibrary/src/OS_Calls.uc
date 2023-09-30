ULang::Native:
 $PathChar = FilePathChar;

 |OS_PrintChar[char Value] -> void: 

  $if compiler::IsBackendC89():
   putchar(Value);
  $else:
   __LogChar(Value);


 |OS_PrintSpan[imut char[&] Buffer,uintptr Size] -> void:

  $if compiler::IsBackendC89():
   
   for [uintptr i = uintptr(0);i < Size;i++]:
     //OS_PrintChar(Buffer[i]);

  $else:
   __LogBuffer(Buffer,Size);


 |OS_PrintClear[] -> void;
 |OS_Malloc[uintptr Size]: 

  $if compiler::IsBackendC89():
   ret malloc(Size);
  $else:
   ret __Malloc(Size);

 |OS_Free[byte[&] Block]:
   
   $if compiler::IsBackendC89():
    free(Block);
   $else:
    __Free(Block);

