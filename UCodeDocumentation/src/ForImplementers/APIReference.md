# API Reference

The API Reference is spit into Three parts and Compiler and RunTime.

Every thing in UCodeLang in UCodeLang NameSpace

```
namespace UC = UCodeLang;
```

## Core
Basic types that UCodeLang Uses stop isn't part of C++17 standard Library.


## Compiler
Types related to compiling and its Output.


| Name | Type | Description |
|--- | --- |  --- | 
[Compiler](./Reference/Compiler/Compiler.md) | Class | The UCodeLang Compiler.
[ModuleFile](./Reference/Compiler/ModuleFile.md) | Class | The UCodeLang Compiler Module System.
[UClib](./Reference/Compiler/ModuleFile.md) | Class | The UCodeLang Compiler Output.
[UCodeBackEnd](./Reference/BackEnds/UCodeBackEnd.md) | Class | The BackEnd for the UCodeInterpreter.
[C11BackEnd](./Reference/BackEnds/C11BackEnd.md) | Class | The BackEnd to output C Code.
[WasmBackEnd](./Reference/BackEnds/WasmBackEnd.md) | Class | The BackEnd targeting Wasm.

## RunTime
Types related to The UCodeVM.


## C API
UCodeLang Has a [C API](./Reference/CAPI.md) for using UCodeLang in  Programming Languages Progaming other than C++.

## Notes