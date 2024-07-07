# Vector\<T>(Standardlibrary/Containers)

Not to be confused with [Vec2](../Math/Vec2.md) and [Vec3](../Math/Vec3.md).

The Vector Type is dynamically resizeable collection of data items stored in contiguous memory.

The Vector type Can be used like so.
```cpp
int[] Items;
Vector<int> Items2;
```
Both being equivalent.


Vector are dynamic so your able to add elements at runtime.

```cpp
int[] Items;
Items.Push(5);
Items.Push(10);
Items.Push(30);


Fmt::Print(Items[0]); \\ 5
Fmt::Print(Items[1]); \\ 10
Fmt::Print(Items[2]); \\ 30


Fmt::Print(Items.Size()); \\ 3
```

Vector can also be can be initialized with starting elements.
```cpp
int[] Items = [10,20,30];

Fmt::Print(Items[0]); \\ 5
Fmt::Print(Items[1]); \\ 10
Fmt::Print(Items[2]); \\ 30

Fmt::Print(Items.Size()); \\ 3
```

## Member functions
|  Signature |  Description
 --- | --- |
 \|new[this&] -> void; | Makes a New Vector.
 \|new[this&,imut this& Other] -> void; | Copys a Vector
 \|new[this&,imut this& Other] -> void; | Moves a Vector
 \|new\<T...>[this&,imut T items] -> void; |Makes a New Vector with the added elements.
\|new\<T...>[this&,moved T items] -> void; |Makes a New Vector with the added elements.
unsafe \|Data[imut this&] -> imut T[&]; | Gets the Raw Data Pointer.
unsafe \|Data[this&] -> T[&]; | Gets the Raw Data Pointer.
 \|Size[imut this&] -> uintptr; | Get the Size of the Array.
 \|Capacity[imut this&] -> uintptr; |  Gets how much memory the string allocated to hold its contents
 \|[][imut this&,uintptr index] -> imut T&; | Gets the address of an element.
\|[][this&,uintptr index] -> T&; |  Gets the address of an element.
\|Push[this&,uintptr index] -> T&; |  Adds the element to the end of the Vector.

## Inherited traits
|  Trait |  Description
 --- | --- |
[Buffer_t\<T>](../Miscellaneous/Buffer_t.md) | Gives the types that have continuous elements helper funcions such as Sort,Find and Filter.
## Notes