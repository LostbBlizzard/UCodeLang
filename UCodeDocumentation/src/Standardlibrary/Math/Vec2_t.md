# Vec2_t(Standardlibrary/Math)

Not to be confused with [Vector](../Containers/Vector.md).

Vec2_t is the base type of a mathematical vector with an x and y value.

```cpp
Vec2_t<float> pos = [5,4];
pos += [10,5];
Fmt::Print(pos);// [X:15,Y:9]
```

Vec2_t is mostly used for position,size and rotation others things that need an x and y value.


## Fields functions

```cpp
$Vec2_t<T>:
 T X;
 T Y;
```

## Member functions
|  Signature |  Description
 --- | --- |
\|new[this&] -> void; | Makes a Vec2 with 0,0 coordinate.
\|new[this&,T x,T y] -> void; | Makes a Vec2 with the coordinate.
\|+=[this&,imut this& other] -> void; |
\|+[imut  this&,imut this& other] -> this; |  
\|==[imut this&,imut this& other] -> bool; | 
\|!=[imut this&,imut this& other] -> bool; | 