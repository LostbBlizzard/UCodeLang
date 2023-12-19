# String_t(Standardlibrary/Containers)


Not to be confused with [StringSpan](../ReferenceTypes/StringSpan.md).
The String Type is used represent sequences of characters.

The String type Can be used like so.

```cpp
String_t<char> mystr = "Hello World";
```

UCodeLang has many different char types which explains why the type is generic.

```cpp
String_t<char> mystr = "Hello World";
String_t<uft8> mystr = "Hello World";
String_t<uft16> mystr = "Hello World";
String_t<uft32> mystr = "Hello World";
```

Most of type the you will be useing the alies [String](../Alias/String.md),[String8](../Alias/String8.md),[String16](../Alias/String16.md),[String32](../Alias/String32Span.md).
Instead of directly instantiating the type.

```cpp
String mystr = "Hello World";
String8 mystr = "Hello World";
String16 mystr = "Hello World";
String32 mystr = "Hello World";
```

like most other languages you can concatenate strings togeter.

```cpp
String mystr = "Hello";
mystr += " World";

Fmt::Print(mystr);
```
## Member Aliases
$StrSpan = StringSpan_t\<T>;

## Member functions
|  Signature |  Description
 --- | --- |
unsafe \|Data[imut this&] -> imut T[&]; | Gets the Raw Data Pointer.
unsafe \|Data[this&] -> T[&]; | Gets the Raw Data Pointer.
 \|Size[imut this&] -> uintptr; | Gets the Size of the Array.
 \|Capacity[imut this&] -> uintptr; |  Gets how much memory the string allocated to hold its contents
 \|[][imut this&,uintptr index] -> imut T&; | Gets the address of an element.
\|[][this&,uintptr index] -> T&; |  Gets the address of an element.
\|+[this&,imut this& other] -> this; | Concatenate a string to make a new string
\|+[this&,imut StrSpan& other] -> this; | Concatenate a Stringspan to make a new string
\|+=[this&,imut this& other] -> this; | Concatenate a string to a new string
\|+=[this&,imut StrSpan& other] -> this; | Concatenate a stringspan to a new string




## Inherited traits
|  Trait |  Description
 --- | --- |
[StringBuffer_t\<T>](../Miscellaneous/StringBuffer_t.md) | Gives the types that are strings funcions such as StartWith,EndWith and GetLines.
## Notes

Be aware that string literals like "hello" are [StringSpans](../ReferenceTypes/StringSpan.md)