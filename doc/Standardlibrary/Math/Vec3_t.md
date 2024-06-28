# Vec3_t(Standardlibrary/Math)

Not to be confused with [Vector](../Containers/Vector.md).

Vec2i_t is the base type of a mathematical vector with an x and y value.

```cpp
Vec3_t<float> pos = [5,4,0];
pos += [10,5,3];
Fmt::Print(pos);// [X:15,Y:9,Z:3]
```

Vec2i_t is mostly used for position,size and rotation others things that need an x and y value.


## Fields functions

```cpp
$Vec3_t<T>:
 T X;
 T Y;
 T Z;
```

## Member functions
|  Signature |  Description
 --- | --- |
\|new[this&] -> void; | Makes a Vec3 with 0,0,0 coordinate.
\|new[this&,T x,T y,T z] -> void; | Makes a Vec3 with the coordinates.
\|+=[this&,imut this& other] -> void; |
\|+[imut  this&,imut this& other] -> this; |  
\|==[imut this&,imut this& other] -> bool; | 
\|!=[imut this&,imut this& other] -> bool; | 