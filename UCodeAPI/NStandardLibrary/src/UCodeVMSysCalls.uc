

$if compiler::IsBackendUCodeVM():
 $FilePathChar = char;
 $FileHandle = uintptr;
 $Socket = uintptr;

 //Cout/Cin

 extern dynamic |putchar[char Value] -> void;

 extern dynamic |putspan[imut char[&] Value,uintptr Size] -> void;

 extern dynamic |__ReadChar[] -> char;

 //Memory
 extern dynamic |__Malloc[uintptr Size] -> byte[&];

 extern dynamic |__Free[byte[&] Block] -> void;

 extern dynamic |__Memcopy[byte[&] destination,byte[&] source,uintptr num] -> void;
 extern dynamic |__Memmove[byte[&] destination,byte[&] source,uintptr num] -> void;
 //File IO

 extern dynamic |__OpenFile[FilePathChar[&] Path,uintptr Size] -> FileHandle;

 extern dynamic |__CloseFile[FileHandle File] -> void;

 extern dynamic |__IsFileOpen[FileHandle File] -> void;

 //Network IO

 