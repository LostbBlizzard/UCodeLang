# bitcast\<T>(keyword)


Converts a ptr into an other pointer or an uintptr.

```
|main[]:
  int& NullPtr = bitcast<int&>(0);
  
  uintptr PtrAsNumber = unsafe bitcast<uintptr>(NullPtr);

```

>[!IMPORTANT]
>This Is section Is incomplete
>
>This can cause undefined Behavior.