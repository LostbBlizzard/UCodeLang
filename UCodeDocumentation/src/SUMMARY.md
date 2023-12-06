# Summary

- [Language and Why](./About.md)


# User Guide
- [Geting Started Users](./GetingStartedUsers.md)
  - [ULangModule](./Users/ULangModule.md) 
  - [BuildScript.uc](./Users/BuildScript.md)

- [UCode Command line tool](./UCodeCL/UCodeCl.md)

# Implementer Guide

- [Geting Started Implementers](./ForImplementers.md)


- [Library API Reference](./ForImplementers/APIReference.md)
  - [C API Reference](./ForImplementers/Reference/CAPI.md)
  - [Compiler](./ForImplementers/Reference/Compiler/Compiler.md)
  - [ModuleFile](./ForImplementers/Reference/Compiler/ModuleFile.md)
  - [UClib](./ForImplementers/Reference/Compiler/UClib.md)
  - [UCodeBackEnd](./ForImplementers/Reference/BackEnds/UCodeBackEnd.md)
  - [C11BackEnd](./ForImplementers/Reference/BackEnds/C11BackEnd.md)
  - [WasmBackEnd](./ForImplementers/Reference/BackEnds/WasmBackEnd.md)


# Language Guide

- [Syntax and semantics](./SyntaxAndSemantics.md)
  - [Syntax](./SyntaxAndSeamantics/Syntax.md)

  - [Memory Management](./SyntaxAndSeamantics/MemoryManagement.md)

  - [Classes](./SyntaxAndSeamantics/Concepts/Class.md)

  - [Functions](./SyntaxAndSeamantics/Concepts/function.md)

  - [Address](./SyntaxAndSeamantics/Concepts/Address.md)

  - [Casting](./SyntaxAndSeamantics/Concepts/Casting.md)

  - [Object Constructer](./SyntaxAndSeamantics/Concepts/AnonymousObjectConstructer.md)

  - [Error Handing](./SyntaxAndSeamantics/Concepts/ErrorHandleing.md)

  - [Lambdas](./SyntaxAndSeamantics/Concepts/Lambda.md)

  - [Constructors & Destructors](./SyntaxAndSeamantics/Concepts/ConstructorsAndDestructors.md)

  - [Traits](./SyntaxAndSeamantics/Concepts/traits.md)

  - [Access Mpdifiers](./SyntaxAndSeamantics/Concepts/Access%20Modifiers.md)

  - [Parameter Packs](./SyntaxAndSeamantics/Concepts/parameter%20packs.md)

  - [Conditional Compliation](./SyntaxAndSeamantics/Concepts/conditional%20compliation.md)

  - [Generics](./SyntaxAndSeamantics/Concepts/generic.md)

  - [Namespaces](./SyntaxAndSeamantics/Concepts/namespaces.md)

  - [NamingConvention](./SyntaxAndSeamantics/Concepts/NamingConvention.md)


- [Standard library](./Standardlibrary/Standardlibrary.md)
  - [Containers]()
    - [Array\<T>](./Standardlibrary/Containers/Array.md)
    - [Vector\<T>](./Standardlibrary/Containers/Vector.md)
    - [String_t\<T>](./Standardlibrary/Containers/String_t.md)

  - [Alias Types]()
    - [String](./Standardlibrary/Alias/String.md)
    - [StringSpan](./Standardlibrary/Alias/StringSpan.md)
  
    - [String8](./Standardlibrary/Alias/String8.md)
    - [String8Span](./Standardlibrary/Alias/String8Span.md)
  
    - [String16](./Standardlibrary/Alias/String16.md)
    - [String16Span](./Standardlibrary/Alias/String16Span.md)

    - [String32](./Standardlibrary/Alias/String32.md)
    - [String32Span](./Standardlibrary/Alias/String32Span.md)
  
  - [ReferenceTypes]()
    - [StringSpan_t<\T>](./Standardlibrary/ReferenceTypes/StringSpan.md)
    - [Span<\T>](./Standardlibrary/ReferenceTypes/Span.md)

  - [Math]() 
    - [Vec2](./Standardlibrary/Math/Vec2.md)
    - [Vec3](./Standardlibrary/Math/Vec3.md)
    - [Vec2i](./Standardlibrary/Math/Vec2i.md)
    - [Vec3i](./Standardlibrary/Math/Vec3i.md)

    - [Vec2_t\<T>](./Standardlibrary/Math/Vec2_t.md)
    - [Vec3_t\<T>](./Standardlibrary/Math/Vec3_t.md)
    - [Vec2i_t\<T>](./Standardlibrary/Math/Vec2i_t.md)
    - [Vec3i_t\<T>](./Standardlibrary/Math/Vec3i_t.md)

  - [Memory]()
    - [Unique_ptr\<T>](./Standardlibrary/Memory/Unique_ptr.md)
    - [Shared_ptr\<T>](./Standardlibrary/Memory/Shared_ptr.md)

  - [IO]()
    - [System]()
      - [Fmt](./Standardlibrary/IOSystem/Fmt.md)

    - [File]()
      - [File](./Standardlibrary/IOFile/File.md)

    - [Networking]()

    - [Serialization]()

  - [Ungrouped]()
    - [Empty](./Standardlibrary/Ungrouped/Empty.md)
- [KeyWords](./SyntaxAndSeamantics/Keywords.md)
  - [int](./SyntaxAndSeamantics/Types/ints.md)
  - [new](./SyntaxAndSeamantics/Keywords/new.md)
  - [drop](./SyntaxAndSeamantics/Keywords/drop.md)
  - [eval](./SyntaxAndSeamantics/Keywords/eval.md)
  
