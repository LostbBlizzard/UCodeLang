
$LowLevel:
 unsafe |Malloc[uintptr Size] => ULang::Native::OS_Malloc(Size);
 unsafe |Free[byte[&] Block] => ULang::Native::OS_Free(Block);
 unsafe |Memcopy[byte[&] destination,byte[&] source,uintptr num] -> void;
 unsafe |Memmove[byte[&] destination,byte[&] source,uintptr num] -> void;
