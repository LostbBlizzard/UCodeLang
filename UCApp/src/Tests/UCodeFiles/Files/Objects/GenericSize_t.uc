$Test<T>:
 static uintptr Size = sizof(T);

|Main[] -> uintptr:
 var A = Test<byte>::Size;
 ret Test<int>::Size;