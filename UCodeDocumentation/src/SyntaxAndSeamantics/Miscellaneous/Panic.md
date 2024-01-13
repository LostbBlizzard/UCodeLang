# panic(keyword)

The panic keyword like in rust is used to represent a bug that has happen in the program so that the program may exit safely and to avoid putting the program in a unstable state.


```
|main[]:
 panic("I Must panic.");
```

The panic keyword takes in only one argument a [StringSpan](../../Standardlibrary/ReferenceTypes/StringSpan.md) or [String](../../Standardlibrary/Containers/String_t.md).

Depending on the runtime the program may exit right after reaching a panic or only unwind the stack to call [destructers](../Concepts/ConstructorsAndDestructors.md) and [defers](./defer.md).