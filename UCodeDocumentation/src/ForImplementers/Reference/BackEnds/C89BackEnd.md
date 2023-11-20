# Reference/C89Backend

## Definition
HeaderFile: UCodeLang/Compliation/Back/C89/C89Backend.hpp

Is Included In UCodeLang.hpp: No

The BackEnd to output C89 Code.
```cpp
class C89Backend;
```
## Example

```cpp
#include "UCodeLang.hpp"
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"

void main()
{
    UCodeLang::Compiler _Compiler;
	_Compiler.Set_BackEnd(UCodeLang::C89Backend::MakeObject);
	//You set the backend to output C89 Code.
}

```

## MemberFuncions
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
Reset | None | void | Resets The BackEnd to be used again.
Build | (const IRBuilder* Input) | void | Generates C89 Code from Input.

## Static Members
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
| MakeObject | None | BackEndObject* | Returns a pointer heap allocated to a BackEndObject.Must be freed.
UpdateToCindentifier | const String& Value | void | Converts a identifier to a valid C89 indentifier.can be used to find a funcion from the UClib ClassMethod using the DecorationName Field.
## Notes

Like All BackEnds UCodeBackEnd is not meant to be directly but instead used from the [Compiler](../Compiler/Compiler.md).