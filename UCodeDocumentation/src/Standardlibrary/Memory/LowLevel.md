# LowLevel(Standardlibrary/Memory)

LowLevel is Class that Provides Low Level memory operations and functions like in languages like C or C++.

```cpp
unsafe:
  VoidPtr p = LowLevel::Malloc(4);

  int& nump = bitcast<int&>(p);
  nump = 10;

  LowLevel::Free(p);

```


## Member functions
|  Signature |  Description
 --- | --- |
unsafe Malloc[uintptr size] -> VoidPtr; | allocate a block of memory in the heap.
unsafe Free[uintptr size] -> VoidPtr; | deallocates the memory from calloc, malloc, or realloc.
unsafe Realloc[VoidPtr ptr,uintptr size] -> VoidPtr; | deallocates the memory from calloc, malloc, or realloc.
unsafe Calloc[size_t num,uintptr size] -> VoidPtr; |Allocates memory for an array of num objects of size and initializes all bytes in the allocated storage to zero.
unsafe Memcpy[VoidPtr dest, const VoidPtr src, uintptr count] -> VoidPtr; |Copies count bytes from the object pointed to by src to the object pointed to by dest. Both objects are reinterpreted as arrays of bytes.
unsafe Memset[VoidPtr dest,byte value, uintptr count] -> VoidPtr; | Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).


## Notes

currently in the compiler it is unable to see these funcion like Malloc,Free Realloc to those specific IR instructions.meaning the compilers unable to optimize it as it just see the function as external calls.

Some Descrptions here are from [en.cppreference.com](https://en.cppreference.com/w/c/memory)