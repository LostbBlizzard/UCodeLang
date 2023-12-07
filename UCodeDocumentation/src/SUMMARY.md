# Summary

- [Introduction](./Introduction.md)
- [Language and Why](./About.md)
- [Syntax](./SyntaxAndSeamantics/Syntax.md)

# User Guide
- [Geting Started Users](./GetingStartedUsers.md)
  - [ULangModule](./Users/ULangModule.md) 
  - [BuildScript.uc](./Users/BuildScript.md)

- [UCode Command line tool](./UCodeCL/UCodeCl.md)
  - [Commands](./UCodeCL/UCodeCl.md#commands)
    - [help](./UCodeCL/Commands/Help.md)
    - [new](./UCodeCL/Commands/New.md)
    - [build](./UCodeCL/Commands/Build.md)
    - [run](./UCodeCL/Commands/Run.md)
    - [get](./UCodeCL/Commands/Get.md)
    - [dump](./UCodeCL/Commands/Dump.md)
    - [clear](./UCodeCL/Commands/Clear.md)
    - [cpptoulangvm](./UCodeCL/Commands/cpptoulangvm.md)
    - [cppdirtoulangvm](./UCodeCL/Commands/cpptoulangvm.md)
    - [test](./UCodeCL/Commands/Test.md)
    - [updatetools](./UCodeCL/Commands/Uninstalltools.md)
    - [install](./UCodeCL/Commands/Install.md)
    - [uninstallmod](./UCodeCL/Commands/Uninstallmod.md)
    - [exits](./UCodeCL/Commands/Exits.md)
# Implementer Guide

- [Geting Started Implementers](./ForImplementers.md)


- [Library API Reference](./ForImplementers/APIReference.md)
  - [C API Reference](./ForImplementers/Reference/CAPI.md)
  - [Compilation](./ForImplementers/APIReference.md#compilation)
    - [Compiler](./ForImplementers/Reference/Compiler/Compiler.md)
    - [ModuleFile](./ForImplementers/Reference/Compiler/ModuleFile.md)
    - [UClib](./ForImplementers/Reference/Compiler/UClib.md)
    - [UCodeBackEnd](./ForImplementers/Reference/BackEnds/UCodeBackEnd.md)
    - [C11BackEnd](./ForImplementers/Reference/BackEnds/C11BackEnd.md)
    - [WasmBackEnd](./ForImplementers/Reference/BackEnds/WasmBackEnd.md)

  - [RunTime](./ForImplementers/APIReference.md#core)
    - [RunTimeLib](./ForImplementers/Reference/RunTime/RunTimeLib.md)
    - [UCodeRunTime](./ForImplementers/Reference/RunTime/UCodeRunTime.md)
    - [AnyInterpreter](./ForImplementers/Reference/RunTime/AnyInterpreter.md)
    - [Interpreter](./ForImplementers/Reference/RunTime/Interpreter.md)
    - [JitInterpreter](./ForImplementers/Reference/RunTime/JitInterpreter.md)
    - [NativeInterpreter](./ForImplementers/Reference/RunTime/NativeInterpreter.md)
  - [Analysis](./ForImplementers/APIReference.md#analysis)
    - [ReflectionVector](./ForImplementers/Reference/Analysis/ReflectionVector.md)
    - [ReflectionString](./ForImplementers/Reference/Analysis/ReflectionString.md)
  - [Core](./ForImplementers/APIReference.md#core)


# Language Guide

- [Syntax and semantics](./SyntaxAndSemantics.md)
  - [Syntax](./SyntaxAndSeamantics/Syntax.md)

  - [Concepts]()
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
  - [Containers](./Standardlibrary/Standardlibrary.md#containers)
    - [Array\<T>](./Standardlibrary/Containers/Array.md)
    - [Vector\<T>](./Standardlibrary/Containers/Vector.md)
    - [String_t\<T>](./Standardlibrary/Containers/String_t.md)

  - [Alias Types](./Standardlibrary/Standardlibrary.md#alias-types)
    - [String](./Standardlibrary/Alias/String.md)
    - [StringSpan](./Standardlibrary/Alias/StringSpan.md)
  
    - [String8](./Standardlibrary/Alias/String8.md)
    - [String8Span](./Standardlibrary/Alias/String8Span.md)
  
    - [String16](./Standardlibrary/Alias/String16.md)
    - [String16Span](./Standardlibrary/Alias/String16Span.md)

    - [String32](./Standardlibrary/Alias/String32.md)
    - [String32Span](./Standardlibrary/Alias/String32Span.md)
  
  - [ReferenceTypes](./Standardlibrary/Standardlibrary.md#referencetypes)
    - [StringSpan_t\<T>](./Standardlibrary/ReferenceTypes/StringSpan.md)
    - [Span\<T>](./Standardlibrary/ReferenceTypes/Span.md)

  - [Math](./Standardlibrary/Standardlibrary.md#math-types) 
    - [Vec2](./Standardlibrary/Math/Vec2.md)
    - [Vec3](./Standardlibrary/Math/Vec3.md)
    - [Vec2i](./Standardlibrary/Math/Vec2i.md)
    - [Vec3i](./Standardlibrary/Math/Vec3i.md)

    - [Vec2_t\<T>](./Standardlibrary/Math/Vec2_t.md)
    - [Vec3_t\<T>](./Standardlibrary/Math/Vec3_t.md)
    - [Vec2i_t\<T>](./Standardlibrary/Math/Vec2i_t.md)
    - [Vec3i_t\<T>](./Standardlibrary/Math/Vec3i_t.md)

  - [Memory](./Standardlibrary/Standardlibrary.md#memory)
    - [Unique_ptr\<T>](./Standardlibrary/Memory/Unique_ptr.md)
    - [Shared_ptr\<T>](./Standardlibrary/Memory/Shared_ptr.md)

  - [IO](./Standardlibrary/Standardlibrary.md#inputoutput-file)
    - [System](./Standardlibrary/Standardlibrary.md#inputoutput-system)
      - [Fmt](./Standardlibrary/IOSystem/Fmt.md)

    - [File](./Standardlibrary/Standardlibrary.md#inputoutput-file)
      - [File](./Standardlibrary/IOFile/File.md)

    - [Networking](./Standardlibrary/Standardlibrary.md#inputoutput-networking)

    - [Serialization](./Standardlibrary/Standardlibrary.md#ungrouped)

  - [Miscellaneous](./Standardlibrary/Standardlibrary.md#miscellaneous)
    - [Empty](./Standardlibrary/Miscellaneous/Empty.md)

- [KeyWords](./SyntaxAndSeamantics/Keywords.md)
  - [Built-in Types](./SyntaxAndSeamantics/Keywords.md#built-in-types)
    - [int](./SyntaxAndSeamantics/Builtin/ints.md)
    - [byte](./SyntaxAndSeamantics/Builtin/ints.md#byte)
    - [bool](./SyntaxAndSeamantics/Builtin/bool.md)
    - [char](./SyntaxAndSeamantics/Builtin/char.md#char)
    - [utf8](./SyntaxAndSeamantics/Builtin/char.md#utf8)
    - [Utf16](./SyntaxAndSeamantics/Builtin/char.md#utf16)
    - [Utf32](./SyntaxAndSeamantics/Builtin/char.md#utf32)
    - [float](./SyntaxAndSeamantics/Builtin/float.md)
    - [float32](./SyntaxAndSeamantics/Builtin/float.md#float32)
    - [float64](./SyntaxAndSeamantics/Builtin/float.md#float64)
    - [uintptr](./SyntaxAndSeamantics/Builtin/ints.md#uintptr)
    - [intptr](./SyntaxAndSeamantics/Builtin/ints.md#intptr)
    - [this](./SyntaxAndSeamantics/Builtin/this.md)
    - [void](./SyntaxAndSeamantics/Builtin/void.md)
    - [typeinfo](./SyntaxAndSeamantics/Builtin/TypeInfo.md)
    - [var](./SyntaxAndSeamantics/Builtin/Var.md)
    - [uint8](./SyntaxAndSeamantics/Builtin/ints.md#uint8)
    - [uint16](./SyntaxAndSeamantics/Builtin/ints.md#uint16)
    - [uint32](./SyntaxAndSeamantics/Builtin/ints.md#uint32)
    - [uint64](./SyntaxAndSeamantics/Builtin/ints.md#uint64)
    - [int8](./SyntaxAndSeamantics/Builtin/ints.md#int8)
    - [int16](./SyntaxAndSeamantics/Builtin/ints.md#int16)
    - [int32](./SyntaxAndSeamantics/Builtin/ints.md#int32)
    - [int64](./SyntaxAndSeamantics/Builtin/ints.md#int64)
  - [Types](./SyntaxAndSeamantics/Keywords.md#type-keywords)
    - [enum](./SyntaxAndSeamantics/Types/Enum.md)
    - [trait](./SyntaxAndSeamantics/Types/trait.md)
    - [tag](./SyntaxAndSeamantics/Types/tag.md)
    - [dynamic](./SyntaxAndSeamantics/Types/dynamic.md)
  - [Memory](./SyntaxAndSeamantics/Keywords.md#memory-keyword)
    - [new](./SyntaxAndSeamantics/Memory/new.md)
    - [drop](./SyntaxAndSeamantics/Memory/drop.md)
    - [unsafe](./SyntaxAndSeamantics/Memory/unsafe.md)
    - [bitcast](./SyntaxAndSeamantics/Memory/bitcast.md)
    - [shr](./SyntaxAndSeamantics/Memory/shr.md)
    - [unq](./SyntaxAndSeamantics/Memory/unq.md)
  - [Varable Types](./SyntaxAndSeamantics/Keywords.md#varable-types-keywords)
    - [static](./SyntaxAndSeamantics/VarableTypes/static.md)
    - [thread](./SyntaxAndSeamantics/VarableTypes/thread.md)
    - [eval](./SyntaxAndSeamantics/VarableTypes/eval.md)
  - [Compile Time](./SyntaxAndSeamantics/Keywords.md#compile-time-keywords)
    - [compiler](./SyntaxAndSeamantics/CompileTime/compiler.md)
    - [invaild](./SyntaxAndSeamantics/CompileTime/invalid.md)
    - [type](./SyntaxAndSeamantics/CompileTime/type.md)
    - [valid](./SyntaxAndSeamantics/CompileTime/valid.md)
  
  - [Miscellaneous](./SyntaxAndSeamantics/Keywords.md#miscellaneous-keywords)
    - [for](./SyntaxAndSeamantics/Miscellaneous/for.md)
    - [import](./SyntaxAndSeamantics/Miscellaneous/import.md)
    - [out](./SyntaxAndSeamantics/Miscellaneous/out.md)
    - [panic](./SyntaxAndSeamantics/Miscellaneous/Panic.md)
    - [use](./SyntaxAndSeamantics/Miscellaneous/use.md)

