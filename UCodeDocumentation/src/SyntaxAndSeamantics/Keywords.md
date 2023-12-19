# Keywords
Keywords are reserved identifiers that have special meanings to the compiler. They can't be used as identifiers in your program
 
this article lists keywords.


## Built-in Types

All keywords that can be used as a type.

| Keyword | short Description
|--- | --- |
 [int](./Builtin/ints.md) | the int32 type
 [byte](./Builtin/ints.md) | the uint8 type
 [bool](./Builtin/bool.md) | the true or false type
 [uintptr](./Builtin/ints.md) | a unsigned int thats the size of a pointer.
 [intptr](./Builtin/ints.md) | a signed int thats the size of a pointer.
 [float](./Builtin/float.md) | the float32 type 
 [float32](./Builtin/float.md) | An IEEE floating point number.
 [float64](./Builtin/float.md) | An IEEE floating point number.
 [char](./Builtin/char.md) | the ASCII character type.
 [utf8](./Builtin/char.md) | the utf8 character type.
 [utf16](./Builtin/char.md) | the utf16 character type.
 [utf32](./Builtin/char.md) | the utf32 character type.
 [uint8](./Builtin/ints.md) |  an unsigned int thats the size of a one byte.
 [uint16](./Builtin/ints.md) | an unsigned int thats the size of a two bytes.
 [uint32](./Builtin/ints.md) | an unsigned int thats the size of a four bytes.
 [uint64](./Builtin/ints.md) | an unsigned int thats the size of a eight bytes.
 [int8](./Builtin/ints.md) | an signed int thats the size of a one byte.
 [int16](./Builtin/ints.md) | an signed int thats the size of a two bytes.
 [int32](./Builtin/ints.md) | an signed int thats the size of a four bytes.
 [int64](./Builtin/ints.md) | an signed int thats the size of a eight bytes.
 [typeinfo](./Builtin/TypeInfo.md) | read type information.

## type keywords
All keywords that can be used to make a type.
| Keyword | short Description
|--- | --- |
[enum](./Types/Enum.md) | a group of constants
[trait](./Types/trait.md) | shareing behavior between types
[tag](./Types/tag.md) | add information about functions and classes
[dynamic](./Types/dynamic.md) |  dynamically dispatched traits

## memory keyword
All keywords that can that deals with memory.
| Keyword | short Description
|--- | --- |
[new](./Memory/new.md) | allocate memory
[drop](./Memory/drop.md) | free memory. 
[unsafe](./Memory/unsafe.md) | lets you do unsafe operations.
[bitcast](./Memory/bitcast.md) | reinterpret the bytes of a type.
[shr](./Memory/shr.md) | make a shared pointer.
[unq](./Memory/unq.md) | make a unique pointer.

## varable types keywords
All keywords that can that deals changes how a varable works.
| Keyword | short Description
|--- | --- |
[static](./VarableTypes/static.md) | global variables
[thread](./VarableTypes/thread.md) | global variables but one per thread.
[eval](./VarableTypes/eval.md) | compile time constant expression.

## Compile Time Keywords
All keywords that can that lets you do stuff at compile time.

| Keyword | short Description
|--- | --- |
[compiler](./CompileTime/compiler.md) | read compiler settings
[invaild](./CompileTime/invalid.md) | throw a compile time error
[type](./CompileTime/type.md)  | get TypeInfo from an Type
[typeof](./CompileTime/type.md) | get TypeInfo from an expression
[bind](./CompileTime/type.md)  | use TypeInfo as a type.
[valid](./CompileTime/valid.md) | check a statement has errors.

## miscellaneous keywords

All keywords that dont fit in the other categories.

| Keyword | short Description
|--- | --- |
[for](./Miscellaneous/for.md) | loop over a list of items
[import](./Miscellaneous/import.md) | import a library
[out](./Miscellaneous/out.md) | a second output value
[panic](./Miscellaneous/Panic.md) | throw a runtime error
[use](./Miscellaneous/use.md) | use a namespace