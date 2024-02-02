# Reference/C89Backend

## Definition
HeaderFile: UCodeLang/Compliation/Back/WebAssembly/WasmBackEnd.hpp

Is Included In UCodeLang.hpp: No

The BackEnd to output a WebAssembly Module.
```cpp
class BackEndObject;
```
## Example

```cpp
#include "UCodeLang.hpp"
#include "UCodeLang/Compliation/Back/WebAssembly/WasmBackEnd.hpp"

void main()
{
    UCodeLang::Compiler _Compiler;
	_Compiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
	//You set the backend to output a Wasm.
}

```

## MemberFuncions
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
Reset | None | void | Resets The BackEnd to be used again.
Build | (const IRBuilder* Input) | void | Generates a WebAssembly Module from Input.

## Static Members
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
| MakeObject | None | BackEndObject* | Returns a pointer heap allocated to a BackEndObject.Must be freed.
ToWebName | const String& Value | void | Converts a identifier to a valid Wasm indentifier.can be used to find a funcion from the UClib ClassMethod using the DecorationName Field.
## Notes

Like All BackEnds UCodeBackEnd is not meant to be directly but instead used from the [Compiler](../Compiler/Compiler.md).