ULang::Native:
  export |CPU_Memcopy[byte[&] destination,byte[&] source,uintptr num]: 
    $if compiler::IsBackendC11():
     memcopy(destination,source,num);
    $else:
     __Memcopy(destination,source,num);
      
  export |CPU_Memmove[byte[&] destination,byte[&] source,uintptr num]:
   $if compiler::IsBackendC11():
     memmove(destination,source,num);
    $else:
     __Memmove(destination,source,num);