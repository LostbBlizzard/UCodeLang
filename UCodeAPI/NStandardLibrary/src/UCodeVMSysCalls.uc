

//Testing

extern dynamic |__Debug[int A] -> int;


//Cout/Cin
extern dynamic |__Log[umut char[&] Buff] -> void;

extern dynamic |__LogChar[char Value] -> void;

extern dynamic |__LogBuffer[char[&] Value,uintptr Size] -> void;

extern dynamic |__ReadChar[] -> char;

//Memory
extern dynamic |__Malloc[uintptr Size] -> byte[&];

extern dynamic |__Free[byte[&] Block] -> void;

//File IO

$FilePathChar = char;
$FileHandle = uintptr;

extern dynamic |__OpenFile[FilePathChar[&] Path,uintptr Size] -> FileHandle;

extern dynamic |__CloseFile[FileHandle File] -> void;

extern dynamic |__IsFileOpen[FileHandle File] -> void;

//Network IO

$Socket = uintptr;