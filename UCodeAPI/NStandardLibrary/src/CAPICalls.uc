
$if compiler::IsBackendC11():
 $FilePathChar = char;
 $FileHandle = uintptr;
 $Socket = uintptr;

 extern "c" |putchar[char Val] -> void;
 extern "c" |malloc[uintptr Size] -> byte[&];
 extern "c" |free[byte[&] Block] -> void;
  