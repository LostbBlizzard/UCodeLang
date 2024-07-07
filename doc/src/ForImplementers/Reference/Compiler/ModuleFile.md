# Reference/ModuleFile

## Definition

HeaderFile: [UCodeLang/Compliation/ModuleFile.hpp](https://github.com/LostbBlizzard/UCodeLang/blob/master/UCodeLang/UCodeLang/Compilation/ModuleFile.hpp)

Is Included In UCodeLang.hpp: No

The UCodeLang Compiler Module System.

```cpp
class ModuleFile;
```

## Example

> [!IMPORTANT]
> This Is section Is incomplete

## Methods

| Funcion         | Parameters                                                                                                 | Return    | Description                                                    |
| --------------- | ---------------------------------------------------------------------------------------------------------- | --------- | -------------------------------------------------------------- |
| BuildModule     | (Compiler& Compiler,const ModuleIndex& Modules,bool IsSubModule = false, OptionalRef<String> LogsOut = {}) | ModuleRet | Builds The Module Project.                                     |
| BuildFile       | (const String& filestring, Compiler& Compiler, const ModuleIndex& Modules)                                 | ModuleRet | Builds The Module file using the filestring as a Build Script. |
| DownloadModules | (const ModuleIndex& Modules,OptionalRef<String> LogsOut = {})                                              | bool      | Downloads The Modules Dependencies.                            |

## Static Funcions

| Funcion  | Parameters                                 | Return | Description                                                         |
| -------- | ------------------------------------------ | ------ | ------------------------------------------------------------------- |
| ToFile   | (const ModuleFile\* Lib, const Path& path) | bool   | Write the Module File to the path and returns true if it succeeded. |
| FromFile | (ModuleFile\* Lib, const Path& path)       | bool   | Get the Module File from the path and returns true if it succeeded  |

## Fields

| Type                       | Name               | Description                                                    |
| -------------------------- | ------------------ | -------------------------------------------------------------- |
| ModuleIdentifier           | ModuleName         | The Module identification.                                     |
| Path                       | ThisModuleDir      | The working directory of this Module.Is set fom FromFile.      |
| bool                       | ForceImport        | if true the Compiler will Force the Import System to be used   |
| bool                       | RemoveUnSafe       | if true the Compiler will ban the use of unsafe in the Module. |
| Vector\<ModuleDependencie> | ModuleDependencies | Modules this Module needs to Compile.                          |

## Static Members

## Types

## Notes
