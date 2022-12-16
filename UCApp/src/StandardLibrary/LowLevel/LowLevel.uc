%ULang: 
 $LowLevel: 
  |Cast<T,U>[T Value] -> U;
  |Malloc[uintptr Size] -> ptr;
  |Calloc[uintptr Size] -> ptr;
  |Realloc[ptr oldptr,uintptr Size] -> ptr;
  |Free[ptr ptr] -> void;
  
  // LowLevel
 $ptr = !uintptr; 
 
 // ULang
