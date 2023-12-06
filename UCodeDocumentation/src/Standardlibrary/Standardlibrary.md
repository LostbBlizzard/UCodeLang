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
 

## Math Types
| Name | Type | Description |
|--- | --- |  --- | 
[Vec2]()  | Alias | Same As Vec2_t\<float>. A vector with X and Y.
[Vec3]()  | Alias | Same As Vec3_t\<float>. A vector with X and Y.
[Vec2i]()  | Alias | Same As Vec2i_t\<int>. A vector with X , Y and Z for float types.
[Vec3i]() | Alias | Same As Vec2i_t\<int>. A Vector with X , Y and Z for int types.
[Vec2_t\<T>](./Types/Generic/VecTypes.md)  | Generic Class | A Vector Base with X , Y  for int types.
[Vec3_t\<T>](./Types/Generic/VecTypes.md) | Generic Class | A Vector Base with X , Y , Z for float types.
[Vec2i_t\<T>](./Types/Generic/VecTypes.md)| Generic Class | A Vector Base with X , Y. for int types.
[Vec3i_t\<T>](./Types/Generic/VecTypes.md)| Generic Class | A Vector Base with X , Y and Z for int types.

## Error Handleing
| Name | Type | Description |
|--- | --- |  --- | 
[Optional\<T>](./Types/Generic/optional.md) | Generic Enum |  Contains a value or Null 
[Result\<T,E>](./Types/Generic/optional.md) | Generic Enum | Contains a Success or Error.

## Memory 
| Name | Type | Description |
|--- | --- |  --- | 
[Unique_ptr\<T>](./Types/Generic/Unique_ptr.md)| Generic Class | A Type that Owns and manages another object on the heap.
[Shared_ptr\<T>](./Types/Generic/Shared_ptr.md)| Generic Class | A Type that Shares ownership  another object on the heap.
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

## Ungrouped

| Name | Type | Description |
|--- | --- |  --- | 
[Empty](./Ungrouped/Empty.md) | class | A Zero-sized type. 

## Notes

Under the hood the Standardlibrary uses [NStandardlibrary API module](./APIs/NStandardlibrary.md) to do  operating system calls.