# API Reference

The API Reference is spit into Three parts and Compiler and RunTime.

Every thing in UCodeLang in UCodeLang NameSpace

```cpp
namespace UC = UCodeLang;
```

## Compilation

Types related to compiling and its Output.

| Name                                                 | Type  | Description                           |
| ---------------------------------------------------- | ----- | ------------------------------------- |
| [Compiler](./Reference/Compiler/Compiler.md)         | Class | The UCodeLang Compiler.               |
| [ModuleFile](./Reference/Compiler/ModuleFile.md)     | Class | The UCodeLang Compiler Module System. |
| [UClib](./Reference/Compiler/ModuleFile.md)          | Class | The UCodeLang Compiler Output.        |
| [UCodeBackEnd](./Reference/BackEnds/UCodeBackEnd.md) | Class | The BackEnd for the UCodeInterpreter. |
| [C11BackEnd](./Reference/BackEnds/C11BackEnd.md)     | Class | The BackEnd to output C Code.         |
| [WasmBackEnd](./Reference/BackEnds/WasmBackEnd.md)   | Class | The BackEnd targeting Wasm.           |

## RunTime

Types related to The UCodeVM/RunTime.

| Name                                                          | Type  | Description                                                                 |
| ------------------------------------------------------------- | ----- | --------------------------------------------------------------------------- |
| [RunTimeLib](./Reference/RunTime/RunTimeLib.md)               | Class | The BackEnd to output C Code.                                               |
| [UCodeRunTime](./Reference/RunTime/UCodeRunTime.md)           | Class | The Core UCodeLang Runtime.                                                 |
| [AnyInterpreter](./Reference/RunTime/AnyInterpreter.md)       | Class | A wapper over Interpreter,JitInterpreter and NativeInterpreter.             |
| [Interpreter](./Reference/RunTime/Interpreter.md)             | Class | The Default Interpreter.                                                    |
| [JitInterpreter](./Reference/RunTime/JitInterpreter.md)       | Class | The Jit Interpreter.                                                        |
| [NativeInterpreter](./Reference/RunTime/NativeInterpreter.md) | Class | Runs Precompiled Native Code.                                               |
| [IOLink](./Reference/RunTime/IOLink.md)                       | Class | Adds CppCalls for direct access to the file system,network and other IO.    |
| [SandBoxedIOLink](./Reference/RunTime/SandBoxedIOLink.md)     | Class | Adds CppCalls for SandBoxed access to the file system,network and other IO. |
| [UtilityLink](./Reference/RunTime/SandBoxedIOLink.md)         | Class | Adds CppCalls for everything else that doesn't fit in IO.                   |

## Analysis

Types related to The Analyzing UCodeLang ,UCodeLang Output or its Runtime.

| Name                                                         | Type  | Description                                   |
| ------------------------------------------------------------ | ----- | --------------------------------------------- |
| [ReflectionVector](./Reference/Analysis/ReflectionVector.md) | Class | Lets you use UCodeLang Vectors in a Easy Way. |
| [ReflectionString](./Reference/Analysis/ReflectionString.md) | Class | Lets you use UCodeLang Strings in a Easy Way. |

## Core

Basic types that UCodeLang Uses stop isn't part of C++17 standard Library.

| Name         | Type          | Description                                        |
| ------------ | ------------- | -------------------------------------------------- |
| Result       | Generic class | A type holding a returned value or an error code.  |
| NeverNullPtr | Generic class | A pointer that should never be null.               |
| NullablePtr  | Generic class | A pointer that could be null.                      |
| Variant      | Generic class | A wapper over std::variant give it more functions. |
| Span         | Generic class | A contiguous sequence of objects                   |

## C API

UCodeLang Has a [C API](./Reference/CAPI.md) for using UCodeLang in Programming Languages other than C++.

## Notes

Many more classes and funcions can be found in UCodeLang but may change may change on a whim its recommended not to used those them if its not documented here.
