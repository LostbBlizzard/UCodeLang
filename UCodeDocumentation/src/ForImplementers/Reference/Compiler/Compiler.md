# Reference/Compiler

## Definition
HeaderFile: UCodeLang/Compliation/Compiler.hpp

Is Included In UCodeLang.hpp: Yes

The UCodeLang Compiler.

```cpp
class Compiler;
```
## Example


>[!IMPORTANT]
>This Is section Is incomplete
>

## Methods
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
CompileText | (const String_view& Text, const ExternalFiles& ExternalFiles = {}) | CompilerRet | Compiles UCodeLang Text and returns the Result.
CompileFileToLib | (const Path& path, const ExternalFiles& ExternalFiles = {}) | CompilerRet | Compiles UCodeLang Text and returns the Result.
CompilePathToObj | (const Path& path, const Path& OutLib, const ExternalFiles& ExternalFiles = {}) | CompilerRet | Compiles UCodeLang File at the Path and returns the Result and Outputs at the OutLib.
CompileFiles | (const CompilerPathData& Data, const ExternalFiles& ExternalFiles = {}) | CompilerRet | Compiles UCodeLang using CompilerPathData and returns the Result.
CompileFiles_UseIntDir | (const CompilerPathData& Data, const ExternalFiles& ExternalFiles = {}) | CompilerRet | Compiles UCodeLang using CompilerPathData and returns the Result and Outputs intermediates file.
Set_FrontEnd | (FrontEndObject_Ptr Value) | void | Sets The FrontEnd.
Set_BackEnd | (BackEndObject_Ptr Value) | void | Sets The BackEnd.
GetBackEndName | None | String | Gets The BackEnd BackEnd Name.
GetOutputExtWithDot | None | String | Gets The Output file extension.
GetOutputBackEndInfo | None | CompliationBackEndInfo | Gets The Metadata about the BackEnd

## Types 

### CompilerPathData
```cpp
struct CompilerPathData
{
    Path FileDir;
    Path IntDir;
    Path OutFile;
}
```
### ExternalFiles
```cpp
struct ExternalFiles
{
	Vector<Path> Files;
};
```
### CompliationSuccess
```cpp
struct CompliationSuccess
{
	NeverNullPtr<UClib> OutPut;
	Optional<BytesPtr> OutFile;
}
```
### CompilerRet
```cpp
using CompilerRet = Result<CompliationSuccess,NeverNullPtr<CompliationErrors>>;
```


## Notes