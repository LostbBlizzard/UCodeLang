# Summary

- [Introduction](./Introduction.md)
- [Language and Why](./About.md)
- [Syntax](./SyntaxAndSeamantics/Syntax.md)

# User Guide
- [Geting Started Users](./GetingStartedUsers.md)
  - [ULangModule](./Users/ULangModule.md) 
  - [BuildScript.uc](./Users/BuildScript.md)

- [UCode Command line tool](./UCodeCL/UCodeCl.md)
  - [Commands](./UCodeCL/UCodeCl.md)
    - [help](./UCodeCL/Commands/Help.md)
    - [new](./UCodeCL/Commands/New.md)
    - [build](./UCodeCL/Commands/Build.md)
    - [run](./UCodeCL/Commands/Run.md)
    - [get](./UCodeCL/Commands/Get.md)
    - [dump](./UCodeCL/Commands/Dump.md)
    - [clean](./UCodeCL/Commands/Clean.md)
    - [cleanall](./UCodeCL/Commands/Cleanall.md)
    - [fmt](./UCodeCL/Commands/Fmt.md)
    - [cpptoulangvm](./UCodeCL/Commands/cpptoulangvm.md)
    - [cppdirtoulangvm](./UCodeCL/Commands/cppdirtoulangvm.md)
    - [test](./UCodeCL/Commands/Test.md)
    - [update](./UCodeCL/Commands/Update.md)
    - [updatetools](./UCodeCL/Commands/Updatetools.md)
    - [uninstalltools](./UCodeCL/Commands/Uninstalltools.md)
    - [index](./UCodeCL/Commands/Index.md)
    - [install](./UCodeCL/Commands/Install.md)
    - [runmod](./UCodeCL/Commands/Runmod.md)
    - [addmod](./UCodeCL/Commands/AddMod.md)
    - [uninstallmod](./UCodeCL/Commands/Uninstallmod.md)
    - [exits](./UCodeCL/Commands/Exits.md)
# Implementer Guide

- [Geting Started Implementers](./ForImplementers.md)
  - [Compileing Project](./ForImplementers.md)
  - [Calling C++ from UCodeLang](./ForImplementers/CallingCppfromUCodeLang.md)
  - [Automatic binding generation](./ForImplementers/Automaticbindinggeneration.md)
  - [Modules and Standard Library](./ForImplementers/StandardLibAndModules.md)

- [Library API Reference](./ForImplementers/APIReference.md)
  - [C API Reference](./ForImplementers/Reference/CAPI.md)
  - [Compilation](./ForImplementers/APIReference.md)
    - [Compiler](./ForImplementers/Reference/Compiler/Compiler.md)
    - [ModuleFile](./ForImplementers/Reference/Compiler/ModuleFile.md)
    - [UClib](./ForImplementers/Reference/Compiler/UClib.md)
    - [UCodeBackEnd](./ForImplementers/Reference/BackEnds/UCodeBackEnd.md)
    - [C11BackEnd](./ForImplementers/Reference/BackEnds/C11BackEnd.md)
    - [WasmBackEnd](./ForImplementers/Reference/BackEnds/WasmBackEnd.md)

  - [RunTime](./ForImplementers/APIReference.md)
    - [RunTimeLib](./ForImplementers/Reference/RunTime/RunTimeLib.md)
    - [UCodeRunTime](./ForImplementers/Reference/RunTime/UCodeRunTime.md)
    - [AnyInterpreter](./ForImplementers/Reference/RunTime/AnyInterpreter.md)
    - [Interpreter](./ForImplementers/Reference/RunTime/Interpreter.md)
    - [JitInterpreter](./ForImplementers/Reference/RunTime/JitInterpreter.md)
    - [NativeInterpreter](./ForImplementers/Reference/RunTime/NativeInterpreter.md)
    - [IOLink](./ForImplementers/Reference/RunTime/IOLink.md)
    - [SandBoxedIOLink](./ForImplementers/Reference/RunTime/SandBoxedIOLink.md)
    - [UtilityLink](./ForImplementers/Reference/RunTime/UtilityLink.md)
  - [Analysis](./ForImplementers/APIReference.md)
    - [ReflectionVector](./ForImplementers/Reference/Analysis/ReflectionVector.md)
    - [ReflectionString](./ForImplementers/Reference/Analysis/ReflectionString.md)
  - [Core](./ForImplementers/APIReference.md)


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
  - [Containers](./Standardlibrary/Standardlibrary.md)
    - [Array\<T>](./Standardlibrary/Containers/Array.md)
    - [Vector\<T>](./Standardlibrary/Containers/Vector.md)
    - [String_t\<T>](./Standardlibrary/Containers/String_t.md)

  - [Alias Types](./Standardlibrary/Standardlibrary.md)
    - [String](./Standardlibrary/Alias/String.md)
    - [StringSpan](./Standardlibrary/Alias/StringSpan.md)
  
    - [String8](./Standardlibrary/Alias/String8.md)
    - [String8Span](./Standardlibrary/Alias/String8Span.md)
  
    - [String16](./Standardlibrary/Alias/String16.md)
    - [String16Span](./Standardlibrary/Alias/String16Span.md)

    - [String32](./Standardlibrary/Alias/String32.md)
    - [String32Span](./Standardlibrary/Alias/String32Span.md)
  
  - [ReferenceTypes](./Standardlibrary/Standardlibrary.md)
    - [StringSpan_t\<T>](./Standardlibrary/ReferenceTypes/StringSpan.md)
    - [Span\<T>](./Standardlibrary/ReferenceTypes/Span.md)

  - [Math](./Standardlibrary/Standardlibrary.md) 
    - [Math](./Standardlibrary/Math/Math.md)
    - [Vec2](./Standardlibrary/Math/Vec2.md)
    - [Vec3](./Standardlibrary/Math/Vec3.md)
    - [Vec2i](./Standardlibrary/Math/Vec2i.md)
    - [Vec3i](./Standardlibrary/Math/Vec3i.md)

    - [Vec2_t\<T>](./Standardlibrary/Math/Vec2_t.md)
    - [Vec3_t\<T>](./Standardlibrary/Math/Vec3_t.md)
    - [Vec2i_t\<T>](./Standardlibrary/Math/Vec2i_t.md)
    - [Vec3i_t\<T>](./Standardlibrary/Math/Vec3i_t.md)
  - [Error Handleing](./Standardlibrary/Standardlibrary.md)
    - [Optional](./Standardlibrary/ErrorHandleing/Optional.md)
    - [Result](./Standardlibrary/ErrorHandleing/Result.md)
  - [Memory](./Standardlibrary/Standardlibrary.md)
    - [Unique_ptr\<T>](./Standardlibrary/Memory/Unique_ptr.md)
    - [Shared_ptr\<T>](./Standardlibrary/Memory/Shared_ptr.md)
    - [LowLevel](./Standardlibrary/Memory/LowLevel.md)
    - [Ptr\<T>](./Standardlibrary/Memory/Ptr.md)
    - [VoidPtr](./Standardlibrary/Memory/VoidPtr.md)
    - [Ptr\<void>](./Standardlibrary/Memory/VoidPtr.md)

  - [IO](./Standardlibrary/Standardlibrary.md)
    - [System](./Standardlibrary/Standardlibrary.md)
      - [Fmt](./Standardlibrary/IOSystem/Fmt.md)

    - [File](./Standardlibrary/Standardlibrary.md)
      - [File](./Standardlibrary/IOFile/File.md)

    - [Networking](./Standardlibrary/Standardlibrary.md)

    - [Serialization](./Standardlibrary/Standardlibrary.md)
  - [Threading](./Standardlibrary/Standardlibrary.md)
    - [Atomic](./Standardlibrary/Thread/Atomic.md)
    - [AtomicShared_ptr](./Standardlibrary/Thread/AtomicShared_ptr.md)
    - [Thread](./Standardlibrary/Thread/Thread.md)

  - [Miscellaneous](./Standardlibrary/Standardlibrary.md)
    - [Empty](./Standardlibrary/Miscellaneous/Empty.md)
    - [Any](./Standardlibrary/Miscellaneous/Any.md)
    - [Buffer_t](./Standardlibrary/Miscellaneous/Buffer_t.md)
    - [StringBuffer_t](./Standardlibrary/Miscellaneous/StringBuffer_t.md)
    - [Range_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)
    - [RangeFrom_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)
    - [RangeTo_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)
    - [RangeFull_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)
    - [RangeInclusive_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)
    - [RangeToInclusive_t\<T>](./Standardlibrary/Miscellaneous/Ranges.md)


- [KeyWords](./SyntaxAndSeamantics/Keywords.md)
  - [Built-in Types](./SyntaxAndSeamantics/Keywords.md)
    - [int](./SyntaxAndSeamantics/Builtin/ints.md)
    - [byte](./SyntaxAndSeamantics/Builtin/ints.md)
    - [bool](./SyntaxAndSeamantics/Builtin/bool.md)
    - [char](./SyntaxAndSeamantics/Builtin/char.md)
    - [utf8](./SyntaxAndSeamantics/Builtin/char.md)
    - [utf16](./SyntaxAndSeamantics/Builtin/char.md)
    - [utf32](./SyntaxAndSeamantics/Builtin/char.md)
    - [float](./SyntaxAndSeamantics/Builtin/float.md)
    - [float32](./SyntaxAndSeamantics/Builtin/float.md)
    - [float64](./SyntaxAndSeamantics/Builtin/float.md)
    - [uintptr](./SyntaxAndSeamantics/Builtin/ints.md)
    - [intptr](./SyntaxAndSeamantics/Builtin/ints.md)
    - [this](./SyntaxAndSeamantics/Builtin/this.md)
    - [void](./SyntaxAndSeamantics/Builtin/void.md)
    - [typeinfo](./SyntaxAndSeamantics/Builtin/TypeInfo.md)
    - [var](./SyntaxAndSeamantics/Builtin/Var.md)
    - [uint8](./SyntaxAndSeamantics/Builtin/ints.md)
    - [uint16](./SyntaxAndSeamantics/Builtin/ints.md)
    - [uint32](./SyntaxAndSeamantics/Builtin/ints.md)
    - [uint64](./SyntaxAndSeamantics/Builtin/ints.md)
    - [int8](./SyntaxAndSeamantics/Builtin/ints.md)
    - [int16](./SyntaxAndSeamantics/Builtin/ints.md)
    - [int32](./SyntaxAndSeamantics/Builtin/ints.md)
    - [int64](./SyntaxAndSeamantics/Builtin/ints.md)
  - [Types](./SyntaxAndSeamantics/Keywords.md)
    - [enum](./SyntaxAndSeamantics/Types/Enum.md)
    - [trait](./SyntaxAndSeamantics/Types/trait.md)
    - [tag](./SyntaxAndSeamantics/Types/tag.md)
    - [dynamic](./SyntaxAndSeamantics/Types/dynamic.md)
  - [Memory](./SyntaxAndSeamantics/Keywords.md)
    - [new](./SyntaxAndSeamantics/Memory/new.md)
    - [drop](./SyntaxAndSeamantics/Memory/drop.md)
    - [unsafe](./SyntaxAndSeamantics/Memory/unsafe.md)
    - [bitcast](./SyntaxAndSeamantics/Memory/bitcast.md)
    - [shr](./SyntaxAndSeamantics/Memory/shr.md)
    - [unq](./SyntaxAndSeamantics/Memory/unq.md)
  - [Varable Types](./SyntaxAndSeamantics/Keywords.md)
    - [static](./SyntaxAndSeamantics/VarableTypes/static.md)
    - [thread](./SyntaxAndSeamantics/VarableTypes/thread.md)
    - [eval](./SyntaxAndSeamantics/VarableTypes/eval.md)
  - [Compile Time](./SyntaxAndSeamantics/Keywords.md)
    - [compiler](./SyntaxAndSeamantics/CompileTime/compiler.md)
    - [invaild](./SyntaxAndSeamantics/CompileTime/invalid.md)
    - [type](./SyntaxAndSeamantics/CompileTime/type.md)
    - [valid](./SyntaxAndSeamantics/CompileTime/valid.md)
  
  - [Miscellaneous](./SyntaxAndSeamantics/Keywords.md)
    - [for](./SyntaxAndSeamantics/Miscellaneous/for.md)
    - [import](./SyntaxAndSeamantics/Miscellaneous/import.md)
    - [out](./SyntaxAndSeamantics/Miscellaneous/out.md)
    - [use](./SyntaxAndSeamantics/Miscellaneous/use.md)
    - [panic](./SyntaxAndSeamantics/Miscellaneous/panic.md)
    - [defer](./SyntaxAndSeamantics/Miscellaneous/defer.md)

