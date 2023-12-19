# File(Standardlibrary/IO/File)

The File Type allows for opening files,reading and writing files.

```cpp
var file = ?File::Open("my.txt");
```



## File Open functions
|  Signature |  Description
 --- | --- |
\|Open[[PathSpan](./PathSpan.md) filename] -> [OpenedFile](./OpenedFile.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md) | Opens file returns an error or an OpenedFile.
\|Open[imut [Path](Path.md)& filename] ->  [OpenedFile](./OpenedFile.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Opens file returns an error or an OpenedFile.
\|Open[imut [String](../Containers/String_t.md)& filename] ->  [OpenedFile](./OpenedFile.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Opens file returns an error or an OpenedFile.
\|Open[imut [StringSpan](../ReferenceTypes/StringSpan.md)& filename] ->  [OpenedFile](./OpenedFile.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Opens file returns an error or an OpenedFile.


## File Read String functions
|  Signature |  Description
 --- | --- |
\|ReadString[imut [StringSpan](../ReferenceTypes/StringSpan.md)& filename] ->  [String](./../Containers/String_t.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to file a as a string or returns an error if there is one.
\|ReadString[imut [Path](Path.md)& filename] ->  [String](./../Containers/String_t.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to read file a as a string or returns an error if there is one.
\|ReadString[[PathSpan](./PathSpan.md) filename filename] ->  [String](./../Containers/String_t.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes read to file a as a string or returns an error if there is one.
\|ReadString[imut [String](../Containers/String_t.md) filename] ->  [String](./../Containers/String_t.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes read to file a as a string or returns an error if there is one.

## File Read Bytes functions
|  Signature |  Description
 --- | --- |
\|ReadBytes[imut [StringSpan](../ReferenceTypes/StringSpan.md) filename] ->  [Vector\<byte>](./../Containers/Vector.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to read file a as a Vector of bytes or returns an error if there is one.
\|ReadBytes[imut [String](../Containers/String_t.md)& filename] ->  [Vector\<byte>](./../Containers/Vector.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to read file a as a Vector of bytes or returns an error if there is one.
\|ReadBytes[imut [PathSpan](./PathSpan.md) filename] ->  [Vector\<byte>](./../Containers/Vector.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to read file a as a Vector of bytes or returns an error if there is one.
\|ReadBytes[imut [Path](./Path.md)& filename]& ->  [Vector\<byte>](./../Containers/Vector.md) ! [OpenFileError](../ErrorHandleing/OpenFileError.md)  | Tryes to file read a as a Vector of bytes or returns an error if there is one.

## File Write String functions
|  Signature |  Description
 --- | --- |
\|Write[imut [StringSpan](../ReferenceTypes/StringSpan.md) filename,imut [StringSpan](../ReferenceTypes/StringSpan.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [String](../Containers/String_t.md)& filename,imut [StringSpan](../ReferenceTypes/StringSpan.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [PathSpan](./PathSpan.md) filename,imut [StringSpan](../ReferenceTypes/StringSpan.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [Path](./Path.md) &filename,imut [StringSpan](../ReferenceTypes/StringSpan.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.

## File Write Bytes functions
|  Signature |  Description
 --- | --- |
 \|Write[imut [StringSpan](../ReferenceTypes/StringSpan.md) filename,imut [Span<byte>](../ReferenceTypes/Span.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [String](../Containers/String_t.md)& filename,imut [Span<byte>](../ReferenceTypes/Span.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [PathSpan](./PathSpan.md) filename,imut [Span<byte>](../ReferenceTypes/Span.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
\|Write[imut [Path](./Path.md) &filename,imut [Span<byte>](../ReferenceTypes/Span.md) data] ->  Empty ! [FileOpenWriteError](../ErrorHandleing/FileOpenWriteError.md.md)  | Writes data the a file or returns an error if there is one.
