

$Cool<T>[T] enum:
 BaseSize = sizeof(T),

$SomeClass:
 Cool<uint32> CoolV;
 
|main[] => Cool<uint32>::BaseSize;
