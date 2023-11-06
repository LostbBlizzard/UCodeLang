# Standardlibrary Types

The list of Type's in the standard library.
Order based on importance.


## Containers
| Name | Type | Description |
|--- | --- |  --- | 
[String](./Types/Classes/String.md) | Alias | Same As String_t\<char>. Used to store a sequence of characters (text).
[Vector\<T>](./Types/Generic/Vector.md) | Generic Class |  Used to store a List of Items.
[String8](./Types/Classes/String.md) | Alias | Same As String\<uft8>. Used to store a sequence of utf8 characters (text).
[String16](./Types/Classes/String.md) | Alias | Same As String\<uft16>. Used to store a   sequence of utf16 characters(text).
[String32](./Types/Classes/String.md) | Alias | Same As String\<uft32>. Used to store a   sequence of utf32 characters (text).
[String_t\<T>](./Types/alias/StringView.md)| Generic Class. | Used as a base to  store a   sequence of contiguous sequence of any char type.

## Reference Types
| Name | Type | Description |
|--- | --- |  --- | 
[StringSpan](./Types/Classes/StringSpan.md) | Alias | Same As StringSpan_t\<char>. Used refer to a contiguous sequence characters (text). 
[String8Span](./Types/Classes/StringSpan.md) | Alias | Same As StringSpan_t\<uft8>. Used refer to a contiguous sequence of utf8 characters (text). 
[String16Span](./Types/Classes/StringSpan.md) | Alias | Same As StringSpan_t\<uft16>. Used refer to a contiguous sequence of utf16 characters (text). 
[String32Span](./Types/Classes/StringSpan.md) | Alias | Same As StringSpan_t\<uft32>. Used refer to a contiguous sequence of utf32 characters (text). 
[StringSpan_t\<T>](./Types/alias/StringView.md)| Generic Class. | Used as a base to refer to a contiguous sequence of any char type.

## Math Types
| Name | Type | Description |
|--- | --- |  --- | 
[Vec2]()  | Alias | Same As Vec2_t\<float>. A vector with X and Y.
[Vec3]()  | Alias | Same As Vec3_t\<float>. A vector with X and Y.
[Vec2i]()  | Alias | Same As Vec2i_t\<int>. A vector with X , Y and Z for float types.
[Vec3i]() | Alias | Same As Vec2i_t\<int>. A Vector with X , Y and Z for int types.
[Vec2_t\<T>](./Types/Generic/VecTypes.md)  | Generic Class | A Vector Base with X , Y  for int types.
[Vec3_t\<T>](./Types/Generic/VecTypes.md) | Generic Class | A Vector Base with X , Y , Z for float types.
[Vec2i_t\<T](./Types/Generic/VecTypes.md)| Generic Class | A Vector Base with X , Y. for int types.
[Vec3i_t\<T>](./Types/Generic/VecTypes.md)| Generic Class | A Vector Base with X , Y and Z for int types.
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
