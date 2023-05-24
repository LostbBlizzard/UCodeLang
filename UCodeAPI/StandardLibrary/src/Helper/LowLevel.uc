%ULang:
 
 $LowLevel:
  |IsNullPtr<T>[T& Ptr] -> bool:
   ret bitcast<uintptr>(Ptr) == 0;
  
  |NullPtr<T>[]:
   ret bitcast<T>(0);
 
  |Cast<X,T>[T Ptr] -> X:
   ret bitcast<X>(Ptr);
 
  |Malloc[size_t Size] -> void_ptr:
   ret Cast<void_ptr>(new byte[Size]);
  
  |Free[void_ptr Ptr] -> void:
   byte[&] BuffPtr = Cast<byte[&]>(Ptr);
   drop(BuffPtr);
  
  |Memcpy[void_ptr destination, umut void_ptr source, size_t num] -> void:
   Debug::NotImplemented();
  
  |Memcpy[void_ptr destination, umut void_ptr source, size_t num] -> void:
   Debug::NotImplemented();

  |Memmove[void_ptr destination, umut void_ptr source, size_t num] -> void:
   Debug::NotImplemented();

   