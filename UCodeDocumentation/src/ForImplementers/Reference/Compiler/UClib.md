# Reference/UClib

## Definition

HeaderFile: [UCodeLang/LangCore/UClib.hpp](https://github.com/LostbBlizzard/UCodeLang/blob/master/UCodeLang/UCodeLang/LangCore/UClib.hpp)

Is Included In UCodeLang.hpp: Yes

The UCodeLang Compiler Output.

```cpp
class UClib;
```

## Example

> [!IMPORTANT]
> This Is section Is incomplete

## Methods

| Funcion      | Parameters         | Return                               | Description                                                        |
| ------------ | ------------------ | ------------------------------------ | ------------------------------------------------------------------ |
| Get_Assembly | None               | [ClassAssembly](./ClassAssembly.md)& | Gets the ClassAssembly.                                            |
| WriteToFile  | (const Path& path) | bool                                 | Write the UClib File to the path and returns true if it succeeded. |

## Static Funcions

| Funcion  | Parameters                            | Return                         | Description                                                        |
| -------- | ------------------------------------- | ------------------------------ | ------------------------------------------------------------------ |
| ToFile   | (const UClib\* Lib, const Path& path) | bool                           | Write the UClib File to the path and returns true if it succeeded. |
| FromFile | (UClib\* Lib, const Path& path)       | bool                           | Get the UClib File from the path and returns true if it succeeded  |
| OpenFile | (const Path& path)                    | Optional\<[UClib](./UClib.md)> | Get the UClib File from the path and returns Nothing if it failed  |
