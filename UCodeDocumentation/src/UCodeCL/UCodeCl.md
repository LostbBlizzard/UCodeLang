# UCodeCL command-line reference

The UCodeLang Command-line tool it serves as a Compiler,Runtime,build system and package manager for UCodeLang.

# Syntax


```
uclang [command] <arguments>
```

# Commands
| Name | arguments | Description |
|--- |--- |--- |
[help](./Commands/New.md) | none | Outputs all the all the available commands.
[new](./Commands/New.md) | none   | makes a new UCodeLang Module Project.
[build](./Commands/Build.md) | Path(optional),flags(optional)  | Builds a UCodeLang File,Module,Folder.
[run](./Commands/Run.md) | Path(optional),flags(optional)  | Builds a UCodeLang File,Module,Folder or .ulib and runs it. [See More](#Run-Command)
[get](./Commands/Get.md) | Path(optional)  | Downloads the Modules dependencies.
[dump](./Commands/Dump.md) | Path  | Builds a UCodeLang File,Module,Folder or .ulib and attempts to convert the output into readable textworks with wasm,window,linux,macos,c89,.ulib,.uir outputs.
[clean](./Commands/Clean.md) | Path| clears a Module a intermediate and output files.
[cleanall](./Commands/Cleanall.md) | none | clears all Modules on the system intermediate and output files.
[cpptoulangvm](./Commands/cpptoulangvm.md) | CppPath,CpplinkPath,ulangout  | uses C++ source code to create bindings for UCodeVm
[cppdirtoulangvm](./Commands/cpptoulangvm.md) | CppDir,CpplinkPath,ulangout  | uses C++ source files code to create bindings for UCodeVm
[fmt](./Commands/Fmt.md) | Path(optional)   |Formats A UCodeLang Module or file.
[index](./Commands/Index.md) | Path(optional)   | Adds Module to to the Module Index so it can be used/referenced by other module in your system.
[test](./Commands/Test.md) | Path(optional)   | Runs Tests on your UCodeLang Module Project.
[update](./Commands/Update.md) | Path(optional)   | updates Module Dependencies uses Tests to avoid breaking changes and revert back if needed this may take Sometime.
[updatetools](./Commands/Updatetools.md) | none   | updates UCodeLangTools.
[uninstalltools](./Commands/Uninstalltools.md) | none | uninstalls uclang and uclanglsp all downloaded modules will be left alone.
[install](./Commands/Install.md) | Name   | Installs a UCodeLangModule.
[addmod](./Commands/AddMod.md) | Name,Path(optional)   | runs a Installed and adds the Module to the project.
[runmod](./Commands/Runmod.md) | Name   | runs a Installed UCodeLangModule.
[uninstallmod](./Commands/Uninstallmod.md)  | Name   | uninstalls a Installed UCodeLangModule
[exits](./Commands/Exits.md) | none | exits ucodelang if opened without command line arguments.


