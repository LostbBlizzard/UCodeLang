%ULang:

 $Debug:
  |NotImplemented[]:
   panic("Not Implemented");

   //if publish mode
   //invaid("Not Implemented");

  |UnreachablePath[]:
   panic("Reached an UnreachablePath");
  
  |OutOfBounds[Size_t Index,Size_t BoundsSize]:
   panic("Out Of Bounds");

  |NullPtrAccess[]:
   panic("Null Ptr Access");