# Array(Standardlibrary/Containers)

Not to be confused with [AddressArray](../../SyntaxAndSeamantics/Concepts/AddressArray.md) and [NativeArrays](../../SyntaxAndSeamantics/Concepts/NativeArrarys.md).

The Array Type is a fixed-size collection of data items stored in contiguous memory with added safety features unlike [NativeArrays](../../SyntaxAndSeamantics/Concepts/NativeArrarys.md).

The Array type Can be used like so.

```cpp
int[10] Items;
Array<int,(10)> Items2;
```
Both being equivalent.

All the values in the array are stored on the stack as the size of the array is known at compile time. 

Arrays mainly are used if you know how many elements you will need at compile time as thay are faster then Vectors in most cases.

```cpp
int[] Items;
Items.Resize(3);//The Size Known at Compile Time.

Items[0] = 10;
Items[1] = 20;
Items[2] = Items[1];
```
Useing an array;
```cpp
int[3] Items;

Items[0] = 10;
Items[1] = 20;
Items[2] = Items[1];
```

## Member functions
|  Signature |  Description
 --- | --- |
unsafe \|Data[imut this&] -> imut T[&]; | Gets the Raw Data Pointer.
unsafe \|Data[this&] -> T[&]; | Gets the Raw Data Pointer.
 \|Size[imut this&] -> uintptr; | Get the Size of the Array.
 \|[][imut this&,uintptr index] -> imut T&; | Gets the address of an element.
\|[][this&,uintptr index] -> T&; |  Gets the address of an element.

## Inherited traits
|  Trait |  Description
 --- | --- |
[Buffer_t\<T>](../Miscellaneous/Buffer_t.md) | Gives the types that have continuous elements helper funcions such as Sort,Find and Filter.
## Notes

The array type will always be the size of it elements it will use the generic constant expression to get the size.

