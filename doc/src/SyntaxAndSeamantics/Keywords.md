# Keywords

Keywords are reserved identifiers that have special meanings to the compiler. They can't be used as identifiers in your program

this article lists keywords.

## Built-in Types

All keywords that can be used as a type.

| Keyword                           | short Description                           |
| --------------------------------- | ------------------------------------------- |
| [int](./Builtin/int32.md)         | an alias to [int32](./Builtin/int32.mdmd)   |
| [byte](./Builtin/uint8.md)        | an alias to [uint8](./Builtin/uint8.md)     |
| [bool](./Builtin/bool.md)         | the true or false type                      |
| [uintptr](./Builtin/uintptr.md)   | a unsigned int thats the size of a pointer. |
| [sintptr](./Builtin/sintptr.md)   | a signed int thats the size of a pointer.   |
| [float](./Builtin/float.md)       | An 32 bit floating point number.            |
| [float32](./Builtin/float.md)     | An 32 bit floating point number.            |
| [float64](./Builtin/float64.md)   | An 64 bit floating point number.            |
| [char](./Builtin/char.md)         | an ASCII character type.                    |
| [utf8](./Builtin/utf8.md)         | an utf8 character type.                     |
| [utf16](./Builtin/utf16.md)       | an utf16 character type.                    |
| [utf32](./Builtin/utf32.md)       | an utf32 character type.                    |
| [uint8](./Builtin/uint8.md)       | an one byte unsigned int.                   |
| [uint16](./Builtin/uint16.md)     | an two byte unsigned int.                   |
| [uint32](./Builtin/uint32.md)     | an four byte unsigned int.                  |
| [uint64](./Builtin/uint64.md)     | an eight bytes unsigned int.                |
| [int8](./Builtin/int8.md)         | an one byte signed int.                     |
| [int16](./Builtin/int16.md)       | an two byte signed int.                     |
| [int32](./Builtin/int32.md)       | an four byte signed int.                    |
| [int64](./Builtin/int64.md)       | an eight bytes signed int.                  |
| [typeinfo](./Builtin/TypeInfo.md) | represents compile type information.        |

## type keywords

All keywords that can be used to make a type.

| Keyword                       | short Description                           |
| ----------------------------- | ------------------------------------------- |
| [enum](./Types/Enum.md)       | a group of constants                        |
| [trait](./Types/trait.md)     | shareing behavior between types             |
| [tag](./Types/tag.md)         | add information about functions and classes |
| [dynamic](./Types/dynamic.md) | dynamically dispatched traits               |
| [functor](./Types/Functor.md) | Funcion Pointer/Lambda Object        |

## memory keyword

All keywords that can that deals with memory.

| Keyword                        | short Description                |
| ------------------------------ | -------------------------------- |
| [new](./Memory/new.md)         | allocate memory                  |
| [drop](./Memory/drop.md)       | free memory.                     |
| [unsafe](./Memory/unsafe.md)   | lets you do unsafe operations.   |
| [bitcast](./Memory/bitcast.md) | reinterpret the bytes of a type. |
| [shr](./Memory/shr.md)         | make a shared pointer.           |
| [unq](./Memory/unq.md)         | make a unique pointer.           |

## varable types keywords

All keywords that can that deals changes how a varable works.

| Keyword                            | short Description                    |
| ---------------------------------- | ------------------------------------ |
| [static](./VarableTypes/static.md) | global variables                     |
| [thread](./VarableTypes/thread.md) | global variables but one per thread. |
| [eval](./VarableTypes/eval.md)     | compile time constant expression.    |

## Compile Time Keywords

All keywords that can that lets you do stuff at compile time.

| Keyword                               | short Description               |
| ------------------------------------- | ------------------------------- |
| [compiler](./CompileTime/compiler.md) | read compiler settings          |
| [invaild](./CompileTime/invalid.md)   | throw a compile time error      |
| [type](./CompileTime/type.md)         | get TypeInfo from an Type       |
| [typeof](./CompileTime/type.md)       | get TypeInfo from an expression |
| [bind](./CompileTime/type.md)         | use TypeInfo as a type.         |
| [valid](./CompileTime/valid.md)       | check a statement has errors.   |

## miscellaneous keywords

All keywords that dont fit in the other categories.

| Keyword                             | short Description         |
| ----------------------------------- | ------------------------- |
| [for](./Miscellaneous/for.md)       | loop over a list of items |
| [import](./Miscellaneous/import.md) | import a library          |
| [out](./Miscellaneous/out.md)       | a second output value.    |
| [panic](./Miscellaneous/panic.md)   | throw a runtime error.    |
| [defer](./Miscellaneous/defer.md)   | throw a runtime error.    |
| [use](./Miscellaneous/use.md)       | use a namespace           |
