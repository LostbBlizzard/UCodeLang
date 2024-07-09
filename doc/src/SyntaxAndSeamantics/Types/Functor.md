# Functor(KeyWord)

The Functor Keyword restricts generic type to be a [funcion pointer](../Concepts/FuncionPointer.md),[lambda object](../Concepts/Lambda.md)
or any type that have the '()' funcion on it with a set parameters and a return type.

This Keyword also allows automatic type duction for lambdas.
```cpp 

|Func<[functor [int a,int b] -> int]FuncType>[FuncType val,int a,int b]:
 ret val(a,b);

|main[]:
 ret Func([a,b] => a * b,10,5);
        // ^ both a and b are int because the functor states them as so.
```
