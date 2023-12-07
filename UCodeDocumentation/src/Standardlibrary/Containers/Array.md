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

## Member functions


|  Signature |  Description
 --- | --- |
unsafe \|Data[imut this&] -> imut T[&]; | N/A
unsafe \|Data[this&] -> T[&]; | N/A
unsafe \|Size[imut this&] -> uintptr; | N/A
unsafe \|[][imut this&,uintptr index] -> T&; | N/A
unsafe \|[][this&,uintptr index] -> T&; | N/A 
