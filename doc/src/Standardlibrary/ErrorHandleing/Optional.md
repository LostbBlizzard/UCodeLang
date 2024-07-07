# Optional\<T>(Standardlibrary/ErrorHandleing)

Optional is a type that is part of the standard library. that can hold either a value of type T or nothing (Null). 

The Optional type Can be used like so.
```cpp
int? MayBeNull = Null;
Optional<int> MayBeNull2 = Null;
```
Both being equivalent.


The Optional is an [enum](../../SyntaxAndSeamantics/Types/Enum.md) and here its implementation.

```cpp
$Optional<T>:
 Opt(T opt),
 Null
```

The Optional type allows you to have 
- Optional struct fields
- Optional function arguments
- Nullable pointers
- default values


```cpp
|divide[float numerator,float denominator] -> float?:
  if denominator == 0:
    ret Null;
  else:
    ret Opt(numerator / denominator);

|main[]:
  var result = divide(2.0, 3.0);
 
  match result:
    Opt(out x):Fmt::Print("Result: {x}"),
    Null:Fmt::Print("Cannot divide by 0"),
```

# Question Mark Operator, 
The question mark operator, ? allows for easier use of the optional type.

It lets.
```cpp
|AddLastNumbers[int[]& stack] -> int?:
    var a = stack.Pop();
    var b = stack.Pop();

    if Opt(a,out vala) && Opt(b,out valb):
      ret Opt(vala + valb);

    ret Null;
```
To become.

```cpp
|AddLastNumbers[int[]& stack] -> int?:
  ret Opt((?stack.Pop()) + (?stack.Pop()))
```

When starting an expression with ? UCodeLang will check if its Null and will do an early return if it is.

makeing your life easier.


## Notes

All Examples is taken from [Rust](https://doc.rust-lang.org/std/option/) as UCodeLang was directly inspired by the  [Rust Programming Language](https://www.rust-lang.org/).