

$Cool<T>[T] enum:
 BaseSize = sizeof(T),

$SomeClass:
 Cool<uint32> CoolV;
 
|main[] => sizeof(Cool<uint32>);
