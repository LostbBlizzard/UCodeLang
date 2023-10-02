

$if compiler::IsBackendUCodeVM():
 $FilePathChar = char;
 $FileHandle = uintptr;
 $Socket = uintptr;

 //Cout/Cin
 extern dynamic |__Log[imut char[&] Buff] -> void;

 extern dynamic |__LogChar[char Value] -> void;

 extern dynamic |__LogBuffer[imut char[&] Value,uintptr Size] -> void;

 extern dynamic |__ReadChar[] -> char;

 //Memory
 extern dynamic |__Malloc[uintptr Size] -> byte[&];

 extern dynamic |__Free[byte[&] Block] -> void;

 //File IO

 extern dynamic |__OpenFile[FilePathChar[&] Path,uintptr Size] -> FileHandle;

 extern dynamic |__CloseFile[FileHandle File] -> void;

 extern dynamic |__IsFileOpen[FileHandle File] -> void;

 //Network IO

 