# Reference/UCodeBackEnd

## Definition

HeaderFile: [UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp](https://github.com/LostbBlizzard/UCodeLang/tree/master/UCodeLang/UCodeLang/Compilation/Back/UCodeBackEnd)

Is Included In UCodeLang.hpp: No

The BackEnd for the [Interpreter](../RunTime/Interpreter.md) or [Jit Interpreter](../RunTime/JitInterpreter.md).

```cpp
class UCodeBackEnd;
```

## Example

```cpp

#include "UCodeLang.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"

void main()
{
    UCodeLang::Compiler _Compiler;
	_Compiler.Set_BackEnd(UCodeLang::UCodeBackEnd::MakeObject);
	//You set the backend to target the Interpreter or jit-Interpreter
}

```

## MemberFuncions

| Funcion | Parameters                | Return | Description                          |
| ------- | ------------------------- | ------ | ------------------------------------ |
| Reset   | None                      | void   | Resets The BackEnd to be used again. |
| Build   | (const IRBuilder\* Input) | void   | Generates a UClib from Input.        |

## Static Members

| Funcion    | Parameters | Return          | Description                                                        |
| ---------- | ---------- | --------------- | ------------------------------------------------------------------ |
| MakeObject | None       | BackEndObject\* | Returns a pointer heap allocated to a BackEndObject.Must be freed. |

## Notes

Like All BackEnds UCodeBackEnd is not meant to be directly but instead used from the [Compiler](../Compiler/Compiler.md).
