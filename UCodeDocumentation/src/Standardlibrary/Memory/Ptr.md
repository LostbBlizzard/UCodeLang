# Ptr(Standardlibrary/Memory)

Ptr is Class that give the same pointer semantics like in C like languages avoiding automatic dereferencing and makes reassigning trivial.

```cpp
int num = 10;

Ptr<int> val = ptr(num);

val.Write(20);

Fmt::Print(num); // 20

int num2 = 4;
val = ptr(num2);

Fmt::Print(val.Get()); // 4 

```
## External functions
|  Signature |  Description
 --- | --- |
\|ptr[imut T& address] -> imut Ptr\<T>; | Makes a Ptr useing an address
\|ptr[T& address] -> Ptr\<T>; | Makes a Ptr useing an address

## Member functions
|  Signature |  Description
 --- | --- |
\|Get[imut this&] -> imut T&; | return the address of the pointer.
\|Get[this&] -> T&; | returns the address of the pointer.
\|Write[this&,move T val] -> void; | Writes the value at the pointer.
\|Write[this&,imut T4 val] -> void; | Writes the value at the pointer.

## Notes
Ptr will always the size of a pointer.