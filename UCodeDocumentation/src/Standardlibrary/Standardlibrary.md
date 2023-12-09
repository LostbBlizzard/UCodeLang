# Standardlibrary Types


The Standard library is intended to be the list of usefull types and funcions that most GameDevs May need.


All of the Standardlibrary is Under The ULang Namespace.

```
use ULang;
```

All of the Standardlibrary Part of StandardLibarary Module By UCodeLang.

```
import UCodeLang::StandardLibarary[0:0:0];
```


## Containers
| Name | Type | Description |
|--- | --- |  --- | 
[Array\<T,(uintptr:Size)>](./Containers/Array.md) | Generic Class | A Fixed-size sequence container.
[Vector\<T>](./Containers/Vector.md) | Generic Class |  Used to store a List of Items.
[String_t\<T>](./Containers/String_t.md)| Generic Class. | Used as a base to  store a   sequence of contiguous sequence of any char type.


## Alias Types
| Name | Type | Description |
|--- | --- |  --- | 
[StringSpan](./Alias/StringSpan.md) | Alias | Same As StringSpan_t\<char>. Used refer to a contiguous sequence characters (text). 
[String8Span](./Alias/String8Span.md) | Alias | Same As StringSpan_t\<uft8>. Used refer to a contiguous sequence of utf8 characters (text). 
[String16Span](./Alias/String16Span.md) | Alias | Same As StringSpan_t\<uft16>. Used refer to a contiguous sequence of utf16 characters (text). 
[String32Span](./Alias/String32Span.md) | Alias | Same As StringSpan_t\<uft32>. Used refer to a contiguous sequence of utf32 characters (text). 
 
## ReferenceTypes
| Name | Type | Description |
|--- | --- |  --- | 
[StringSpan_t\<T>](./ReferenceTypes/StringSpan.md) | Generic Class | used as a base to  reference a contiguous sequence of any char type.
[Span\<T>](./ReferenceTypes/Span.md) | Generic Class | used store to reference a contiguous sequence of type T.

## Math Types
| Name | Type | Description |
|--- | --- |  --- | 
[Math](./Math/Math.md)  | Class | Contains Mathematical Functions.
[Vec2](./Math/Vec2.md)  | Alias | Same As Vec2_t\<float>. A vector with X and Y.
[Vec3](./Math/Vec3.md)  | Alias | Same As Vec3_t\<float>. A vector with X and Y.
[Vec2i](./Math/Vec2i.md)  | Alias | Same As Vec2i_t\<int>. A vector with X , Y and Z for float types.
[Vec3i](./Math/Vec3i.md) | Alias | Same As Vec2i_t\<int>. A Vector with X , Y and Z for int types.
[Vec2_t\<T>](./Math/Vec2_t.md)  | Generic Class | A Vector Base with X , Y  for int types.
[Vec3_t\<T>](./Math/Vec3_t.md) | Generic Class | A Vector Base with X , Y , Z for float types.
[Vec2i_t\<T>](./Math/Vec2i_t.md)| Generic Class | A Vector Base with X , Y. for int types.
[Vec3i_t\<T>](./Math/Vec3_t.md)| Generic Class | A Vector Base with X , Y and Z for int types.

## Error Handleing
| Name | Type | Description |
|--- | --- |  --- | 
[Optional\<T>](./ErrorHandleing/Optional.md) | Generic Enum |  Contains a value or Null 
[Result\<T,E>](./ErrorHandleing/Result.md) | Generic Enum | Contains a Success or Error.

## Memory 
| Name | Type | Description |
|--- | --- |  --- | 
[Unique_ptr\<T>](./Memory/Unique_ptr.md)| Generic Class | A Type that Owns and manages another object on the heap.
[Shared_ptr\<T>](./Memory/Shared_ptr.md)| Generic Class | A Type that Shares ownership  another object on the heap.
[LowLevel](./Memory/LowLevel.md)| Class | A Type That has LowLevel funcions like memcpy,memcpy,malloc.
[Ptr\<T>](./Memory/Ptr.md) | Generic Class | A Type that allows give C pointer semantics.  
[VoidPtr](./Memory/VoidPtr.md) | Alias | A pointer that has no type with it.
[Ptr\<void>](./Memory/VoidPtr.md) | Class Specialization | A pointer that has no type with it.
## Input/Output System
| Name | Type | Description |
|--- | --- |  --- | 
## Input/Output File
| Name | Type | Description |
|--- | --- |  --- | 
## Input/Output Networking
| Name | Type | Description |
|--- | --- |  --- | 
## Input/Output Serialization
| Name | Type | Description |
|--- | --- |  --- | 


## Miscellaneous

| Name | Type | Description |
|--- | --- |  --- | 
[Empty](./Miscellaneous/Empty.md) | class | A Zero-sized type. 
[Buffer_t\<T>](./Miscellaneous/StringBuffer_t.md) | Generic trait | Gives the types that have continuous elements helper funcions such as Sort,Find and Filter.
[StringBuffer_t\<T>](./Miscellaneous/StringBuffer_t.md) | Generic trait | Gives the types that are strings funcions such as StartWith,EndWith and GetLines.
[Range_t\<T>](./Miscellaneous/Ranges.md#range) |
[RangeFrom_t\<T>](./Miscellaneous/Ranges.md#rangefrom) | 
[RangeTo_t\<T>](./Miscellaneous/Ranges.md#rangeto) | 
[RangeFull_t\<T>](./Miscellaneous/Ranges.md#rangefull_t) | 
[RangeInclusive_t\<T>](./Miscellaneous/Ranges.md#rangeinclusive_t) | 
[RangeToInclusive_t\<T>](./Miscellaneous/Ranges.md#rangetoinclusive_t) | 


## Notes

Under the hood the Standardlibrary uses [NStandardlibrary API module](./APIs/NStandardlibrary.md) to do  operating system calls.