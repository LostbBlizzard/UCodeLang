# Math(Standardlibrary/Math)

Provides funcions for trigonometric, logarithmic, and other common mathematical functions.

```
|main[]:
 
 Fmt::Print(Math::PI);//~3.14 
 
 Fmt::Print(Math::Abs(-1.5));//1.5

 Fmt::Print(Math::Min(4.2,8.5));//4.2
```


## Member functions
|  Signature |  Description
 --- | --- |
|Min\<T>[T a,T b] -> T; | Returns the smaller of a int or a float.
|Max\<T>[T a,T b] -> T; | Returns the larger of a int or a float.
|Abs\<T>[T v] -> T; | Returns the absolute value of a int or a float.

# Notes

This Class is base on the [C# Math Class](https://learn.microsoft.com/en-us/dotnet/api/system.math?view=net-8.0)