$SomeType<T> = match type(T == int):
  true => type(uint32);
  false => type(uint64);
;

|main[] => sizeof(SomeType<int>) != sizeof(SomeType<bool>);
