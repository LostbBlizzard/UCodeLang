
$LowLevel:
 export unsafe |Malloc[uintptr Size] => ULang::Native::OS_Malloc(Size);
 export unsafe |Free[byte[&] Block] => ULang::Native::OS_Free(Block);
 export unsafe |Memcopy[byte[&] destination,byte[&] source,uintptr num] -> void;
 export unsafe |Memmove[byte[&] destination,byte[&] source,uintptr num] -> void;
